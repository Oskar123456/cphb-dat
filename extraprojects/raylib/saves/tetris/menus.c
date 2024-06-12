#define TETRIS_BUTTON_WIDTH 200
#define TETRIS_BUTTON_HEIGHT 40
#define TETRIS_BUTTON_PADDING 10

struct TetrisButton {
    Rectangle rec;
    char *text;
    Color border_color;
    Color text_color;
    int game_state_change;
};

typedef struct TetrisButton TetrisButton;

TetrisButton TETRIS_BUTTONS_MAIN[] = {
    { { TETRIS_BUTTON_PADDING,
          TETRIS_BUTTON_HEIGHT + TETRIS_BUTTON_PADDING,
          TETRIS_BUTTON_WIDTH, TETRIS_BUTTON_HEIGHT },
        "PLAY", BLACK, BLACK, TETRIS_PLAY},
    { { TETRIS_BUTTON_PADDING,
          2 * TETRIS_BUTTON_HEIGHT + 2 * TETRIS_BUTTON_PADDING,
          TETRIS_BUTTON_WIDTH, TETRIS_BUTTON_HEIGHT },
        "QUIT", BLACK, BLACK, TETRIS_QUIT},
};

void
tetris_render_button(TetrisButton button)
{
    DrawRectangleLinesEx(button.rec, 2, button.border_color);
    DrawText(button.text, 
             button.rec.x + button.rec.width / 3,
             button.rec.y + button.rec.height / 4, 
             button.rec.height / 2,
             button.text_color);
}

void
tetris_render_menu(TetrisButton *buttons, int len){
    for (int i = 0; i < len; ++i)
        tetris_render_button(buttons[i]);
}

bool
tetris_update_menu(TetrisButton *buttons, int len){
    Rectangle mouse_pos = {CURRENT_MOUSE_POS.x, CURRENT_MOUSE_POS.y, 1, 1};
    for (int i = 0; i < len; ++i){
        if (CheckCollisionRecs(buttons[i].rec, mouse_pos)){
            buttons[i].border_color = RED;
            if (CURRENT_GESTURE == GESTURE_TAP || CURRENT_GESTURE == GESTURE_DOUBLETAP)
                CURRENT_GAME_STATE = buttons[i].game_state_change;
        }
        else 
            buttons[i].border_color = BLACK;
    }
    return true;
}











