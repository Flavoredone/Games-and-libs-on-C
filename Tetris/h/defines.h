#ifndef DEFINES_H
#define DEFINES_H

#define WIN_INIT(time) {\
                             initscr();\
                             noecho();\
                             curs_set(0);\
                             keypad(stdscr, TRUE);\
                             timeout(time);\
                        }

#define GET_USER_INPUT getch()

#define BOARDS_BEGIN 2

#define MVPRINTW(y, x, ...) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
// #define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')

#define ROWS_MAP 14
#define COLS_MAP 10

#define MAP_PADDING 3

#define INITIAL_TIMEOUT  150

#define BOARD_N     (ROWS_MAP + MAP_PADDING * 2)
// #define BOARD_M     20
#define BOARD_M     20
#define HUD_WIDTH   12

// brick start coords
#define START_X      (BOARD_M / 2)
#define START_Y      (0)

#define SUCCESS   0
#define ERROR     1

#define NO_INPUT -1

#define ESCAPE    27
#define ENTER_KEY 10
#define SPACE_KEY 32
#define PAUSE_KEY 112

#endif
