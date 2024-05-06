#ifndef OBJECTS_H
#define OBJECTS_H

#include <ncurses.h>
#include "defines.h"
#include "main.h"

typedef struct
{
    char ways[BOARD_M][BOARD_N];
    char count[BOARD_M+1];
    bool fl;
} board_t; // map

typedef struct
{
    int best;
    int score;
    int level;
    int won;
    int next_shape;
} game_stats_t; // stats

typedef struct {
    int x, y;
    int rotation;
    int type;
    int shape[5][5];
} tetromino_t; //brick

#endif