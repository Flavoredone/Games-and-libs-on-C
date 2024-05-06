#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "defines.h"
#include "fsm.h"
#include "objects.h"

// (init) ////////////////////////////////////////////////////////////////
void init_map(board_t *map);
void init_stats(game_stats_t *stats);
void init_tetromino(tetromino_t *tetromino, game_stats_t *stats);

// (backend) ////////////////////////////////////////////////////////////////
void game_loop();

bool check_attach(tetromino_t *brick, board_t *map);
bool check_overflow(board_t *map);

bool check_score_up(board_t *map);
void redraw_when_score_up(board_t *map);

// (frontend) ////////////////////////////////////////////////////////////////
void clean_intro(void);

void print_overlay(void);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(game_stats_t *stats);
void print_brick(tetromino_t *brick, board_t *map, int a);

void print_gameover(void);
void print_pause(void);
void clear_pause(void);

///////////////////////////////////////////////////////////////////////////////

#endif
