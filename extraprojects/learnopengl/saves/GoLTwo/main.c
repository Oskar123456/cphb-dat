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
#define INIT_GL_VER_MIN 5

static Camera player_camera;
static u64 updates_total;

#include "gol.c"

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
    u32 shader = shaderutilsMakeProgram("simple.vert", "simple.frag");
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
            if (updates_total % 60 == 0) {
                printf("cells: %ld\n", hmlen(cells));
            }
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
        fprintf(stderr, "zoom/speed/sens: %.2f %.2f %.2f\n", player_camera.zoom, player_camera.speed, player_camera.sens);
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
