#ifndef FSM_H
#define FSM_H

#include "main.h"
#include "fsm.h"
#include "defines.h"
#include "objects.h"

typedef enum
{
    START = 0,
    SPAWN,
    MOVING,
    SHIFTING,
    ATTACHING,
    SCOREUP,
    GAMEOVER,
    PAUSE,
    EXIT_STATE
} state;

typedef enum
{
    MOVE_UP = 0,
    MOVE_DOWN,
    MOVE_RIGHT,
    MOVE_LEFT,
    ESCAPE_BTN,
    ENTER_BTN,
    SPACE_BTN,
    P_BTN,
    NOSIG
} signals;

signals get_signal(int user_input);
void sigact(signals sig, state *state, game_stats_t *stats, board_t *map, tetromino_t *brick);

#endif
