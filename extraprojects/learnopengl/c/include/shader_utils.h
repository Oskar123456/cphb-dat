#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H


#include "../include/obh_defs.h"
#include "includes_dot_haitch.h"

void shader_utils_set_shader_dir_vertex(const char *path);
void shader_utils_set_shader_dir_geometry(const char *path);
void shader_utils_set_shader_dir_fragment(const char *path);

u32 shader_utils_compile(u32 stype, const char *src);
void shader_utils_delete_shaders(u32 shaders_count, u32 *shaders);
u32 shader_utils_link_program(u32 shaders_count, u32 *shaders);
u32 shaderutilsMakeProgram(const char *vertName, const char *fragName);

#endif
