#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H


#include "../include/obh_defs.h"
#include "includes_dot_haitch.h"

void shaderutilsSetPathVert(const char *path);
void shaderutilsSetPathGeom(const char *path);
void shaderutilsSetPathFrag(const char *path);

u32 shaderutilsCompile(u32 stype, const char *src);
void shaderutilsDelete(u32 shaders_count, u32 *shaders);
u32 shaderutilsLinkProgram(u32 shaders_count, u32 *shaders);
u32 shaderutilsMakeProgram(const char *vertName, const char *fragName);

#endif
