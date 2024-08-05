#include "../include/shader_utils.h"

static const char *vertex_dir;
static const char *geometry_dir;
static const char *fragment_dir;

void
shaderutilsSetPathVert(const char *path)
{
    vertex_dir = path;
}
void
shaderutilsSetPathGeom(const char *path)
{
    geometry_dir = path;
}
void
shaderutilsSetPathFrag(const char *path)
{
    fragment_dir = path;
}

u32
shaderutilsCompile(u32 shader_type, const char *src)
{
    const char *dir = NULL;
    if (shader_type == GL_VERTEX_SHADER)
        dir = vertex_dir;
    if (shader_type == GL_FRAGMENT_SHADER)
        dir = fragment_dir;
    if (shader_type == GL_GEOMETRY_SHADER)
        dir = geometry_dir;

    sds full_path = sdsnew(dir);
    full_path = sdscat(full_path, "/");
    full_path = sdscat(full_path, src);
    FILE *shader_source_file = fopen(full_path, "r");
    // sdsfree(full_path);
    if (shader_source_file == NULL){
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
    }
    fseek(shader_source_file, 0, SEEK_END);
    u64 shader_source_len = ftell(shader_source_file);
    rewind(shader_source_file);
    char shader_source_buf[shader_source_len + 1];
    if (fread(shader_source_buf, 1, shader_source_len, shader_source_file) < shader_source_len){
        fprintf(stderr, "shader_utils_compile: short count during file read of %s\n", src);
        fclose(shader_source_file);
        return 0;
    }
    shader_source_buf[shader_source_len] = 0;
    fclose(shader_source_file);
    
    u32 shader_id = glCreateShader(shader_type);
    char *shader_src_ptr = shader_source_buf;
    glShaderSource(shader_id, 1, (const GLchar**) &shader_src_ptr, NULL);
    glCompileShader(shader_id);
    i32 success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success){
        char compile_info[512] = {0};
        glGetShaderInfoLog(shader_id, 512, NULL, compile_info);
        fprintf(stderr, "shader_utils_compile: failed to compile: %s\n", compile_info);
    }
    // else
    //     fprintf(stderr, "shader_utils_compile: successfully compiled:\n%s\n", shader_source_buf);


    return shader_id;
}

void 
shaderutilsDelete(u32 shaders_count, u32 *shaders)
{
    for (int i = 0; i < shaders_count; ++i)
        glDeleteShader(shaders[i]);
}

u32 
shaderutilsLinkProgram(u32 shaders_count, u32 *shaders)
{
    u32 shader_program = glCreateProgram();
    for (int i = 0; i < shaders_count; ++i)
        glAttachShader(shader_program, shaders[i]);
    glLinkProgram(shader_program);

    int success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success){
        char buf[512] = {0};
        glGetProgramInfoLog(shader_program, 512, NULL, buf);
        fprintf(stderr, "shader_utils_link_program: failed to create and link program: %s\n", buf);
    }

    return shader_program;
}

u32
shaderutilsMakeProgram(const char *vertName, const char *fragName)
{
    u32 shaders[] = {
        shaderutilsCompile(GL_VERTEX_SHADER, vertName),
        shaderutilsCompile(GL_FRAGMENT_SHADER, fragName),
    };
    u32 program = shaderutilsLinkProgram(2, shaders);
    shaderutilsDelete(2, shaders);
    return program;
}
