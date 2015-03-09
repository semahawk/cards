/*
 *
 * main.c
 *
 * Created at:  Tue Mar  3 02/03/2015 16:30:53 PM
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License: the MIT license
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <SDL/SDL.h>

#include "actor.h"
#include "main.h"
#include "event.h"
#include "text.h"
#include "inventory.h"
#include "map.h"
#include "render.h"
#include "stack.h"

unsigned WINDOW_COLS = 80, WINDOW_ROWS = 36;

float frames_per_second;

SDL_Surface *screen = NULL;

bool running = true;
bool next_turn  = false;
unsigned turn = 0;

void cap_frame_rate(void)
{
  /* {{{ */
  static unsigned wait_time = 1000.0f / FPS_CAP;
  static unsigned frame_start_time = 0;
  static int delay_time;

  delay_time = wait_time - (SDL_GetTicks() - frame_start_time);

  if (delay_time > 0)
    SDL_Delay((unsigned)delay_time);

  frame_start_time = SDL_GetTicks();
  /* }}} */
}

void count_fps(void)
{
  /* {{{ */
  static float alpha = 0.05;
  static Uint32 get_ticks, frame_time_delta, frame_time_last;
  static float frame_time;

  get_ticks = SDL_GetTicks();
  frame_time_delta = get_ticks - frame_time_last;
  frame_time_last = get_ticks;
  frame_time = alpha * frame_time_delta + (1.0 - alpha) * frame_time;
  frames_per_second = 1000.0 / frame_time;
  /* }}} */
}

void draw_infobar(void)
{
  /* {{{ */
  int x = 0;

  count_fps();

  draws("`bfps `c", x, 0);
  x += 5;
  draws("   ", x, 0);

  drawd((int)frames_per_second, x, 0);
  x += 4;

  draws("`bx:`c", x, 0);
  x += 6;
  draws("   ", x, 0);

  drawd(hero_pos_x, x, 0);
  x += 5;

  draws("`by:`c", x, 0);
  x += 6;
  draws("   ", x, 0);

  drawd(hero_pos_y, x, 0);
  x += 4;

  draws("`bturn:`c", x, 0);
  x += 8;
  drawd(turn, x, 0);
  /* }}} */
}

void stop_running(void)
{
  running = false;
}

void update_world(void)
{
  update_actors();
}

int main(int argc, char *argv[])
{
  /* suspress warnings */
  (void)argc;
  (void)argv;

  SDL_Init(SDL_INIT_EVERYTHING);

  srand(time(NULL));

  text_init1("./font.bmp");
  screen = SDL_SetVideoMode(WINDOW_COLS * font_width, WINDOW_ROWS * font_height, 32, SDL_HWSURFACE);
  text_init2();

  event_init();
  render_init();
  map_init();

  render_setnew(map_renderer);

  (STACK_TOP(renderers)).render();

  draw_infobar();
  inventory_init();

  SDL_Flip(screen);

  Uint32 next_game_tick = SDL_GetTicks();
  int loops;

  while (running){
    loops = 0;

    while (SDL_GetTicks() > next_game_tick && loops < MAX_FRAMESKIP){
      handle_events();

      if (next_turn){
        update_world();

        turn++;
      }

      next_turn = false;

      next_game_tick += 1000 / GAME_SPEED;
      loops++;
    }

    (STACK_TOP(renderers)).render();

    draw_infobar();
    SDL_Flip(screen);

    cap_frame_rate();
  }

  inventory_fini();
  map_fini();
  text_fini();

  atexit(SDL_Quit);

  return 0;
}

