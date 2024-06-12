/* GAME CALCULATIONS & RENDERING */
#define TETP_ARENA_WIDTH 10
#define TETP_ARENA_HEIGHT 20
#define TETP_PIECE_SIZE 4
#define TETP_PIECE_OPTIONS 7
#define TETP_COLOR_OPTIONS 6
#define TETP_UPDATE_DELAY_DEFAULT 500000
#define TETP_UPDATE_DELAY_STEP 30000
#define TETP_MOVE_DELAY 75000
#define TETP_ANIMATION_FRAMES 45

typedef struct vec2_u8 {
    u8 x;
    u8 y;
} vec2_u8;

typedef struct TETP_REMOVED_ROW {
    u8 row_idx;
    u32 updates_left;
    u8 colors[TETP_ARENA_WIDTH];
} tetp_removed_row;

static u8 TETP_ARENA[TETP_ARENA_HEIGHT][TETP_ARENA_WIDTH];
static u8 TETP_SCORED[TETP_ARENA_HEIGHT];

static vec2_u8 TETP_CURRENT_PIECE[4];
static vec2_u8 *TETP_NEXT_PIECE;
static u8 TETP_CURRENT_PIECE_COLOR;
static u8 TETP_NEXT_PIECE_COLOR;

static u64 TETP_TOTAL_UPDATES = 0;
static int TETP_CURRENT_LEVEL = 0;
static bool TETP_GAME_OVER = false;
static u64 TETP_SCORE;

static float TETP_PADDING = 0.05;
/* timing */
static u64 tetp_lastupdate;
static u64 tetp_lastmove;
static u64 tetp_update_delay;
/* animation */
static tetp_removed_row *tetp_rows_to_animate;

u32 tetp_points_index[] = { 40, 100, 300, 1200 };

vec2_u8 TETP_PIECES[] = {
    /* STRAIGHT */
    { 5, 0 },
    { 5, 1 },
    { 5, 2 },
    { 5, 3 },
    /* SNAKE L */
    { 5, 0 },
    { 5, 1 },
    { 6, 1 },
    { 6, 2 },
    /* SNAKE R */
    { 6, 0 },
    { 6, 1 },
    { 5, 1 },
    { 5, 2 },
    /* L L */
    { 5, 0 },
    { 5, 1 },
    { 5, 2 },
    { 6, 2 },
    /* L R */
    { 6, 0 },
    { 6, 1 },
    { 6, 2 },
    { 5, 2 },
    /* T */
    { 4, 0 },
    { 5, 0 },
    { 6, 0 },
    { 5, 1 },
    /* SQUARE */
    { 5, 0 },
    { 5, 1 },
    { 6, 0 },
    { 6, 1 },
};

Color TETP_COLORS[] = { BLANK, RED, GREEN, BLUE, YELLOW, MAGENTA, PURPLE };

void
tetp_rotate_90(vec2_u8 *vec)
{
    vec2_u8 vec_pivot = vec[TETP_PIECE_SIZE - 1];
    vec2_u8 vecs_temp[TETP_PIECE_SIZE];
    memcpy(vecs_temp, vec, sizeof(vecs_temp));
    for (int i = 0; i < TETP_PIECE_SIZE - 1; ++i) {
        i8 i_x = ((i8)vec[i].x - (i8)vec_pivot.x);
        i8 i_y = ((i8)vec[i].y - (i8)vec_pivot.y);
        vecs_temp[i].x = (u8)(-1 * i_y + vec_pivot.x);
        vecs_temp[i].y = (u8)(i_x + vec_pivot.y);
        if (vecs_temp[i].x < 0 || vecs_temp[i].x >= TETP_ARENA_WIDTH
            || vecs_temp[i].y < 0 || vecs_temp[i].y >= TETP_ARENA_HEIGHT)
            return;
    }
    memcpy(vec, vecs_temp, sizeof(vecs_temp));
}

void
tetp_anchor()
{
    for (int i = 0; i < TETP_PIECE_SIZE; ++i)
        TETP_ARENA[TETP_CURRENT_PIECE[i].y][TETP_CURRENT_PIECE[i].x]
            = TETP_CURRENT_PIECE_COLOR;
}

bool
tetp_check_collision()
{
    for (int i = 0; i < TETP_PIECE_SIZE; ++i) {
        if (TETP_ARENA[TETP_CURRENT_PIECE[i].y][TETP_CURRENT_PIECE[i].x] > 0
            || TETP_CURRENT_PIECE[i].x < 0
            || TETP_CURRENT_PIECE[i].x >= TETP_ARENA_WIDTH
            || TETP_CURRENT_PIECE[i].y >= TETP_ARENA_HEIGHT)
            return true;
    }
    return false;
}

bool
tetp_move(u64 total_t, i8 x_offset, i8 y_offset, bool manual)
{
    if (manual) {
        u64 delta_t = total_t - tetp_lastmove;
        if (delta_t < TETP_MOVE_DELAY)
            return false;
        tetp_lastmove = total_t;
    }
    bool moved = false;
    for (int i = 0; i < TETP_PIECE_SIZE; ++i) {
        TETP_CURRENT_PIECE[i].x += x_offset;
        TETP_CURRENT_PIECE[i].y += y_offset;
    }
    if (tetp_check_collision())
        for (int i = 0; i < TETP_PIECE_SIZE; ++i) {
            TETP_CURRENT_PIECE[i].x -= x_offset;
            TETP_CURRENT_PIECE[i].y -= y_offset;
        }
    else
        moved = true;
    return moved;
}

vec2_u8 *
tetp_choose_piece()
{
    u8 r = rand() % TETP_PIECE_OPTIONS;
    return &TETP_PIECES[r * TETP_PIECE_SIZE];
}

u8
tetp_choose_color()
{
    u8 r = (rand() % TETP_COLOR_OPTIONS) + 1;
    return r;
}

void
tetp_adjust_rows()
{
    for (int j = 0; j < TETP_ARENA_WIDTH; ++j) {
        int bot_idx = TETP_ARENA_HEIGHT - 1;
        for (int i = TETP_ARENA_HEIGHT - 1; i >= 0; --i) {
            if (TETP_ARENA[bot_idx][j] != 0)
                TETP_ARENA[bot_idx][j] = TETP_ARENA[i][j];
            if (TETP_ARENA[i][j] > 0 && i != bot_idx) {
                TETP_ARENA[bot_idx][j] = TETP_ARENA[i][j];
                TETP_ARENA[i][j] = 0;
                --bot_idx;
            }
        }
    }
}

void
tetp_remove_rows()
{
    bool removed = false;
    for (int i = 0; i < TETP_ARENA_HEIGHT; ++i) {
        if (TETP_SCORED[i]) {
            printf("removing row %d\n", i);
            TETP_SCORED[i] = 0;
            tetp_removed_row rem_row;
            rem_row.row_idx = i;
            rem_row.updates_left = TETP_ANIMATION_FRAMES;
            memcpy(&rem_row.colors[0], &TETP_ARENA[i][0], TETP_ARENA_WIDTH);
            arrput(tetp_rows_to_animate, rem_row);
            memset(&TETP_ARENA[i][0], 0, TETP_ARENA_WIDTH);
        }
    }
    tetp_adjust_rows();
}

void
tetp_score()
{
    u8 scored = 0;
    for (int i = 0; i < TETP_ARENA_HEIGHT; ++i) {
        bool full_row = true;
        for (int j = 0; j < TETP_ARENA_WIDTH; ++j) {
            if (TETP_ARENA[i][j] == 0)
                full_row = false;
        }
        if (full_row) {
            ++scored;
            TETP_SCORED[i] = 1;
        }
    }
    if (scored) {
        TETP_SCORE += tetp_points_index[scored - 1] * (1 + TETP_CURRENT_LEVEL);
        printf("score: %d\n", TETP_SCORE);
        tetp_remove_rows();
    }
}

bool
tetp_update(u64 total_t)
{
    ++TETP_TOTAL_UPDATES;

    if (IsKeyDown(KEY_PERIOD))
        TETP_TOTAL_UPDATES += 600;
    if (IsKeyDown(KEY_COMMA))
        TETP_TOTAL_UPDATES -= 600;

    if (IsKeyPressed(KEY_UP))
        tetp_rotate_90(TETP_CURRENT_PIECE);
    if (IsKeyDown(KEY_LEFT))
        tetp_move(total_t, -1, 0, true);
    if (IsKeyDown(KEY_RIGHT))
        tetp_move(total_t, 1, 0, true);
    if (IsKeyDown(KEY_DOWN))
        tetp_move(total_t, 0, 1, true);
    if (IsKeyPressed(KEY_SPACE)) {
        while (tetp_move(total_t, 0, 1, false)) { }
        goto FASTFORWARD;
    }

    u64 delta_t = total_t - tetp_lastupdate;
    // printf("total_t: %llu, lastupdate: %llu\tdelta_t: %llu\n", total_t,
    // tetp_lastupdate, delta_t);
    if (delta_t <= tetp_update_delay || TETP_GAME_OVER)
        return false;
    tetp_lastupdate = total_t;

    TETP_CURRENT_LEVEL = min((TETP_TOTAL_UPDATES / TET_UPS) / 10, 24);
    tetp_update_delay = TETP_UPDATE_DELAY_DEFAULT
        - TETP_CURRENT_LEVEL * TETP_UPDATE_DELAY_STEP;

FASTFORWARD:
    if (!tetp_move(total_t, 0, 1, false)) {
        tetp_anchor();
        memcpy(TETP_CURRENT_PIECE, TETP_NEXT_PIECE,
            TETP_PIECE_SIZE * sizeof(vec2_u8));
        TETP_CURRENT_PIECE_COLOR = TETP_NEXT_PIECE_COLOR;
        if (tetp_check_collision()) {
            TETP_GAME_OVER = true;
            printf("GAME_OVER\n");
        }
        TETP_NEXT_PIECE = tetp_choose_piece();
        TETP_NEXT_PIECE_COLOR = tetp_choose_color();
    }

    tetp_score();

    return true;
}

void
tetp_render_piece(Rectangle rec_arena, vec2_u8 *piece, float square_sz,
    Color color, bool outline)
{
    for (int i = 0; i < TETP_PIECE_SIZE; ++i) {
        float x = rec_arena.x + piece[i].x * square_sz;
        float y = rec_arena.y + piece[i].y * square_sz;
        Rectangle r = { x, y, square_sz, square_sz };
        DrawRectangleRec(r, color);
        if (outline)
            DrawRectangleLinesEx(r, 2, BLACK);
    }
}

void
tetp_render()
{
    float arena_width = 0.33 * (float)SCR_WIDTH;
    float arena_height = 2.0 * arena_width;

    Rectangle rec_arena = {
        arena_width + TETP_PADDING,
        (SCR_HEIGHT - arena_height) / 2.0,
        arena_width - TETP_PADDING,
        arena_height,
    };

    Rectangle rec_level_score = {
        2.0 * arena_width + TETP_PADDING,
        (SCR_HEIGHT - arena_height) / 2.0,
        arena_width - TETP_PADDING,
        arena_height / 3.0 - TETP_PADDING,
    };
    Rectangle rec_next_piece = {
        2.0 * arena_width + TETP_PADDING,
        rec_level_score.height + (SCR_HEIGHT - arena_height) / 2.0
            + 2.0 * TETP_PADDING,
        arena_width - TETP_PADDING,
        arena_height - rec_level_score.height - 2.0 * TETP_PADDING,
    };

    float square_sz = rec_arena.width / (float)TETP_ARENA_WIDTH;
    // printf("tetp_render:");
    for (int i = 0; i < TETP_ARENA_HEIGHT; ++i) {
        for (int j = 0; j < TETP_ARENA_WIDTH; ++j) {
            float x = rec_arena.x + j * square_sz;
            float y = rec_arena.y + i * square_sz;
            Rectangle r = { x, y, square_sz, square_sz };
            DrawRectangleRec(r, TETP_COLORS[TETP_ARENA[i][j]]);
            if (TETP_ARENA[i][j] > 0)
                DrawRectangleLinesEx(r, 2, BLACK);
            else
                DrawRectangleLinesEx(r, 1, LIGHTGRAY);
            // printf("[%f,%f]", x,y);
        }
    }
    /* shadow */
    u8 n_moves = 0;
    Color shadow = LIGHTGRAY;
    shadow.a = 75;
    while (tetp_move(0, 0, 1, false)) {
        ++n_moves;
        // tetp_render_piece(rec_arena, TETP_CURRENT_PIECE, square_sz, shadow,
        // false);
    }
    shadow.a = 160;
    tetp_render_piece(rec_arena, TETP_CURRENT_PIECE, square_sz, shadow, false);
    tetp_move(0, 0, -n_moves, false);
    /* real piece */
    tetp_render_piece(rec_arena, TETP_CURRENT_PIECE, square_sz,
        TETP_COLORS[TETP_CURRENT_PIECE_COLOR], true);

    float x_adjustment = rec_next_piece.width / 10.0;
    float y_adjustment = rec_next_piece.height / 5.0;
    rec_next_piece.x -= x_adjustment;
    rec_next_piece.y += y_adjustment;
    tetp_render_piece(rec_next_piece, TETP_NEXT_PIECE, square_sz,
        TETP_COLORS[TETP_NEXT_PIECE_COLOR], true);
    rec_next_piece.x += x_adjustment;
    rec_next_piece.y -= y_adjustment;
    /* animations */
    for (int i = 0; i < arrlen(tetp_rows_to_animate); ++i) {
        if (tetp_rows_to_animate[i].updates_left-- < 2)
            arrdel(tetp_rows_to_animate, i);
        float x = rec_arena.x;
        float y = rec_arena.y + tetp_rows_to_animate[i].row_idx * square_sz;
        Rectangle r = { x, y, rec_arena.width, square_sz };

        float tm = 255.0 / (float)TETP_ANIMATION_FRAMES;
        float transparency = tm * (float)TETP_ANIMATION_FRAMES
            - tm * (float)tetp_rows_to_animate[i].updates_left;
        Color color = { 242, 64, 236, max(255 - (u32)transparency, 0) };

        DrawRectangleRec(r, color);
    }
    /* UI */
    DrawRectangleLinesEx(rec_arena, 4, BLACK);
    DrawRectangleLinesEx(rec_level_score, 4, BLACK);
    DrawRectangleLinesEx(rec_next_piece, 4, BLACK);

    char str[256];
    sprintf(str, "Level %d", TETP_CURRENT_LEVEL);
    DrawText(str, rec_level_score.x + rec_level_score.width / 6.0,
        rec_level_score.y + rec_level_score.height / 3.0,
        rec_level_score.height / 8.0, GREEN);
    sprintf(str, "Score %d", TETP_SCORE);
    DrawText(str, rec_level_score.x + rec_level_score.width / 6.0,
        rec_level_score.y + 2.0 * TETP_PADDING
            + 2.0 * rec_level_score.height / 3.0,
        rec_level_score.height / 8.0, GREEN);
    sprintf(str, "Next up", TETP_SCORE);
    DrawText(str, rec_next_piece.x + rec_next_piece.width / 6.0,
        rec_next_piece.y + 2.0 * TETP_PADDING
            + 2.0 * rec_next_piece.height / 3.0,
        rec_next_piece.height / 8.0, GREEN);
}

void
tetp_init(u64 total_t)
{
    tetp_lastupdate = total_t;
    tetp_lastmove = total_t;
    TETP_SCORE = 0;
    for (int i = 0; i < TETP_ARENA_HEIGHT; ++i) {
        for (int j = 0; j < TETP_ARENA_WIDTH; ++j) {
            TETP_ARENA[i][j] = 0;
            TETP_NEXT_PIECE = tetp_choose_piece();
            memcpy(TETP_CURRENT_PIECE, TETP_NEXT_PIECE,
                TETP_PIECE_SIZE * sizeof(vec2_u8));
            TETP_NEXT_PIECE = tetp_choose_piece();
            TETP_CURRENT_PIECE_COLOR = tetp_choose_color();
            TETP_NEXT_PIECE_COLOR = tetp_choose_color();
        }
    }
}
