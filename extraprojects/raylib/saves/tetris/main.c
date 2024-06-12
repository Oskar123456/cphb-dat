#include "../include/includes_dot_haitch.h"

/*
 * **********************
 * ======================
 * obh C project template
 * ======================
 * **********************
 */
#define TET_UPS 60
#define TETRIS_INIT_SCR_WIDTH 800
#define TETRIS_INIT_SCR_HEIGHT 600

enum TETRIS_GAME_STATE {
    TETRIS_MENU_MAIN,
    TETRIS_PLAY,
    TETRIS_QUIT,
};

u32 SCR_WIDTH = 800;
u32 SCR_HEIGHT = 600;

int CURRENT_GAME_STATE = TETRIS_MENU_MAIN;
bool CURRENT_GAME_PAUSE = false;

u32 TETRIS_GAME_UPDATE_FREQ = 1000 * 100;

int CURRENT_GESTURE = GESTURE_NONE;
Vector2 CURRENT_GESTURE_POS = { 0, 0 };
Vector2 CURRENT_MOUSE_POS = { 0, 0 };

#include "menus.c"
#include "play.c"

void
tetris_poll_input()
{
    CURRENT_GESTURE = GetGestureDetected();
    CURRENT_MOUSE_POS = GetMousePosition();
}

bool
tetris_update(u64 total_t)
{
    switch (CURRENT_GAME_STATE) {
    case TETRIS_MENU_MAIN:
        return tetris_update_menu(TETRIS_BUTTONS_MAIN,
            sizeof(TETRIS_BUTTONS_MAIN) / sizeof(TetrisButton));
        break;
    case TETRIS_PLAY: 
        return tetp_update(total_t);
        break;
    default: break;
    }
    return false;
}

void
tetris_render()
{
    switch (CURRENT_GAME_STATE) {
    case TETRIS_MENU_MAIN:
        tetris_render_menu(TETRIS_BUTTONS_MAIN,
            sizeof(TETRIS_BUTTONS_MAIN) / sizeof(TetrisButton));
        break;
    case TETRIS_PLAY: 
            tetp_render();
            break;
    default: break;
    }
}

int
main(int argc, char *argv[])
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    u64 usec_start = tv.tv_sec * 1000000 + tv.tv_usec;

    srand(time(NULL));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(SCR_WIDTH, SCR_HEIGHT, "Raylib basic window");
    SetTargetFPS(TET_UPS);

    tetp_init(usec_start);

    while (
        !WindowShouldClose() && CURRENT_GAME_STATE != TETRIS_QUIT) {
        /* WINDOW SIZE */
        if (IsWindowResized()) {
            SCR_WIDTH = GetScreenWidth();
            SCR_HEIGHT = GetScreenHeight();
        }
        /* TIME */
        gettimeofday(&tv, NULL);
        u64 usec_now = tv.tv_sec * 1000000 + tv.tv_usec;
        u64 usec_total = usec_now - usec_start;
        /* INPUT */
        tetris_poll_input();
        /* LOGIC */
        bool updated = tetris_update(usec_now);
        /* DRAW */
        BeginDrawing();
        ClearBackground(RAYWHITE);

        tetris_render();

        DrawFPS(10, 10);
        EndDrawing();

        // NOTE: debug
        // usleep(1000 * 500);
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
