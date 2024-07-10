/*
 * HEIGHT MAPS AND MORE
 */

#include "noise.c"

#define WORLDGEN_WORLD_LEN (1 << 24)
#define WORLDGEN_CHUNK_LEN 32
#define WORLDGEN_CHUNK_SIZE (WORLDGEN_CHUNK_LEN * WORLDGEN_CHUNK_LEN)
#define WORLDGEN_HEIGHT_NOISE_FREQ (WORLDGEN_WORLD_LEN >> 12)

/*
 *
 * NOTE: biomes could be implemented as points 
 * on map, and blocks would belong to nearest 
 * biome, but you also use interpolation, esp. 
 * during height calculations with nearest 
 * neighbor biome or something like that. 
 *
 * Think voroi diagrams: 
 * https://en.wikipedia.org/wiki/Fortune's_algorithm
 *
 */

typedef enum {
    BIOME_FOREST,
    BIOME_MOUNTAINS,
    BIOME_BEACH,
    BIOME_DESERT,

    BIOME__NUM
} WORLDGEN_BIOME;

struct WorldGenHints {
    u32 width;  // n blocks wide
    u32 length; // n blocks long
};
typedef struct WorldGenHints WorldGenHints;

struct WorldGenBiome {
    WORLDGEN_BIOME type;
    i32 mean, variance, detail;
    float plateau;
    point2d pos;
};
typedef struct WorldGenBiome WorldGenBiome;

struct WorldGenChunk {
    block blocks[WORLDGEN_CHUNK_LEN][WORLDGEN_CHUNK_LEN];
};
typedef struct WorldGenChunk WorldGenChunk;

struct WorldGenChunkMap {
    union {
        point2d key;
        point2d pos;
    };
    union {
        WorldGenChunk *value;
        WorldGenChunk *chunk;
    };
};
typedef struct WorldGenChunkMap WorldGenChunkMap;

struct WorldGenWorld {
    WorldGenBiome *biomes;
    float seed;
    WorldGenChunkMap *chunkMap;
};
typedef struct WorldGenWorld World;
/* Some defaults for initialising types of biomes, but 
 * should be able to make your own custom variations*/
void
worldgenInitBiome(WorldGenBiome *biome)
{
    if (biome->type < 0 || biome->type >= BIOME__NUM){
        fprintf(stderr, "worldgenInitBiome: invalid type");
        return;
    }
    if (biome->type == BIOME_FOREST){
        biome->mean = 10;
        biome->variance = 75;
        biome->detail = 5;
        biome->plateau = 1.0;
    }
    if (biome->type == BIOME_MOUNTAINS){
        biome->mean = 75;
        biome->variance = 400;
        biome->detail = 12;
        biome->plateau = 1.0;
    }
    if (biome->type == BIOME_BEACH){
        biome->mean = -45;
        biome->variance = 200;
        biome->detail = 6;
        biome->plateau = 1.0;
    }
}
/* Map block coordinates to world chunk */
void
worldgenTellChunk(World *world, i32 x, i32 z)
{

}
/* Map block coordinates to world biomes, 
 * returns sum of distances between xz and the biomes*/
WorldGenBiome*
worldgenTellNearestBiomes(World *world, i32 x, i32 z)
{
    int i,j;
    float sumDistances = 0.0;
    float minDist = FLT_MAX;
    WorldGenBiome *minDistBiome = NULL;
    for (j = 0; j < arrlen(world->biomes); ++j){
        WorldGenBiome biom = world->biomes[j];
        float dist = sqrt(pow((float)x - (float)biom.pos.x, 2.0) + pow((float)z - (float)biom.pos.z, 2.0));
        if (dist < minDist){
            minDist = dist;
            minDistBiome = &world->biomes[j];
        }
    }
    return minDistBiome;
}
/* Generate noise based on biome */
float
worldgenNoise(float x, float y, float z, u32 nSamples, float plateauEffect)
{
    float noiseFinal = 0.0;
    float noiseDenominator = 0.0;
    float noises[nSamples];
    for (int i = 0; i < nSamples; ++i){
        float freq = (float)((1 << 15) << i);
        noiseFinal += (1.0 / freq) * stb_perlin_noise3(
                freq * x, freq * y, z, 0, 0, 0);
        noiseDenominator += (1.0 / freq);
    }
    noiseFinal /= noiseDenominator;
    noiseFinal = pow(noiseFinal, plateauEffect);
    //printf("noise: %f\n", noiseFinal);
    return noiseFinal;
}
/* Interpolate height between blocks two 
 * nearest biomes */
float
worldgenComputeHeightAndType(World *world, block *block, i32 x, i32 z)
{
    float xNormed = (float)(x + WORLDGEN_WORLD_LEN / 2) / (float)WORLDGEN_WORLD_LEN;
    float zNormed = (float)(z + WORLDGEN_WORLD_LEN / 2) / (float)WORLDGEN_WORLD_LEN;

    WorldGenBiome *nearestBiome = worldgenTellNearestBiomes(world, x, z);       

    float elevation = worldgenNoise(xNormed, zNormed, world->seed, nearestBiome->detail, nearestBiome->plateau);

    block->type = nearestBiome->type;
    elevation = elevation * (float)nearestBiome->variance + (float)nearestBiome->mean;
    return elevation;
}
/* Yes */
void
worldgenChunk(World *world, WorldGenChunk *chunk, i32 x, i32 z)
{
    for (int i = 0; i < WORLDGEN_CHUNK_LEN; ++i){
        for (int j = 0; j < WORLDGEN_CHUNK_LEN; ++j){
            i32 xWorld = x * WORLDGEN_CHUNK_LEN + j;
            i32 zWorld = z * WORLDGEN_CHUNK_LEN + i;
            chunk->blocks[i][j] = (block){.pos[0] = xWorld, .pos[1] = 0.0, .pos[2] = zWorld, rand() % BT__NUM};
            float elevation = worldgenComputeHeightAndType(world, &chunk->blocks[i][j], xWorld, zWorld);
            chunk->blocks[i][j].pos[1] = (i32)elevation;

            int rnd = rand() % 10;
            if ((i32)elevation > 110 + rnd)
                chunk->blocks[i][j].type = BT_SNOW;
                
            //fprintf(stderr, "added block %d, %d, %d\n", 
            //      chunk->blocks[i][j].pos[0], 
            //      chunk->blocks[i][j].pos[1], 
            //      chunk->blocks[i][j].pos[2]);
        }
    }
}
/* Remake map COMPLETELY based on hints */
void
worldgenRemake(World *world)
{
    hmfree(world->chunkMap);
    arrfree(world->biomes);
    int nBiomes = 1;
    for (int i = 0; i < nBiomes; ++i){
        WorldGenBiome b = {rand() % 3, .pos =  (point2d){rand() % 400 , rand() % 400}};
        worldgenInitBiome(&b);
        arrput(world->biomes, b);
    }
}
