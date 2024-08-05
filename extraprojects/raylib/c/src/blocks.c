#ifndef OBH_BLOCKS
#define OBH_BLOCKS

typedef enum {
    BT_GRASSDIRT,
    BT_STONE,
    BT_DIRT,
    BT_DRYDIRT,
    BT_SAND,
    BT_SNOW,

    BT__NUM
} BLOCK_TYPE;

struct Block {
    ivec3 pos;
    BLOCK_TYPE type; 
};
typedef struct Block block;

const char *_blockTexturesFolder = "resources";
const char *_blockTextures[] = {
    "blocktexture_grassdirt.jpg",
    "stonebig.png",
    "drydirt.jpg",
    "drydirt.jpg",
    "drydirt.jpg",
    "puresnow.jpg",
    "grassdirt.jpg",
};

u32 _blockTextureIDs[BT__NUM];
u32 _blockVAOs[BT__NUM];
u32 _blockShaders[BT__NUM];

float _blockVertexData3Part[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.34f, 1.00f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.66f, 1.00f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.66f, 0.00f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.66f, 0.00f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.34f, 0.00f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.34f, 1.00f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.34f, 1.00f, 
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.66f, 1.00f, 
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.66f, 0.00f, 
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.66f, 0.00f, 
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.34f, 0.00f, 
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.34f, 1.00f, 

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.66f, 0.00f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.34f, 0.00f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.34f, 1.00f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.34f, 1.00f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.66f, 1.00f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.66f, 0.00f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.66f, 0.00f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.34f, 0.00f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.34f, 1.00f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.34f, 1.00f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.66f, 1.00f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.66f, 0.00f,
    // BOT
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.00f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.34f, 1.00f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.34f, 0.00f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.34f, 0.00f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.00f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.00f,
    // TOP
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.70f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.00f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.00f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.70f, 0.00f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.70f, 1.0f
};

float _blockVertexData[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    // BOT
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    // TOP
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

Material _blockMaterials[BT__NUM] = {
    {{0.8,  0.8,  0.8},
     {0.8,  0.8,  0.8},
     {0.05, 0.05, 0.05},
      4.0,  1 },
};


u32
blocksMakeVAO(const char *texturePath, int n)
{
    // GL BUFFER DATA STUFF
    u32 VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_blockVertexData3Part), _blockVertexData3Part, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0); 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6)); 
    glEnableVertexAttribArray(2);
    // TEXTURE
    u32 tex1;
    glGenTextures(1, &tex1);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1);

    int img_width, img_height, img_n_channels;
    u8 *img_data = stbi_load(texturePath, &img_width, &img_height, &img_n_channels, 0);
    if (img_data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 
                0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(img_data);
        fprintf(stderr, "succesfully loaded texture %s\n", texturePath);
    } else
        fprintf(stderr, "failed to load image");
    _blockTextureIDs[n] = tex1;
    _blockVAOs[n] = VAO;
    glBindVertexArray(0);
    return VAO;
}

bool
blocksInit(u32 VAOs[static BT__NUM])
{
    char texturePath[256] = {0};
    for (int i = 0; i < BT__NUM; ++i){
        sprintf(texturePath, "%s/%s", _blockTexturesFolder, _blockTextures[i]);
        VAOs[i] = blocksMakeVAO(texturePath, i);
    }
}

#endif
































