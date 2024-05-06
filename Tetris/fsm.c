#include "h/fsm.h"
#include "h/defines.h"

signals get_signal(int user_input)
{
    signals rc = NOSIG;

    if (user_input == KEY_UP)
        rc = MOVE_UP;
    else if (user_input == KEY_DOWN)
        rc = MOVE_DOWN;
    else if (user_input == KEY_LEFT)
        rc = MOVE_LEFT;
    else if (user_input == KEY_RIGHT)
        rc = MOVE_RIGHT;
    else if (user_input == ESCAPE)
        rc = ESCAPE_BTN;
    else if (user_input == ENTER_KEY)
        rc = ENTER_BTN;
    else if (user_input == SPACE_KEY)
        rc = SPACE_BTN;
    else if (user_input == PAUSE_KEY)
        rc = P_BTN;

    return rc;
}

////////////////////////////////////////////////////////////////

void clear_pixel(board_t *map, int x, int y)
{
    MVADDCH(y, x, ' ');
    map->ways[y][x] = ' ';
}

void clear_prevpos(tetromino_t *brick, board_t *map) 
{
    int x = brick->x - 1, y = brick->y - 1, inc_x, inc_y;

    brick->type == 0 ? (inc_x = inc_y = 5) : (inc_x = inc_y = 5);

    for (int i = x; i <= x + inc_x; i++) {
        for (int j = y; j <= y + inc_y; j++) {
            if (map->ways[j][i] == '#')
            clear_pixel(map, i, j);
        }
    }
}

void clear_board(board_t *map)
{
    for (int i = 1; i <= BOARD_N; i++) {
        for (int j = 1; j <= BOARD_M; j++) {
            clear_pixel(map, i, j);
        }
    }  
}
////////////////////////////////////////////////////////////////

void default_down(tetromino_t *brick, board_t *map)
{
    if (!check_attach(brick, map))
    {
        clear_prevpos(brick, map);
        brick->y++;
    }
}

void speed_down(tetromino_t *brick, board_t *map)
{
    if (!check_attach(brick, map) && (brick->y <= BOARD_N - 4))
    {
        clear_prevpos(brick, map);
        brick->y += 2;
    }
}

void moveright(tetromino_t *brick, board_t *map)
{
    if (brick->x < BOARD_M - 2)
    {
        brick->x++;
        clear_prevpos(brick, map);
    }
}

void moveleft(tetromino_t *brick, board_t *map)
{
    if (brick->x > 0)
    {  
        brick->x--;
        clear_prevpos(brick, map);
    }
}

void stomp(tetromino_t *brick, board_t *map)
{
    clear_prevpos(brick, map);
    
    while (!check_attach(brick, map)) {
        brick->y++;
    }
}

void rotate(tetromino_t *brick, board_t *map, int pivotx, int pivoty)
{
    clear_prevpos(brick, map);

    char temp[5][5];

    for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= 4; j++) {
            temp[i][j] = brick->shape[i][j];
        }
    }

    for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= 4; j++) {
            brick->shape[i][j] = temp[3 - j + pivotx - pivoty][i + pivoty - pivotx];
        }
    }

    brick->rotation == 0 ? brick->rotation++ : brick->rotation--; 
}

////////////////////////////////////////////////////////////////

void sigact(signals sig, state *state, game_stats_t *stats, board_t *map, tetromino_t *brick)
{
    int stomp_fl = 0;

    switch (*state)
    {
        case START:
            switch (sig)
            {
                case ENTER_BTN:
                // goto spawn
                    clean_intro();
                    init_map(map);
                    *state = SPAWN;
                    break;
                case ESCAPE_BTN:
                    *state = EXIT_STATE;
                    break;
                // loop before enter
                default:
                    *state = START;
                    break;
            }
            break;


        // start game
        case SPAWN:
            if (stats->level > 1)
                *state = GAMEOVER;
            else
                init_tetromino(brick, stats);
                *state = MOVING;
            break;


        // main in loop
        case MOVING:
            switch (sig)
            {
                case MOVE_DOWN:
                    speed_down(brick, map);
                    break;

                case MOVE_UP:
                    rotate(brick, map, 2, 2);
                    break;

                case MOVE_RIGHT:
                    moveright(brick, map);
                    break;

                case MOVE_LEFT:
                    moveleft(brick, map);
                    break;

                case SPACE_BTN:
                    stomp_fl++; 
                    stomp(brick, map);
                    *state = ATTACHING;
                    break;

                case ESCAPE_BTN:
                    *state = EXIT_STATE;
                    break;
                
                case P_BTN:
                    *state = PAUSE;
                    break;

                default:
                    default_down(brick, map);
                    break;
            } 

            if ((stomp_fl) || (*state == EXIT_STATE) || (*state == PAUSE))
            {
                if (stomp_fl) stomp_fl--;
                break;
            } else {
                if (check_attach(brick, map))
                    *state = ATTACHING;
                else if (check_overflow(map))
                    *state = GAMEOVER;
                else if (check_score_up(map))
                    *state = SCOREUP;
                else
                    *state = SHIFTING;
            }
            break;


        // отрисовка событий
        case SHIFTING:
            if (check_attach(brick, map))
                *state = ATTACHING;
            else if (check_overflow(map))
                *state = GAMEOVER;
            else if (check_score_up(map))
                *state = SCOREUP;
            else
            {
                clear_prevpos(brick, map);
                print_brick(brick, map, FALSE);
                print_stats(stats);
                *state = MOVING;
            }
            break;


        // while dropped
        case ATTACHING:
            print_brick(brick, map, TRUE);

            if (check_overflow(map)) 
                *state = GAMEOVER;
            else 
                *state = SPAWN; 
            break;


        // when all in row
        case SCOREUP:
            redraw_when_score_up(map);
            stats->score++;
            *state = MOVING;
            break;


        // when overflowing
        case GAMEOVER:
            print_gameover();
            switch (sig)
            {
                case ENTER_BTN:
                    clear_board(map);
                    print_overlay();
                    *state = START;
                    break;
                case ESCAPE_BTN:
                    *state = EXIT_STATE;
                    break;
                // loop before enter
                default:
                    *state = GAMEOVER;
                    break;
            }
            break;


        // pause
        case PAUSE:

            print_pause();
            switch (sig)
            {
                case ENTER_BTN:
                    clear_pause();
                    *state = MOVING;
                    break;
                case ESCAPE_BTN:
                    *state = EXIT_STATE;
                    break;
                // loop before enter
                default:
                    *state = PAUSE;
                    break;
            }
            break;


        //////
        case EXIT_STATE:
            *state = EXIT_STATE;
            break;

        //////
        default:
            break;
    }
}
