/*
 * THROWING ALL TYPES IN HERE WHY NOT
 */

#ifndef OBH_TYPES
#define OBH_TYPES

struct WorldGenCoord2D {
    i32 x, z;
};
typedef struct WorldGenCoord2D point2d;

struct WorldGenCoord3D {
    union {
        point2d key;
        point2d pos;
    };
    union {
        i32 value;
        i32 y;
        i32 h;
        i32 height;
    };
};
typedef struct WorldGenCoord3D point3d;


/* 
 * SHADER STUFF
 * */

struct LightSource {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shine;
    i32   type;
};

typedef struct LightSource LightSource; 
typedef struct Material Material; 

#endif
