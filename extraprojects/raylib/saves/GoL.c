#include "../include/includes_dot_haitch.h"

/*
 * **********************
 * ======================
 * obh C project template
 * ======================
 * **********************
 */

typedef struct cell {
    i32 x;
    i32 y;
} cell;

typedef struct cellhm {
    cell key;
    bool value;
} cellhm;

Rectangle *
grid_to_recs(
    u32 scr_width, u32 scr_height, u32 view_width, u32 view_height)
{
    Rectangle *recs = NULL;

    float rec_width = (float)scr_width / (float)view_width;
    float rec_height = (float)scr_height / (float)view_height;
    float rec_dim = (rec_width > rec_height) ? rec_height : rec_width;

    int nx = (int) ceil((float)scr_width / (float)rec_dim);
    int ny = (int) ceil((float)scr_height / (float)rec_dim);

    for (int i = 0; i < nx; ++i) {
        Rectangle r = { i * rec_dim, 0, rec_dim, scr_height };
        arrput(recs, r);
    }
    for (int i = 0; i < ny; ++i) {
        Rectangle r = { 0, i * rec_dim, scr_width, rec_dim };
        arrput(recs, r);
    }
    return recs;
}

Rectangle *
cells_to_recs(u32 scr_width, u32 scr_height, i32 view_width,
    i32 view_height, i32 view_x, i32 view_y, cellhm *cells)
{
    Rectangle *recs = NULL;

    float rec_width = (float)scr_width / (float)view_width;
    float rec_height = (float)scr_height / (float)view_height;

    float rec_dim = (rec_width > rec_height) ? rec_height : rec_width;

    int nx = (int) ceil((float)scr_width / (float)rec_dim);
    int ny = (int) ceil((float)scr_height / (float)rec_dim);

    u32 cells_len = hmlenu(cells);
    for (int i = 0; i < cells_len; ++i) {
        cell c = cells[i].key;
        // printf("(%d,%d) vs (%d,%d)", c.x, c.y, view_x, view_y);
        if (c.x >= view_x && c.x <= view_x + nx && c.y >= view_y
            && c.y <= view_y + ny) {
            Rectangle r = { ((float)c.x - (float)view_x) * rec_dim,
                ((float)c.y - (float)view_y) * rec_dim, rec_dim,
                rec_dim };
            arrput(recs, r);
            // printf("added rec %f %f %f %f\n", r.x, r.y, r.width,
            // r.height);
        }
    }
    // printf("\n");

    return recs;
}

int
cells_nneighbors(cellhm *cells, cell c)
{
    int n = 0, i, j;
    for (i = -1; i < 2; ++i) {
        for (j = -1; j < 2; ++j) {
            if (i == 0 && j == 0)
                continue;
            cell tmp = { .x = j + c.x, .y = i + c.y };
            if (hmgeti(cells, tmp) >= 0)
                ++n;
        }
    }
    return n;
}

cellhm *
cells_getneighbors(cell c)
{
    cellhm *cells_result = NULL;
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (i == 0 && j == 0)
                continue;
            cell tmp = { .x = j + c.x, .y = i + c.y };
            hmput(cells_result, tmp, true);
        }
    }
    return cells_result;
}

cellhm *
cells_gol(cellhm *cells)
{
    cellhm *cells_result = NULL;
    for (int i = 0; i < hmlen(cells); ++i) {
        cell c = cells[i].key;
        cellhm *c_square = cells_getneighbors(c);
        int n = cells_nneighbors(cells, c);
        if (n > 1 && n < 4)
            hmput(cells_result, c, true);
        for (int i = 0; i < hmlen(c_square); ++i) {
            int n = cells_nneighbors(cells, c_square[i].key);
            if (n == 3)
                hmput(cells_result, c_square[i].key, true);
        }
        hmfree(c_square);
    }
    return cells_result;
}

cell
cell_from_mouse_pos(int mouse_x, int mouse_y, int view_x, int view_y,
    int scr_width, int scr_height, int view_width, int view_height)
{
    float recs_width = (float)scr_width / (float)view_width;
    float recs_height = (float)scr_height / (float)view_height;

    float rec_width_height
        = (recs_width < recs_height) ? recs_width : recs_height;

    float x = (float)mouse_x / rec_width_height;
    float y = (float)mouse_y / rec_width_height;

    cell c
        = { .x = (int)floor(x) + view_x, .y = (int)floor(y) + view_y };
    return c;
}

int
main(int argc, char *argv[])
{
    u32 scr_width = 800;
    u32 scr_height = 600;
    u32 view_width = 40;
    u32 view_height = 30;
    i32 view_x = 0;
    i32 view_y = 0;

    u64 loop_count = 0;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    u64 usec_lastupdate = tv.tv_sec * 1000000 + tv.tv_usec;
    u64 usec_start = tv.tv_sec * 1000000 + tv.tv_usec;

    Vector2 touchPosition = { 0, 0 };
    int currentGesture = GESTURE_NONE;

    cellhm *cells = NULL;

    Rectangle *recs = cells_to_recs(scr_width, scr_height, view_width,
        view_height, view_x, view_y, cells);
    Rectangle *grid
        = grid_to_recs(scr_width, scr_height, view_width, view_height);

    bool paused = false, enable_grid = true;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(scr_width, scr_height, "Raylib basic window");
    SetTargetFPS(120);
    while (!WindowShouldClose()) {
        /* WINDOW SIZE */
        if (IsWindowResized()) {
            scr_width = GetScreenWidth();
            scr_height = GetScreenHeight();
        }
        /* TIME */
        gettimeofday(&tv, NULL);
        u64 usec_now = tv.tv_sec * 1000000 + tv.tv_usec;
        u64 usec_delta = usec_now - usec_lastupdate;
        /* INPUT */
        if (IsKeyPressed(KEY_SPACE))
            paused = !paused;
        if (IsKeyPressed(KEY_G))
            enable_grid = !enable_grid;

        if (IsKeyDown(KEY_LEFT))
            --view_x;
        if (IsKeyDown(KEY_RIGHT))
            ++view_x;
        if (IsKeyDown(KEY_UP))
            --view_y;
        if (IsKeyDown(KEY_DOWN))
            ++view_y;

        i32 mouse_wheel_moved = (i32)GetMouseWheelMove();
        view_width -= 8 * mouse_wheel_moved;
        view_height -= 6 * mouse_wheel_moved;
        view_x += 4 * mouse_wheel_moved;
        view_y += 3 * mouse_wheel_moved;
        /* INTERACTION */
        currentGesture = GetGestureDetected();
        touchPosition = GetTouchPosition(0);
        if (currentGesture == GESTURE_TAP
            || currentGesture == GESTURE_DOUBLETAP) {
            int mouse_x = GetMouseX();
            int mouse_y = GetMouseY();
            // printf("%d at %d,%d\n", currentGesture, mouse_x, mouse_y);
            // printf("paused: %d\t%d,%d / %d,%d\n", paused, view_x,
            //     view_y, hmlen(cells), arrlen(recs));
            cell c = cell_from_mouse_pos(mouse_x, mouse_y, view_x,
                view_y, scr_width, scr_height, view_width, view_height);
            hmput(cells, c, true);
        }
        /* DRAW */
        BeginDrawing();
        ClearBackground(RAYWHITE);
        arrfree(recs);
        arrfree(grid);
        recs = cells_to_recs(scr_width, scr_height, view_width,
            view_height, view_x, view_y, cells);
        grid = grid_to_recs(
            scr_width, scr_height, view_width, view_height);
        for (int i = 0; i < arrlen(recs); ++i)
            DrawRectangleRec(recs[i], BLACK);
        if (enable_grid)
            for (int i = 0; i < arrlen(grid); ++i)
                DrawRectangleLinesEx(grid[i], 0.5, GRAY);
        // DrawFPS(10, 10);
        EndDrawing();
        /* LOGIC */
        if (!paused && usec_delta > 1000 * 50) {
            cellhm *cells_prev = cells;
            cells = cells_gol(cells_prev);
            hmfree(cells_prev);
            usec_lastupdate = usec_now;
        }
    }
    CloseWindow();

    return EXIT_SUCCESS;
}
