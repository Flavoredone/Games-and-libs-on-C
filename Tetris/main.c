#include "h/main.h"
#include "h/fsm.h"
#include "h/defines.h"
#include "h/objects.h"
#include "h/init_shapes.h"

int main(void)
{
    WIN_INIT(300);
    setlocale(LC_ALL, "");
    print_overlay();
    game_loop();
    endwin();

    srand(time(NULL));

    return SUCCESS;
}

void game_loop(void)
{
    board_t map;
    game_stats_t stats;
    tetromino_t brick;

    ////////////////////////
    state state = START;
    bool break_flag = TRUE;
    int signal = 0;
    ////////////////////////

    init_stats(&stats);

    while (break_flag)
    {
        if (state == EXIT_STATE)
            break_flag = FALSE;

        sigact(get_signal(signal), &state, &stats, &map, &brick);

        if (state == MOVING || state == START || state == GAMEOVER || state == PAUSE)
            signal = GET_USER_INPUT;
    }
}

// (init) //////////////////////////////////////////////////////////////////

void init_tetromino(tetromino_t *brick, game_stats_t *stats) 
{
    brick->x = START_X;
    brick->y = START_Y;
    brick->rotation = 0;

    // turn on or off random
    int random = 1;
    // select 0..7 of type (if random == 1 then pass)
    int value = 0;

    if (random) {
        brick->type = stats->next_shape;
        stats->next_shape = rand() % 7;
    } else {
        brick->type = value;
        stats->next_shape = brick->type;
    }

    for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= 4; j++) {
            brick->shape[i][j] = shape_storage[brick->type][i][j];
        }
    }
}

void init_stats(game_stats_t *stats)
{
    stats->level = 1;
    stats->score = 0;
    stats->won = FALSE;
    stats->best = 1;
    stats->next_shape = 0;
}

void init_map(board_t *map)
{
    for (int i = 1; i <= BOARD_N; i++) {
        for (int j = 1; j <=  BOARD_M; j++) {
            map->ways[i][j] = ' ';
            ///!////
            MVADDCH(i, j, '.');
        } 
    }
    map->fl = FALSE; 

    for (int i = 0; i <= BOARD_M; i++) {
        map->count[i] = ' ';
    }
}

// (backend) ////////////////////////////////////////////////////////////

bool check_attach(tetromino_t *brick, board_t *map)
{
    bool res = FALSE;

    int x = brick->x;
    int y = brick->y;

    if (brick->y >= BOARD_N - 2) res = TRUE;

    for (int i = x + 4; i >= x; i--) {                 // x
        for (int j = y + 4; j >= y; j--) {             // y
            if
            (
                (brick->shape[i - x][j - y] == '#') && \
                (map->ways[j + 1][i] == '*')
            ) {
                res = TRUE;
            }         
        }
    }
    return res;
}

bool check_overflow(board_t *map)
{
    bool res = FALSE;

    for (int i = 1; i <= BOARD_M; i++)
    {
        if (map->ways[2][i] == '*')
            res = TRUE;
    }
    return res;
}

bool check_score_up(board_t *map)
{
    bool res = FALSE;

    for (int i = 0; i <= BOARD_M; i++) {
        map->count[i] = map->ways[BOARD_N][i];
    }

    // char* re = " **                 ";
    // if (strcmp(map->count, re) > 0) res = TRUE;

    //test stuff 
    // char* re = " **                 ";
    // if (strcmp(map->count, re) > 0) {
    //     for (int i = 0; i <= BOARD_M; i++) 
    //         map->count[i] = ' ';
    //     res = TRUE;
    // }

    // //test2 stuff 
    // char* re = " ***                 ";
    // if (strcmp(map->count, re) > 0) {
    //     for (int i = 0; i <= BOARD_M; i++) 
    //         map->count[i] = ' ';
    //     res = TRUE;
    // }

    //true stuff 
    // char* re = " ******************* ";
    char* re = "*********************";
    if (strcmp(map->count, re) == 0) {
        for (int i = 0; i <= BOARD_M; i++) 
            map->count[i] = ' ';
        res = TRUE;
    }

    //true stuff N = 10
    // char* re = "�**********";
    // char* re = " *           ";

    MVPRINTW(BOARD_N + 5, -2, "%s", map->count);
    MVPRINTW(BOARD_N + 9, -2, "%d", strcmp(map->count, re));

    // // char* re = "�**********";
    // if (strcmp(map->count, re) == 0) {
    //     for (int i = 0; i <= BOARD_M; i++) 
    //         map->count[i] = ' ';
    //     res = TRUE;
    // }

    return res;
}

void redraw_when_score_up(board_t *map)
{
    char temp[BOARD_N+2][BOARD_M+2];

    for (int i = 0; i <= BOARD_N; i++) {
        for (int j = 0; j <=  BOARD_M; j++) {
            temp[i][j] = ' ';
        } 
    }

    for (int i = 1; i <= BOARD_N; i++) {
        for (int j = 1; j <=  BOARD_M; j++) {
            temp[i][j] = map->ways[i][j];
        } 
    }

    for (int i = 1; i <= BOARD_N; i++) {
        for (int j = 1; j <=  BOARD_M; j++) {
            MVADDCH(i,j, ' ');
        } 
    }

    for (int i = 1; i <= BOARD_N; i++) {
        for (int j = 1; j <=  BOARD_M; j++) {
            map->ways[i][j] = temp[i-1][j];
            MVADDCH(i,j, temp[i-1][j]);
        } 
    }
}

// (frontend) //////////////////////////////////////////////////////////

void print_brick(tetromino_t *brick, board_t *map, int a)
{
    int x = brick->x;
    int y = brick->y;
    char sprite = '#';

    a == 1 ? sprite = '*' : sprite;

    // brick->type == 0 ? y++ : y;

    for (int i = x; i < x + 4; i++) {                 // x
        for (int j = y; j < y + 4; j++) {             // y
            if (brick->shape[i - x][j - y] == '#') 
            {
                MVADDCH(j, i, sprite);
                // map->ways[j][i] = brick->shape[i - x][j - y];
                map->ways[j][i] = sprite;
            }
        }   
    }
    /// tmp
    // for (int i = 0; i <= BOARD_M; i++) {

        // MVPRINTW(BOARD_N + 7,  5 + i, "%d", (char)map->count[i]);

        // MVPRINTW(BOARD_N + 5,  5 + i, "%c", map->ways[BOARD_M][i]);
        // MVADDCH(BOARD_N + 5,  5 + i, map->ways[BOARD_N][i]);
    // }

    // char re[BOARD_M] = " **                  ^?";
    // MVPRINTW(BOARD_N + 9, -2, "%d", strcmp(map->count, re));

    // MVPRINTW(BOARD_N + 5, -2, "%s", map->ways[BOARD_N-1]);
    // MVPRINTW(BOARD_N + 5, -2, "%s", map->count);
    // MVPRINTW(BOARD_N + 11, -2, "%c", map->fl);
}
////!!!!!!!!!!!!!!////////////////////////////////////////////////////////////

/**
 *
 * @brief Отрисовка статистики
 *
 */
void print_stats(game_stats_t *stats) 
{
    // NEXT in stats (current static)
    int y = 13;
    int x = BOARD_M + 7;

    for (int i = x; i < x + 4; i++) {
        for (int j = y; j < y + 4; j++) {
            MVADDCH(j, i, shape_storage[stats->next_shape][i - x][j - y]); 
        }
    }
    //other
    MVPRINTW(2,  BOARD_M + 11, "%d", stats->best);
    MVPRINTW(5,  BOARD_M + 11, "%d", stats->score);
    MVPRINTW(8,  BOARD_M + 11, "%d", stats->level);

    // MVPRINTW( 2, BOARD_M + 5, "BEST");
    // MVPRINTW( 5, BOARD_M + 5, "SCORE");
    // MVPRINTW( 8, BOARD_M + 5, "LEVEL");
    // MVPRINTW(11, BOARD_M + 5, "NEXT");
}

/**
 *
 * @brief Отрисовка статики на экране ("BEST, SCORE, LEVEL, NEXT") и рамок
 *
 */
void print_overlay(void)
{
    print_rectangle(0, BOARD_N + 1, 0, BOARD_M + 1);
    print_rectangle(0, BOARD_N + 1, BOARD_M + 2, BOARD_M + HUD_WIDTH + 3);

    print_rectangle( 1,  3, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
    print_rectangle( 4,  6, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
    print_rectangle( 7,  9, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
    print_rectangle(10, 20, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);

    MVPRINTW( 2, BOARD_M + 5, "BEST");
    MVPRINTW( 5, BOARD_M + 5, "SCORE");
    MVPRINTW( 8, BOARD_M + 5, "LEVEL");
    MVPRINTW(11, BOARD_M + 5, "NEXT");

    MVPRINTW(BOARD_N / 2,  1, "Tap ENTER!");
    MVPRINTW(2,  BOARD_M + HUD_WIDTH + 5, "Avalible controls:");
    MVPRINTW(6,  BOARD_M + HUD_WIDTH + 5, "ESC - to quit");
    MVPRINTW(8,  BOARD_M + HUD_WIDTH + 5, "P - to pause");
    MVPRINTW(10, BOARD_M + HUD_WIDTH + 5, "LEFT, RIGHT arrows - move left, right");
    MVPRINTW(12, BOARD_M + HUD_WIDTH + 5, "UP arrow - rotate");
    MVPRINTW(14, BOARD_M + HUD_WIDTH + 5, "DOWN arrow - speed up");
    MVPRINTW(16, BOARD_M + HUD_WIDTH + 5, "SPACE - stomp shit");
}

void clean_intro(void)
{
    MVPRINTW(BOARD_N / 2, 1, "          ");
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x)
{
    MVADDCH(top_y, left_x, ACS_ULCORNER);

    int i = left_x + 1;

    for (;i < right_x; i++)
        MVADDCH(top_y, i, ACS_HLINE);
    MVADDCH(top_y, i, ACS_URCORNER);

    for (int i = top_y + 1; i < bottom_y; i++)
    {
        MVADDCH(i, left_x, ACS_VLINE);
        MVADDCH(i, right_x, ACS_VLINE);
    }

    MVADDCH(bottom_y, left_x, ACS_LLCORNER);
    i = left_x + 1;
    for (;i < right_x; i++)
        MVADDCH(bottom_y, i, ACS_HLINE);
    MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void print_gameover(void) 
{
    MVPRINTW(BOARD_N / 2, (BOARD_M - 10) / 2 + 1, "GAME OVER!");
    MVPRINTW(BOARD_N / 2 + 2, (BOARD_M - 18) / 2 + 1, "Press ENTER or ESC");
}

void print_pause(void)
{
    MVPRINTW(24, 1, "PAUSE, PRESS ENTER");
}

void clear_pause(void)
{
    MVPRINTW(24, 1, "                  ");
}