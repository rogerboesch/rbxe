
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define RBXE_ENGINE
#include <rbxe.h>
#include <rbxe-font.h>

// MARK: -  Chess engine calls

void chess_last_move(int* from_col, int* from_row, int* to_col, int* to_row);
int chess_piece_at(int row, int col);
int chess_is_player_in_check(void);
int chess_is_computer_in_check(void);
int chess_is_mate(void);
int chess_user_move(int from, int dest);
int chess_computer_move(void);
void chess_initialize(void);

// MARK: - Callback for remote handling
void game_handle_remote_request(char* request)  {}

// MARK: - Graphic assets

#define SCREEN_WIDTH    364
#define SCREEN_HEIGHT   364
#define TOPMARGIN       0
#define LEFTMARGIN      16
#define INFO_LINE_1     390
#define INFO_LINE_2     380
#define INFO_RIGHT      300
#define INFO_CENTRE		200
#define INFO_LEFT       16

#define HSPACING SCREEN_WIDTH/8
#define VSPACING SCREEN_WIDTH/8
#define SCALE 2
#define RENDER_SCALE 4/5

#define BUTTON_FOUR 4
#define DEFAULT_TEXT_SMALL_SIZE 10

const pixel_info default_color = {255, 174, 55, 255};       // Amber
const pixel_info highlight_color = {255, 219, 165, 255};    // Cloudberry
const pixel_info lowlight_color = {210, 125, 0, 255};       // Butterscotch
const pixel_info text_color = {255, 255, 255, 255};         // White
const pixel_info transparent = {0, 0, 0, 0};

#define DEFAULT_COLOR           1
#define HIGHLIGHT_COLOR         2
#define LOWLIGHT_COLOR          3
#define TEXT_COLOR              4

#define WHITE 3
#define BLACK 0
#define BLACK_OFFSET 10
#define HORIZ "ABCDEFGH"

#define PRG_NAME "CHESS 0.3 BY ROGER BOESCH"

const int pawn[] = {
    -1,0*SCALE,-10*SCALE,

    -1,2*SCALE,3*SCALE,
    -1,6*SCALE,0*SCALE,
    -1,2*SCALE,-2*SCALE,
    -1,3*SCALE,4*SCALE,
    -1,-3*SCALE,4*SCALE,
    -1,-2*SCALE,-2*SCALE,
    -1,-6*SCALE,0*SCALE,
    -1,-2*SCALE,3*SCALE,

    1
};

const int castle[] = {
    -1,0*SCALE,-10*SCALE,

    -1,2*SCALE,2*SCALE,
    -1,10*SCALE,0*SCALE,
    -1,1*SCALE,-2*SCALE,
    -1,3*SCALE,0*SCALE,
    -1,0*SCALE,2*SCALE,
    -1,-2*SCALE,0*SCALE,
    -1,0*SCALE,2*SCALE,
    -1,2*SCALE,0*SCALE,
    -1,0*SCALE,2*SCALE,
    -1,-2*SCALE,0*SCALE,
    -1,0*SCALE, 2*SCALE,
    -1,2*SCALE,0*SCALE,
    -1,0*SCALE,2*SCALE,
    -1,-3*SCALE,0*SCALE,
    -1,-1*SCALE,-2*SCALE,
    -1,-10*SCALE,0*SCALE,
    -1,-2*SCALE,2*SCALE,

    1
};

const int knight[] = {
    -1,0*SCALE,-10*SCALE,

    -1,2*SCALE,2*SCALE,
    -1,1*SCALE,-2*SCALE,
    -1,11*SCALE,3*SCALE,
    -1,2*SCALE,3*SCALE,
    -1,-1*SCALE,0*SCALE,
    -1,-2*SCALE,4*SCALE,
    -1,-2*SCALE,0*SCALE,
    -1,0*SCALE,-4*SCALE,
    -1,-8*SCALE,4*SCALE,
    -1,-1*SCALE,-2*SCALE,
    -1,-2*SCALE,2*SCALE,

    1
};

const int bishop[] = {
    -1,0*SCALE,-10*SCALE,

    -1,2*SCALE,3*SCALE,
    -1,12*SCALE,-2*SCALE,
    -1,4*SCALE,4*SCALE,
    -1,-4*SCALE,4*SCALE,
    -1,-4*SCALE,-4*SCALE,
    -1,2*SCALE,4*SCALE,
    -1,-10*SCALE,-2*SCALE,
    -1,-2*SCALE,3*SCALE,

    1
};

const int queen[] = {
    -1,0*SCALE,-10*SCALE,

    -1,2*SCALE,3*SCALE,
    -1,10*SCALE,0*SCALE,
    -1,0*SCALE,-2*SCALE,
    -1,10*SCALE,0*SCALE,
    -1,-4*SCALE,2*SCALE,
    -1,4*SCALE,2*SCALE,
    -1,-4*SCALE,2*SCALE,
    -1,4*SCALE,2*SCALE,
    -1,-10*SCALE,0*SCALE,
    -1,0*SCALE,-2*SCALE,
    -1,-10*SCALE,0*SCALE,
    -1,-2*SCALE,3*SCALE,

    1
};

const int king[] = {
    -1,0*SCALE,-10*SCALE,

    -1,2*SCALE,3*SCALE,
    -1,14*SCALE,1*SCALE,
    -1,1*SCALE,-4*SCALE,
    -1,1*SCALE,4*SCALE,
    -1,4*SCALE,1*SCALE,
    -1,-4*SCALE,1*SCALE,
    -1,-1*SCALE,4*SCALE,
    -1,-1*SCALE,-4*SCALE,
    -1,-14*SCALE,1*SCALE,
    -1,-2*SCALE,3*SCALE,

    1
};

// MARK: - Game types used

#define PAWN    1
#define KNIGHT  2
#define BISHOP  3
#define ROOK    4
#define QUEEN   5
#define KING    6

const int* pieces[8] = {castle, knight, bishop, queen, king, bishop, knight, castle};
const int* piece_type[6] = {pawn, knight, bishop, castle, queen, king};

typedef enum _GAME_STATE {
    GAME_INITIALIZE, GAME_START,
    COMPUTER_THINK, COMPUTER_MOVED, COMPUTER_ANIMATE, COMPUTER_ANIMATE_END, COMPUTER_WIN,
    PLAYER_CHOOSE_FROM, PLAYER_CHOOSE_TO, PLAYER_MOVE, PLAYER_ANIMATE, PLAYER_ANIMATE_END, PLAYER_WIN
} GAME_STATE;

const char* states[] = {"initialize", "start", "end",
                        "computer think", "computer moved", "computer animate", "computer animate end",
                        "player choose FROM", "player choose TO", "player move", "player animate", "player animate end"
                        };

// MARK: - Forwards

void update_board(void);
void build_last_user_position(void);
void game_change_state(int to_state);
void game_start(void);

// MARK: - Game vars

#define ANIMATION_TIME 15

GAME_STATE game_state;
int game_colour;
int game_board[8][8] = {};
int game_from_x, game_from_y, game_to_x, game_to_y;
int game_comp_from_x, game_comp_from_y, game_comp_to_x, game_comp_to_y;
int draw_color = DEFAULT_COLOR;

char player_move_str[256];
char player_info[256];
char comp_move_str[256];
char comp_info[256];
int animation_counter = 0;
int animation_time = 0;

// MARK: - platform helpers

void platform_init(char* name, int width, int height, int hz) {
    if (!rbxeStart("Chess", 400, 400, 1, 0)) return;

    rbxeFontInit();

}

void platform_start_remote_input(void) {
}

int platform_input_is_left(void) {
    return rbxeKeyPressed(KEY_LEFT);
}

int platform_input_is_right(void) {
    return rbxeKeyPressed(KEY_RIGHT);
}

int platform_input_is_up(void) {
    return rbxeKeyPressed(KEY_UP);
}

int platform_input_is_down(void) {
    return rbxeKeyPressed(KEY_DOWN);
}

int platform_button_is_pressed(int number) {
    if (number == 1 && rbxeKeyPressed(KEY_1))
        return 1;

    if (number == 2 && rbxeKeyPressed(KEY_2))
        return 1;

    if (number == 3 && rbxeKeyPressed(KEY_3))
        return 1;

    if (number == 4 && rbxeKeyPressed(KEY_4))
        return 1;

    if (number == 4 && rbxeKeyPressed(KEY_SPACE))
        return 1;

    return 0;
}

void platform_draw_line(int x1, int y1, int x2, int y2, int color) {
    pixel_info fg_color;
    
    switch (color) {
        case LOWLIGHT_COLOR:
            fg_color = lowlight_color;
            break;
        case HIGHLIGHT_COLOR:
            fg_color = highlight_color;
            break;
        case TEXT_COLOR:
            fg_color = text_color;
            break;
        default:
            fg_color = default_color;
            break;
    }

    rbxePlotLine(x1, y1, x2, y2, fg_color);
}

void platform_draw_lines(int* points, int count, int color) {
    int index = 2;
    int* offset = points;

    //printf("Draw points: %d (%d)\n", count, color);
    
    if (count < 2)
        return;

    if (count % 2 != 0)
        return;

    int x1 = *offset;
    offset++;
    int y1 = *offset;
    offset++;

    if (count == 2) {
        platform_draw_line(x1, y1, x1, y1, color);
        return;
    }

    while (index < count) {
        int x2 = *offset;
        x2 += x1;
        offset++;
        
        int y2 = *offset;
        y2 += y1;
        offset++;

        // printf(" > %d,%d,%d,%d\n", x1, y1, x2, y2);

        platform_draw_line(x1, y1, x2, y2, color);
        
        x1 = x2;
        y1 = y2;
        
        index += 2;
    }
}

// TODO: Replace with vector font
void platform_msg(char* msg, int x, int y, int size, int color) {
    pixel_info fg_color;
    
    switch (color) {
        case LOWLIGHT_COLOR:
            fg_color = lowlight_color;
            break;
        case HIGHLIGHT_COLOR:
            fg_color = highlight_color;
            break;
        case TEXT_COLOR:
            fg_color = text_color;
            break;
        default:
            fg_color = default_color;
            break;
    }
    rbxeFontDrawString(x, y, msg, fg_color, transparent);
}

void platform_frame_begin(void) {
    rbxeClear(0);
}

void platform_frame_end(void) {
    rbxeRun();
}

// MARK: - drawing helpers

void draw_rect(int row, int col, int color) {
    int x = LEFTMARGIN + col * HSPACING;
    int y = TOPMARGIN + row * VSPACING;

    // Temporary solution
    int points[5*2];
    int points_count = 0;

    points[points_count++] = x;
    points[points_count++] = y;

    points[points_count++] = HSPACING;
    points[points_count++] = 0;

    points[points_count++] = 0;
    points[points_count++] = VSPACING;

    points[points_count++] = -HSPACING;
    points[points_count++] = 0;

    points[points_count++] = 0;
    points[points_count++] = -VSPACING;
    
    platform_draw_lines(&points[0], points_count, color);
}

void draw_marker(int row, int col, int color) {
    int x = LEFTMARGIN + col * HSPACING;
    int y = TOPMARGIN + row * VSPACING;

    // Temporary solution
    int points[4*2];
    int points_count = 0;

    points[points_count++] = x;
    points[points_count++] = y;
    points[points_count++] = HSPACING;
    points[points_count++] = 0;
    points[points_count++] = 0;
    points[points_count++] = -2;
    points[points_count++] = -HSPACING;
    points[points_count++] = 0;

    platform_draw_lines(&points[0], points_count, color);
}

void draw_lines_xy(const int *lines, int x, int y, int color) {
    // Temporary solution
    int points[100];
    int points_count = 0;
    
    // Should remove this magic numbers later, also the diff betweenn blacl/white
    y += 6;
    x += 10;
    
    // Offset fixes
    if (game_colour == BLACK) {
        x += HSPACING/2;
    }
    else {
        x += 6;
    }
    
    points[points_count++] = x;
    points[points_count++] = y;

    for (;;) {
        int f = *lines;

        if (f == 1 || f == 2) {
            platform_draw_lines(&points[0], points_count, color);
            return;
        }

        lines++;
        int xOff = *lines;

        lines++;
        int yOff = *lines;

        lines++;

        points[points_count++] = yOff*RENDER_SCALE;
        points[points_count++] = xOff*RENDER_SCALE;
    }
}

void draw_lines(const int *lines, int row, int col, int color) {
    int x = LEFTMARGIN + col * HSPACING;
    int y = TOPMARGIN + row * VSPACING;

    draw_lines_xy(lines, x, y, color);
}

// MARK: - Board drawing

void draw_piece(const int *piece, int row, int col, int color) {
    draw_lines((int *)piece+game_colour, row, col, color);
}

void draw_piece_xy(const int *piece, int x, int y, int color) {
    draw_lines_xy((int *)piece+game_colour, x, y, color);
}

void draw_board_piece(int row, int col) {
    int index = game_board[row][col];
    if (index == 0) {
        // Empty board cell
        return;
    }
    
    game_colour = WHITE;
    draw_color = DEFAULT_COLOR;
    
    if (index > 9) {
        index -= BLACK_OFFSET;
        game_colour = BLACK;
        draw_color = HIGHLIGHT_COLOR;
    }
    
    draw_piece(piece_type[index-1], row, col, draw_color);
}

void draw_board() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((game_state == PLAYER_ANIMATE || game_state == PLAYER_ANIMATE_END) && game_from_x == col && game_from_y == row) {
                ; // Dont render
            }
            else {
                draw_board_piece(row, col);
            }
        }
    }
}

void update_board() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            game_board[row][col] = chess_piece_at(row, col);
            draw_board_piece(row, col);
        }
    }
}

void draw_grid() {
    for (int row = 0; row < 9; row++) {
        int y = TOPMARGIN + row * VSPACING;
        platform_draw_line(LEFTMARGIN, y, LEFTMARGIN+8*HSPACING, y, LOWLIGHT_COLOR);
    }
    
    for (int col = 0; col < 9; col++) {
        int x = LEFTMARGIN + col * HSPACING;
        platform_draw_line(x, TOPMARGIN, x, TOPMARGIN+8*VSPACING, LOWLIGHT_COLOR);
    }
}

// MARK: - Game state helpers

void choose_from_move() {
    if (platform_input_is_down()) {
        if (game_from_y > 0) {
            game_from_y--;
            build_last_user_position();
        }
    }
    if (platform_input_is_up()) {
        if (game_from_y < 7) {
            game_from_y++;
            build_last_user_position();
        }
    }
    if (platform_input_is_left()) {
        if (game_from_x > 0) {
            game_from_x--;
            build_last_user_position();
        }
    }
    if (platform_input_is_right()) {
        if (game_from_x < 7) {
            game_from_x++;
            build_last_user_position();
        }
    }

    if (platform_button_is_pressed(BUTTON_FOUR)) {
        game_to_x = game_from_x;
        game_to_y = game_from_y;

        game_change_state(PLAYER_CHOOSE_TO);

        build_last_user_position();
    }
}

void choose_to_move() {
    if (platform_input_is_down()) {
        if (game_to_y > 0) {
            game_to_y--;
            build_last_user_position();
        }
    }
    if (platform_input_is_up()) {
        if (game_to_y < 8) {
            game_to_y++;
            build_last_user_position();
        }
    }
    if (platform_input_is_left()) {
        if (game_to_x > 0) {
            game_to_x--;
            build_last_user_position();
        }
    }
    if (platform_input_is_right()) {
        if (game_to_x < 8) {
            game_to_x++;
            build_last_user_position();
        }
    }
    
    if (platform_button_is_pressed(BUTTON_FOUR)) {
        game_change_state(PLAYER_MOVE);

        build_last_user_position();
    }
}

void draw_from_move() {
    draw_rect(game_from_y, game_from_x, HIGHLIGHT_COLOR);
}

void draw_choosen_from_move() {
    draw_marker(game_from_y, game_from_x, HIGHLIGHT_COLOR);
}

void draw_to_move() {
    draw_rect(game_to_y, game_to_x, HIGHLIGHT_COLOR);
}

void draw_computer_move() {
    draw_rect(game_comp_to_y, game_comp_to_x, DEFAULT_COLOR);
}

void wait_for_begin() {
    if (platform_button_is_pressed(BUTTON_FOUR)) {
        game_change_state(GAME_START);
    }
}

// MARK: - User message

void build_last_user_position() {
    sprintf(player_info, "");

    if (game_state == PLAYER_CHOOSE_FROM || game_state == COMPUTER_MOVED) {
        char h1 = HORIZ[game_from_x];

        sprintf(player_move_str, "P: %c%d TO ", h1, 8-game_from_y);
    }
    else {
        char h1 = HORIZ[game_from_x];
        char h2 = HORIZ[game_to_x];

        sprintf(player_move_str, "P: %c%d TO %c%d", h1, 8-game_from_x, h2, 8-game_to_y);
    }
}

void build_last_computer_position() {
    char h1 = HORIZ[game_comp_from_x];
    char h2 = HORIZ[game_comp_to_x];

    sprintf(comp_move_str, "C: %c%d TO %c%d", h1, 8-game_comp_from_y, h2, 8-game_comp_to_y);
}

void display_computer_info() {
    if (strlen(comp_move_str) > 0) {
        platform_msg(comp_move_str, INFO_RIGHT, INFO_LINE_1, DEFAULT_TEXT_SMALL_SIZE, TEXT_COLOR);
    }
    
    if (strlen(comp_info) > 0) {
        platform_msg(comp_info, INFO_RIGHT, INFO_LINE_2, DEFAULT_TEXT_SMALL_SIZE, TEXT_COLOR);
    }
}

void display_user_info() {
    if (strlen(player_move_str) > 0) {
        platform_msg(player_move_str, INFO_LEFT, INFO_LINE_1, DEFAULT_TEXT_SMALL_SIZE, TEXT_COLOR);
    }
    
    if (strlen(player_info) > 0) {
        platform_msg(player_info, INFO_LEFT, INFO_LINE_2, DEFAULT_TEXT_SMALL_SIZE, TEXT_COLOR);
    }
}

void display_msg(char* msg1, char* msg2) {
    platform_msg(msg1, INFO_LEFT, INFO_LINE_1, DEFAULT_TEXT_SMALL_SIZE, TEXT_COLOR);
    platform_msg(msg2, INFO_LEFT, INFO_LINE_2, DEFAULT_TEXT_SMALL_SIZE, TEXT_COLOR);
}

void end_game_screen() {
    if (game_state == COMPUTER_WIN) {
        display_msg("YOU LOST", "GAME ENDED");
    }
    else {
        display_msg("YOU WIN", "GAME ENDED");
    }
    
    if (platform_button_is_pressed(BUTTON_FOUR)) {
        // Restart game
        game_start();
    }
}

// MARK: - Animate figure

int distance_player() {
    int dist = abs(game_to_y - game_from_y) + abs(game_to_x - game_from_x);
    return dist;
}

int distance_computer() {
    int dist = abs(game_comp_to_y - game_comp_from_y) + abs(game_comp_to_x - game_comp_from_x);
    return dist;
}

int lerp(int a, int b, int time, int duration) {
    int r = a+((b-a)*time/duration);
    return r;
}

void animate_piece(int from_row, int from_col, int to_row, int to_col) {
    int index = game_board[from_row][from_col];
    if (index == 0) {
        // Empty board cell
        return;
    }
    
    game_colour = WHITE;
    draw_color = DEFAULT_COLOR;
    
    if (index > 9) {
        index -= BLACK_OFFSET;
        game_colour = BLACK;
        draw_color = HIGHLIGHT_COLOR;
    }

    int x1 = LEFTMARGIN + from_col * HSPACING;
    int y1 = TOPMARGIN  + from_row * VSPACING;
    int x2 = LEFTMARGIN + to_col * HSPACING;
    int y2 = TOPMARGIN  + to_row * VSPACING;
    
    int x = x1;
    int y = y1;
    
    if (x1 != x2) {
        x = lerp(x1, x2, animation_counter, animation_time);
    }
    
    if (y1 != y2) {
        y = lerp(y1, y2, animation_counter, animation_time);
    }
    
    draw_piece_xy(piece_type[index-1], x, y, draw_color);
}

void animate_player() {
    animate_piece(game_from_y, game_from_x, game_to_y, game_to_x);
    
    if (animation_counter >= animation_time) {
        game_change_state(PLAYER_ANIMATE_END);
    }
    else {
        animation_counter++;
    }
}

void animate_computer() {
    animate_piece(game_comp_from_y, game_comp_from_x, game_comp_to_y, game_comp_to_x);

    if (animation_counter >= animation_time) {
        game_change_state(COMPUTER_ANIMATE_END);
    }
    else {
        animation_counter++;
    }
}

// MARK: - Chess

#ifdef FREESTANDING

void computer_move() {
    game_change_state(COMPUTER_THINK);
    chess_computer_move();
    chess_last_move(&game_comp_from_x, &game_comp_from_y, &game_comp_to_x, &game_comp_to_y);
    game_change_state(COMPUTER_MOVED);
}

#else

void* threadFunction(void* args) {
    printf("Start thinking in thread\n");
    if (chess_computer_move() == -2) {
        // Check mate
        game_change_state(PLAYER_WIN);
        return 0;
    }
    
    printf("End of thinking in thread\n");

    chess_last_move(&game_comp_from_x, &game_comp_from_y, &game_comp_to_x, &game_comp_to_y);
    
    game_change_state(COMPUTER_MOVED);

    return 0;
}

void computer_move() {
    pthread_t id;
    int ret;
    
    // creating thread
    ret = pthread_create(&id, NULL, &threadFunction,NULL);
    if (ret == 0) {
        printf("Thinking thread created successfully.\n");

        game_change_state(COMPUTER_THINK);
        sprintf(comp_info, "THINKING");
    }
    else {
        printf("Thread not created.\n");
    }
}

#endif

int user_move() {
    int from = game_from_y*8+game_from_x;
    int to = game_to_y*8+game_to_x;

    int result = chess_user_move(from, to);
    if (result == -2) {
        // No move found anymore, mate?
        game_change_state(COMPUTER_WIN);
        return 0;
    }
    
    return result == 0 ? 1 : 0;
}

// MARK: -Initialisation

void init_vars(void) {
    game_from_x = 0;
    game_from_y = 0;
    game_to_x = 0;
    game_to_y = 0;
    game_comp_from_x = 0;
    game_comp_from_y = 0;
    game_comp_to_x = 0;
    game_comp_to_y = 0;
    draw_color = WHITE;

    sprintf(player_move_str, "");
    sprintf(player_info, "");
    sprintf(comp_move_str, "");
    sprintf(comp_info, "");

    animation_counter = 0;
    animation_time = 0;
}

void init_board() {
    game_board[0][0] = ROOK;
    game_board[0][1] = KNIGHT;
    game_board[0][2] = BISHOP;
    game_board[0][3] = QUEEN;
    game_board[0][4] = KING;
    game_board[0][5] = BISHOP;
    game_board[0][6] = KNIGHT;
    game_board[0][7] = ROOK;
    
    for (int i = 0; i < 8; i++) {
        game_board[1][i] = PAWN;
        game_board[6][i] = BLACK_OFFSET+PAWN;
    }

    game_board[7][0] = BLACK_OFFSET+ROOK;
    game_board[7][1] = BLACK_OFFSET+KNIGHT;
    game_board[7][2] = BLACK_OFFSET+BISHOP;
    game_board[7][3] = BLACK_OFFSET+QUEEN;
    game_board[7][4] = BLACK_OFFSET+KING;
    game_board[7][5] = BLACK_OFFSET+BISHOP;
    game_board[7][6] = BLACK_OFFSET+KNIGHT;
    game_board[7][7] = BLACK_OFFSET+ROOK;
    
    chess_initialize();
    
    game_change_state(GAME_INITIALIZE);

    update_board();
}

// MARK: - Game loop

void game_change_state(int to_state) {
    printf("Change game state from '%s' to '%s'\n", states[game_state], states[to_state]);
    game_state = to_state;
}

int is_game_finished() {
    if (chess_is_mate()) {
        return 1;
    }
    
    if (chess_is_player_in_check()) {
        sprintf(player_info, "CHECK");
        return 0;
    }

    if (chess_is_computer_in_check()) {
        sprintf(comp_info, "CHECK");
        return 0;
    }

    return 0;
}

void game_start(void) {
    init_vars();
    init_board();
}

void game_stop(void) {
}

int game_frame() {
    platform_frame_begin();

    if (rbxeKeyDown(KEY_ESCAPE)) {
        return 0;
    }
    
    draw_grid();
    draw_board();
    
    switch (game_state) {
        case GAME_INITIALIZE:
            wait_for_begin();
            display_msg(PRG_NAME, "PRESS TO START");
            break;
        case GAME_START:
            computer_move();
            break;
        case COMPUTER_THINK:
        	break;
        case COMPUTER_MOVED:
            sprintf(comp_info, "");
            build_last_user_position();
            build_last_computer_position();
            animation_counter = 0;
            animation_time = distance_computer() * ANIMATION_TIME;
            game_change_state(COMPUTER_ANIMATE);
            break;
        case COMPUTER_ANIMATE:
            animate_computer();
            draw_computer_move();
            break;
        case COMPUTER_ANIMATE_END:
            game_from_x = 0; game_from_y = 0;
            game_to_x = 0; game_to_y = 0;

            update_board();
            
            if (!is_game_finished()) {
                game_change_state(PLAYER_CHOOSE_FROM);
            }
            break;
        case PLAYER_CHOOSE_FROM:
            draw_computer_move();
            choose_from_move();
            draw_from_move();
            break;
        case PLAYER_CHOOSE_TO:
            choose_to_move();
            draw_choosen_from_move();
            draw_to_move();
            break;
        case PLAYER_MOVE:
            if (user_move()) {
                animation_counter = 0;
                animation_time = distance_player() * ANIMATION_TIME;
                game_change_state(PLAYER_ANIMATE);
            }
            else {
                if (game_state == PLAYER_MOVE) {
                    sprintf(player_info, "ILLEGAL MOVE");
                    
                    game_from_x = 0; game_from_y = 0;
                    game_to_x = 0; game_to_y = 0;
                    
                    game_change_state(PLAYER_CHOOSE_FROM);
                }
            }
            break;
        case PLAYER_ANIMATE:
            animate_player();
            break;
        case PLAYER_ANIMATE_END:
            update_board();

            if (!is_game_finished()) {
                computer_move();
            }

            break;
        case COMPUTER_WIN:
        case PLAYER_WIN:
            end_game_screen();
            break;
    }

    // Render UI
    display_computer_info();
    display_user_info();

    platform_frame_end();

    return 1;
}

int main() {
    platform_start_remote_input();
    platform_init("chess", SCREEN_WIDTH, SCREEN_HEIGHT, 50);
    
    game_start();

    for (;;) {
//        rbxeClear(0);

        if (!game_frame()) {
            game_stop();
            return 0;
        }
    }
}
