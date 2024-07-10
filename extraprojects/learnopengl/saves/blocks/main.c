#include "../include/obh_defs.h"
#include "../include/includes_dot_haitch.h"
#include "../include/shader_utils.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"
#endif

#ifndef STB_DS_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION
#include "../external/stb/stb_ds.h"
#endif


#include "obh_types.h"
#include "shapes.c"
#include "camera.c"
#include "blocks.c"
#include "world_gen.c"

/*
 * **********************
 * ======================
 * obh C project template
 * ======================
 * **********************
 */

/* DECLARATIONS */
#define INIT_SCR_WIDTH 800
#define INIT_SCR_HEIGHT 600
#define INIT_GL_VER_MAJ 4
#define INIT_GL_VER_MIN 6

static int scr_width = INIT_SCR_WIDTH;
static int scr_height = INIT_SCR_HEIGHT;

static u32 UPS = 60;
static u32 frame_time_ms;
static u64 nsec_start;

static bool paused;

static bool  cursor_first_time = true;
static float cursor_last_x = INIT_SCR_WIDTH / 2.0;
static float cursor_last_y = INIT_SCR_HEIGHT / 2.0;;

static struct Camera player_camera = {
    .pos      = {0.0, 0.0, -110.0},
    .dir      = {0.0, 0.0,  1.0},
    .up       = {0.0, 1.0,  0.0},
    .up_world = {0.0, 1.0,  0.0},
    
    .yaw   = 90.0,
    .pitch = 00.0,
    .zoom  = 45.0,

    .sens   = 0.05,
    .speedCurrent  = 0.75,
    .speedRegular  = 0.75,
    .speedSprint  = 3.00,

    .draw_dist = 300.0,
    .restrict_pitch = false,
    .camtype = FLYING,
};


struct LightSource g_light = {
    {50, 50, 50},
    {0.9, 0.9, 0.9},
    {0.9, 0.9, 0.9},
    {0.9, 0.9, 0.9},
};

struct Material g_material = {
    {0.8, 0.8, 0.8},
    {0.8, 0.8, 0.8},
    {0.05, 0.05, 0.05},
    4.0, 1
};

/* LOGIC */
static bool cubes_should_rotate = true;
static bool use_texture = true;

World world = {.seed = 7.47};
i32 currentChunkX = 0;
i32 currentChunkZ = 0;

/* LOGIC FUNCTIONS */

/* WINDOW/GFX etc. FUNCTIONS */

void
framebuffer_size_callback(GLFWwindow *win, int width, int height)
{
    scr_width = width;
    scr_height = height;
    glViewport(0, 0, width, height);
}

void 
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        paused = !paused;
        if (paused)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        cubes_should_rotate = !cubes_should_rotate;
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        use_texture = !use_texture;
    }
    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        player_camera.sens = fmax((player_camera.sens - 0.01), 0.01);
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        player_camera.sens = fmin((player_camera.sens + 0.01), 0.75);
    }
    if (key == GLFW_KEY_F5 && action == GLFW_PRESS){
        worldgenRemake(&world);
    }
    // DEBUG
    if (key == GLFW_KEY_BACKSLASH && action == GLFW_PRESS) {
        fprintf(stderr, "debug info:\n");
        fprintf(stderr, "cam props: ");
        fprintf(stderr, "pos: %.2f %.2f %.2f | ", player_camera.pos.x, player_camera.pos.y, player_camera.pos.z);
        fprintf(stderr, "right: %.2f %.2f %.2f | ", player_camera.right.x, player_camera.right.y, player_camera.right.z);
        fprintf(stderr, "left: %.2f %.2f %.2f | ", player_camera.left.x, player_camera.left.y, player_camera.left.z);
        fprintf(stderr, "up: %.2f %.2f %.2f | ", player_camera.up.x, player_camera.up.y, player_camera.up.z);
        fprintf(stderr, "at: %.2f %.2f %.2f | ", player_camera.at.x, player_camera.at.y, player_camera.at.z);
        fprintf(stderr, "dir: %.2f %.2f %.2f\n", player_camera.dir.x, player_camera.dir.y, player_camera.dir.z);
        fprintf(stderr, "speed/sens: %.2f %.2f\n", player_camera.speedCurrent, player_camera.sens);
        fprintf(stderr, "chunks/blocks: %d %d\n", hmlen(world.chunkMap), hmlen(world.chunkMap) * WORLDGEN_CHUNK_SIZE);
        fprintf(stderr, "frame_time_ms: %u\n", frame_time_ms);
    }
}

void
mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
{    
    if (paused || cursor_first_time) {
        cursor_last_x = x_pos;
        cursor_last_y = y_pos;
        cursor_first_time = false;
        return;
    }
    float x_offset = (float)x_pos - cursor_last_x;
    float y_offset = cursor_last_y - (float)y_pos;
    cursor_last_x = (float)x_pos;
    cursor_last_y = (float)y_pos;
    /**********************************************************************
     * NOTE: Guard against glfw's automatic re-centering of the cursor
     ******************************************************************** */
    float x_scr_c = scr_width / 2.0;
    float y_scr_c = scr_height / 2.0;
    float x_diff = abs(x_pos - x_scr_c);
    float y_diff = abs(y_pos - y_scr_c);
    if (x_diff < 1.0 && y_diff < 1.0)
        return;
    if (abs(x_offset) > scr_width / 2.0 || abs(y_offset) > scr_height / 2.0)
        return;
    /**********************************************************************/
    cam_update_dir(&player_camera, x_offset, y_offset);
//    cam_yaw   += x_offset * cursor_sens;
//    cam_pitch += y_offset * cursor_sens;
//    if (cam_pitch >  89.0)
//        cam_pitch =  89.0;
//    if (cam_pitch < -89.0)
//        cam_pitch = -89.0;
//
//    cam_dir[0] = cos(glm_rad(cam_yaw) * cos(glm_rad(cam_pitch)));
//    cam_dir[1] = sin(glm_rad(cam_pitch));
//    cam_dir[2] = sin(glm_rad(cam_yaw) * cos(glm_rad(cam_pitch)));
//    glm_vec3_normalize(cam_dir);
}

void
scroll_callback(GLFWwindow *window, double x_offset, double y_offset){
    cam_update_zoom(&player_camera, x_offset, y_offset);
}

void
process_input(GLFWwindow *window)
{
    if (paused)
        return;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam_update_move(&player_camera, DIR_LEFT, 1);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam_update_move(&player_camera, DIR_RIGHT, 1);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam_update_move(&player_camera, DIR_FORWARD, 1);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam_update_move(&player_camera, DIR_BACKWARD, 1);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        player_camera.speedCurrent = player_camera.speedSprint;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        player_camera.speedCurrent = player_camera.speedRegular;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        g_light.pos[0] += 1.0;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        g_light.pos[0] -= 1.0;
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
        g_light.pos[1] += 1.0;
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
        g_light.pos[1] -= 1.0;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        g_light.pos[2] += 1.0;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        g_light.pos[2] -= 1.0;
}

GLFWwindow*
startup(int width, int height, int gl_ver_maj, int gl_ver_min)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = 
        glfwCreateWindow(INIT_SCR_WIDTH, INIT_SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return NULL;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    return window;
}

i32
renderChunks(u32 shaderProgram)
{
    point2d myChunk = {
        player_camera.pos.x / WORLDGEN_CHUNK_LEN, 
        player_camera.pos.z / WORLDGEN_CHUNK_LEN};

    glUseProgram(shaderProgram);

    u32 modlMatLoc = glGetUniformLocation(shaderProgram, "modl");
    u32 underWaterLoc = glGetUniformLocation(shaderProgram, "under_water");

    int chunksRendered = 0;
    int chunkSquareRad = player_camera.draw_dist / WORLDGEN_CHUNK_LEN + 1;
    for (int i = myChunk.z - chunkSquareRad; i < myChunk.z + chunkSquareRad; ++i) {
        for (int j = myChunk.x - chunkSquareRad; j < myChunk.x + chunkSquareRad; ++j) {
            bool skip = true;
            // HACK NEAR PLAYER / CAMERA
            if (abs(myChunk.z - i) < 3 && abs(myChunk.x - j) < 3)
                skip = false;

            vec2 chunkposvecs[] = {
                { j * WORLDGEN_CHUNK_LEN,       i * WORLDGEN_CHUNK_LEN},
                {(j + 1) * WORLDGEN_CHUNK_LEN,  i * WORLDGEN_CHUNK_LEN},
                { j * WORLDGEN_CHUNK_LEN,      (i + 1) * WORLDGEN_CHUNK_LEN},
                {(j + 1) * WORLDGEN_CHUNK_LEN, (i + 1) * WORLDGEN_CHUNK_LEN},
            };
            for (int ic = 0; ic < 4; ++ic) {
                vec2 camdirvec = {player_camera.dir.x, player_camera.dir.z};
                vec2 camposvec = {player_camera.pos.x, player_camera.pos.z};
                vec2 chunkposvec = {chunkposvecs[ic][0], chunkposvecs[ic][1]};

                vec2 chunkdirvec;
                glm_vec2_sub(camposvec, chunkposvec, chunkdirvec);
                glm_vec2_normalize(camdirvec);
                glm_vec2_normalize(chunkdirvec);
                float dotproduct = glm_vec2_dot(camdirvec, chunkdirvec);
                float dotangle = glm_deg(acos(dotproduct));
                dotangle = 180.0 - dotangle;
                if (dotangle < player_camera.zoom)
                    skip = false;
            }
            if (skip)
                continue;
            ++chunksRendered;
            
            point2d chunkPos = {j, i};
            if (hmget(world.chunkMap, chunkPos) == NULL) {
                WorldGenChunk *chunk = calloc(1, sizeof(WorldGenChunk));
                worldgenChunk(&world, chunk, chunkPos.x, chunkPos.z);
                hmput(world.chunkMap, chunkPos, chunk);
            }

            WorldGenChunk *currentChunk = hmget(world.chunkMap, chunkPos);
//            printf("rendering chunk %d %d\n", chunkPos.x, chunkPos.z);
            for (int ib = 0; ib < WORLDGEN_CHUNK_LEN; ++ib) {
                for (int jb = 0; jb < WORLDGEN_CHUNK_LEN; ++jb) {
                    block b = currentChunk->blocks[ib][jb];
                    mat4 modl = GLM_MAT4_IDENTITY_INIT;
                    vec3 blockPos = {
                        b.pos[0],
                        b.pos[1],
                        b.pos[2],
                    };
                    glm_translate(modl, blockPos);
                    glUniformMatrix4fv(modlMatLoc, 1, GL_FALSE, (float*)modl);

                    if (blockPos[1] < 0.0)
                        glUniform1i(underWaterLoc, true);
                    else
                        glUniform1i(underWaterLoc, false);


                    glBindVertexArray(_blockVAOs[b.type]);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, _blockTextureIDs[b.type]);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }

        }
    }

    return chunksRendered;
}

/* MAIN ENTRANCE */

#define unvec3(a) {(a)[0], (a)[1], (a)[2]}

int
main()
{
    /* INIT */
    srand(time(NULL));
    GLFWwindow *window = 
        startup(INIT_SCR_WIDTH, INIT_SCR_HEIGHT, INIT_GL_VER_MAJ, INIT_GL_VER_MIN);

    shader_utils_set_shader_dir_vertex("shaders/vertex");
    shader_utils_set_shader_dir_fragment("shaders/fragment");

    hmdefault(world.chunkMap, NULL);

    /* DATA */
    vec4 bg_color = { 0.0, 0.0, 0.0, 1.0 };

    u32 blockVAOs[BT__NUM];
    blocksInit(blockVAOs);

    worldgenRemake(&world);

    /* SHADERS */
    u32 shader_program = shaderutilsMakeProgram("material.vert", "material.frag");
    u32 shader_program_source = shaderutilsMakeProgram("material.vert", "lighting_source.frag");
    u32 shader_program_biomes = shaderutilsMakeProgram("material.vert", "biomes.frag");
    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "Texture"), 0);
    glUseProgram(0);

    for (int i = 0; i < BT__NUM; ++i){
        _blockShaders[i] = shader_program;
    }
    /* TIME */
    u64 updates = 0;

    struct timespec time_spec;
    clock_gettime(CLOCK_REALTIME, &time_spec);
    nsec_start = time_spec.tv_sec * 1000000000 + time_spec.tv_nsec;
    u64 microsec_start = nsec_start / 1000;
    u64 microsec_lastupdate = 0;
    u64 microsec_lastrender = 0;
    u64 microsec_delta = 1000000;
    u64 microsec_now = microsec_start;
    u64 microsec_update_time = 1000000 / UPS;

    /* MATH */
    mat4 rot_mat = GLM_MAT4_IDENTITY_INIT;
    float angle, rots_per_sec = 4.0;
    float ups_to_radians = (2.0 * M_PI) / (rots_per_sec * (float)UPS);

    /* MAIN LOOP */
    while (!(glfwWindowShouldClose(window))) {
        /* TIME */
        clock_gettime(CLOCK_REALTIME, &time_spec);
        u64 nsec_now = time_spec.tv_sec * 1000000000 + time_spec.tv_nsec;
        microsec_now = nsec_now / 1000;
        microsec_delta = microsec_now - microsec_lastupdate;
        frame_time_ms = microsec_delta / 1000;
        if (microsec_delta < microsec_update_time){
            usleep(microsec_update_time - microsec_delta);
            microsec_now += microsec_update_time - microsec_delta;
        }
        microsec_lastupdate = microsec_now;
        /* INPUT */
        process_input(window);
        /* EVENTS */
        glfwPollEvents();
        /* UPDATE */
        if (!paused)
            ++updates;
        else {
            continue;
        }

        /* RENDER */
        glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // DRAWING CUBES
        //glBindVertexArray(VAO);
        glBindVertexArray(blockVAOs[0]);
        glUseProgram(shader_program);

        // LIGHTING
        int camPosLoc = glGetUniformLocation(shader_program, "camera.pos");
        int lightPosLoc = glGetUniformLocation(shader_program, "light.pos");
        int lightAmbientLoc = glGetUniformLocation(shader_program, "light.ambient");
        int lightDiffuseLoc = glGetUniformLocation(shader_program, "light.diffuse");
        int lightSpecularLoc = glGetUniformLocation(shader_program, "light.specular");

        int matTypeLoc = glGetUniformLocation(shader_program, "material.type");
        int matShineLoc = glGetUniformLocation(shader_program, "material.shine");
        int matAmbientLoc = glGetUniformLocation(shader_program, "material.ambient");
        int matDiffuseLoc = glGetUniformLocation(shader_program, "material.diffuse");
        int matSpecularLoc = glGetUniformLocation(shader_program, "material.specular");

        glUniform3fv(camPosLoc, 1, (float*)player_camera.pos.vec);

        glUniform3fv(lightPosLoc, 1, (float*)g_light.pos);
        glUniform3fv(lightAmbientLoc, 1, (float*)g_light.ambient);
        glUniform3fv(lightDiffuseLoc, 1, (float*)g_light.diffuse);
        glUniform3fv(lightSpecularLoc, 1, (float*)g_light.specular);

        glUniform1f(matShineLoc, g_material.shine);
        glUniform1i(matTypeLoc,  g_material.type);
        glUniform3fv(matAmbientLoc, 1, (float*)g_material.ambient);
        glUniform3fv(matDiffuseLoc, 1, (float*)g_material.diffuse);
        glUniform3fv(matSpecularLoc, 1, (float*)g_material.specular);

        int useTextureLoc = glGetUniformLocation(shader_program, "use_texture");
        int underWaterLoc = glGetUniformLocation(shader_program, "under_water");
        glUniform1i(useTextureLoc, use_texture);

        mat4 view = GLM_MAT4_IDENTITY_INIT;
        mat4 proj = GLM_MAT4_IDENTITY_INIT;

        cam_get_view(&player_camera, &view);
        glm_perspective(
                glm_rad(player_camera.zoom), 
                (float)scr_width / (float)scr_height, 0.1, 
                player_camera.draw_dist * 1.2, 
                proj);

        int view_loc = glGetUniformLocation(shader_program, "view");
        int proj_loc = glGetUniformLocation(shader_program, "proj");
        int modl_loc = glGetUniformLocation(shader_program, "modl");

        glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, (float*)proj);

        double secs = glfwGetTime() / 05.0;
        /* **************************** */
        /* RENDER THE WHOLE BLOCK WORLD */
        int chunksRendered = renderChunks(shader_program);
        /* **************************** */
        glUseProgram(shader_program_source);
        for (int i = 0; i < 1; ++i){
            int view_loc_ls = glGetUniformLocation(shader_program_source, "view");
            int proj_loc_ls = glGetUniformLocation(shader_program_source, "proj");
            int modl_loc_ls = glGetUniformLocation(shader_program_source, "modl");

            glUniformMatrix4fv(view_loc_ls, 1, GL_FALSE, (float*)view);
            glUniformMatrix4fv(proj_loc_ls, 1, GL_FALSE, (float*)proj);

            mat4 modl = GLM_MAT4_IDENTITY_INIT;
            glm_translate(modl, g_light.pos);
            glUniformMatrix4fv(modl_loc_ls, 1, GL_FALSE, (float*)modl);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // DRAW BIOMES
        glUseProgram(shader_program_biomes);
        for (int i = 0; i < arrlen(world.biomes); ++i){
            //printf("biome at %d %d\n", world.biomes[i].pos.x, world.biomes[i].pos.z);
            int view_loc_ls = glGetUniformLocation(shader_program_biomes, "view");
            int proj_loc_ls = glGetUniformLocation(shader_program_biomes, "proj");
            int modl_loc_ls = glGetUniformLocation(shader_program_biomes, "modl");
            int biomeColor_loc_ls = glGetUniformLocation(shader_program_biomes, "biomeColor");

            vec3 colorvec = {0.99, 0,0};
            if (world.biomes[i].type == BIOME_FOREST) {
                colorvec[0] = 0.33;
                colorvec[1] = 0.12;
                colorvec[2] = 0.02;
            }
            if (world.biomes[i].type == BIOME_MOUNTAINS) {
                colorvec[0] = 0.25;
                colorvec[1] = 0.25;
                colorvec[2] = 0.25;
            }
            
            glUniform3fv(biomeColor_loc_ls, 1, (float*)colorvec);
            glUniformMatrix4fv(view_loc_ls, 1, GL_FALSE, (float*)view);
            glUniformMatrix4fv(proj_loc_ls, 1, GL_FALSE, (float*)proj);

            mat4 modl = GLM_MAT4_IDENTITY_INIT;
            glm_translate(modl, (vec3){world.biomes[i].pos.x, 070.0, world.biomes[i].pos.z});
            glm_scale_uni(modl, 12.0);
            glUniformMatrix4fv(modl_loc_ls, 1, GL_FALSE, (float*)modl);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glBindVertexArray(0);
        // END DRAWING CUBES

        //        /* SWAP BUFFERS */
        //        if (microsec_now - microsec_lastrender >= microsec_update_time){
        glfwSwapBuffers(window);
        //            microsec_lastrender = microsec_now;
        //        }
        if (updates % 60 == 0)
            printf("chunks rendered: %d/%d\n", chunksRendered, hmlen(world.chunkMap));
    }

    glfwTerminate();
}
