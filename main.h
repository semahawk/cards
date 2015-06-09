/*
 *
 * main.h
 *
 * Created at:  Tue Mar  3 21:41:50 2015 21:41:50
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <SDL/SDL.h>

#include "hero.h"

#define FPS_CAP       (25)

#define GAME_SPEED    (20)
#define MAX_FRAMESKIP (10)

extern struct hero the_hero;

extern unsigned WINDOW_COLS, WINDOW_ROWS;

extern SDL_Surface *screen;
extern bool action_issued;
extern unsigned turn;

extern uint32_t seed;

void stop_running(void);
void next_turn(void);
void skip_turn(void);

#define mod(a,b) (((a < 0) ? ((a % b) + b) : a) % b)
#define MAX(a,b) ((a > b) ? (a) : (b))
#define MIN(a,b) ((a < b) ? (a) : (b))

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

#endif /* MAIN_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

