#include "../include/includes_dot_haitch.h"
#include "../include/obh_defs.h"
#include "../include/shader_utils.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"
#endif

#ifndef STB_DS_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION
#include "../external/stb/stb_ds.h"
#endif

/*
 * **********************
 * ======================
 * obh C project template
 * ======================
 * **********************
 */

void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow *window, double x_offset, double y_offset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#include "timing.c"
#include "win.c"
#include "camera.c"

/* DECLARATIONS */
#define INIT_SCR_WIDTH 800
#define INIT_SCR_HEIGHT 600
#define INIT_GL_VER_MAJ 4
#define INIT_GL_VER_MIN 6

static Camera player_camera;
static u64 updates_total;

static float cell_vertex_data[] = {
    -0.5f, -0.5f, 0.0f, // left  
    -0.5f,  0.5f, 0.0f, // right 
     0.5f,  0.5f, 0.0f, // top   

    -0.5f, -0.5f, 0.0f, // left  
     0.5f, -0.5f, 0.0f, // right 
     0.5f,  0.5f, 0.0f  // top   
};

typedef struct {
    union {
    // for hashmap / hashset
        ivec2s key;
        ivec2s value;
        ivec2s pos;
        struct {int x, y;};
    };
} Cell;

Cell*
golUpdate(Cell **cells)
{
    Cell *next_gen = NULL;
    int x, y, neighbor_x, neighbor_y;
    for (int i = 0; i < hmlen(*cells); ++i) {
        int center_cell_neighbors = 0;
        ivec2s center_cell_pos = (*cells)[i].pos;
        for (x = center_cell_pos.x - 1; x <= center_cell_pos.x + 1; ++x) {
            for (y = center_cell_pos.y - 1; y <= center_cell_pos.y + 1; ++y) {
                if (x == center_cell_pos.x && y == center_cell_pos.y)
                    continue;
                int neighbor_cell_neighbors = 0;
                ivec2s neighbor_cell_pos = {x, y};
                for (neighbor_x = neighbor_cell_pos.x - 1; neighbor_x <= neighbor_cell_pos.x + 1; ++neighbor_x) {
                    for (neighbor_y = neighbor_cell_pos.y - 1; neighbor_y <= neighbor_cell_pos.y + 1; ++neighbor_y) {
                        if (neighbor_x == neighbor_cell_pos.x && neighbor_y == neighbor_cell_pos.y)
                            continue;
                        ivec2s neighbor_n_cell_pos = {neighbor_x, neighbor_y};
                        if (hmgeti((*cells), neighbor_n_cell_pos) >= 0)
                            ++neighbor_cell_neighbors;
                    }
                }
                if (neighbor_cell_neighbors == 3)
                    hmput(next_gen, neighbor_cell_pos, neighbor_cell_pos);
                if (hmgeti((*cells), neighbor_cell_pos) >= 0)
                    ++center_cell_neighbors;
            }
        }
        if (center_cell_neighbors >= 2 && center_cell_neighbors <= 3)
            hmput(next_gen, center_cell_pos, center_cell_pos);
    }
    hmfree((*cells));
    return next_gen;
}

void
golRender(Cell *cells, vec4s bg_col, vec4s cell_col, u32 vao, u32 shader)
{
    glUseProgram(shader);
    glBindVertexArray(vao);

    u32 shader_uniform_color = glGetUniformLocation(shader, "uniform_color");
    u32 shader_uniform_view = glGetUniformLocation(shader, "view");
    u32 shader_uniform_proj = glGetUniformLocation(shader, "proj");
    u32 shader_uniform_modl = glGetUniformLocation(shader, "modl");

    mat4s view = camGetViews(&player_camera);
    //mat4s proj = glms_ortho(
    //        -(_win_scr_width / 18.0), (_win_scr_width / 18.0), 
    //        -(_win_scr_height / 18.0), (_win_scr_height / 18.0), 
    //        0.01, player_camera.draw_dist);
    mat4s proj = glms_perspective(player_camera.fov, winGetScrAspect(), 0.01, player_camera.draw_dist);

    glUniform4fv(shader_uniform_color, 1, (float*)cell_col.raw);
    glUniformMatrix4fv(shader_uniform_view, 1, GL_FALSE, (float*)view.raw);
    glUniformMatrix4fv(shader_uniform_proj, 1, GL_FALSE, (float*)proj.raw);

    for (int i = 0; i < hmlen(cells); ++i) {
        vec3s cell_pos = {cells[i].x, cells[i].y, 0.0};
        vec4s cell_pos_4 = {cells[i].x, cells[i].y, 0.0, 1.0};
        mat4s modl = glms_translate(mat4_identity(), cell_pos);
        glUniformMatrix4fv(shader_uniform_modl, 1, GL_FALSE, (float*)modl.raw);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

int
main()
{
    /* INIT */
    srand(time(NULL));
    GLFWwindow *window = winInit(INIT_SCR_WIDTH, INIT_SCR_HEIGHT,
        INIT_GL_VER_MAJ, INIT_GL_VER_MIN);

    shaderutilsSetPathVert("shaders/vertex");
    shaderutilsSetPathFrag("shaders/fragment");

    camInit(&player_camera);

    /* TEST */

    /* DATA */
    vec4s bg_color   = { 0.0, 0.0, 0.0, 1.0 };
    vec4s cell_color = { 0.9, 0.9, 0.9, 1.0 };

    Cell *cells = NULL;
    for (int i = 0; i < 15; ++i) {
        ivec2s c = {rand() % 5, rand() % 5};
        hmput(cells, c, c);
    }

    /* SHADERS */
    u32 shader = shaderutilsMakeProgram("simple2d.vert", "simple2d.frag");
    /* OPENGL */
    u32 vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cell_vertex_data), cell_vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0); 
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    glUseProgram(shader);
    glUseProgram(0);

    glClearColor(bg_color.x, bg_color.y, bg_color.z, bg_color.w);
    // set uniforms

    /* TIME */
    TimingTimer start_timer;
    timingStartTimer(&start_timer);

    /* MATH */

    /* MAIN LOOP */
    while (!(glfwWindowShouldClose(window))) {
        /* TIME */
        usleep(16 * 1000);
        timingReadTimer(&start_timer);
        /* INPUT */
        process_input(window);
        /* UPDATE */
        if (!winIsPaused()) {
            ++updates_total;
            if (updates_total % 05 == 0)
                cells = golUpdate(&cells);
        }
        /* RENDER */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        golRender(cells, bg_color, cell_color, vao, shader);

        glfwSwapBuffers(window);
        /* EVENTS */
        glfwPollEvents();
    }

    glfwTerminate();
}

void 
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        _win_paused = !_win_paused;
        if (_win_paused)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    // RESET CAMERA Z POSITION / ZOOM
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {}
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {}
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
        player_camera.speed = player_camera.speed_sprint;
    }
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
        player_camera.speed = player_camera.speed_default;
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
        fprintf(stderr, "zoom/speed/sens: %.2f %.2f\n", player_camera.zoom, player_camera.speed, player_camera.sens);
    }
}

void
mouse_callback(GLFWwindow *window, double x_pos, double y_pos) { }

void
scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
    
}

void
mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double cursor_x, cursor_y;
    glfwGetCursorPos(window, &cursor_x, &cursor_y);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        
    }
}

void
process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player_camera.pos.x += player_camera.speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player_camera.pos.x -= player_camera.speed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player_camera.pos.y += player_camera.speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player_camera.pos.y -= player_camera.speed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        player_camera.pos.z += 01 * player_camera.speed;
        if (player_camera.pos.z > -10)
            player_camera.pos.z = -10.0;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        player_camera.pos.z -= 01 * player_camera.speed;
    }
}
