/*
 * THROWING ALL TYPES IN HERE WHY NOT
 */

#ifndef OBH_TYPES
#define OBH_TYPES

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
