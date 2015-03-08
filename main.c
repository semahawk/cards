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
#include "text.h"
#include "inventory.h"
#include "map.h"

unsigned WINDOW_COLS = 80, WINDOW_ROWS = 36;

float frames_per_second;

SDL_Surface *screen = NULL;

bool running = true;
bool next_turn  = false;
unsigned turn = 0;

void handle_events()
{
  SDL_Event event;

  SDL_PollEvent(&event);

  Uint8* keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_LEFT] || keystate[SDLK_h]){
    move_hero_left();
    next_turn = true;
  }

  if (keystate[SDLK_RIGHT] || keystate[SDLK_l]){
    move_hero_right();
    next_turn = true;
  }

  if (keystate[SDLK_UP] || keystate[SDLK_k]){
    move_hero_up();
    next_turn = true;
  }

  if (keystate[SDLK_DOWN] || keystate[SDLK_j]){
    move_hero_down();
    next_turn = true;
  }

  if (keystate[SDLK_PERIOD])
    next_turn = true;

  switch (event.type){
    case SDL_QUIT:
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
      else if (event.key.keysym.sym == SDLK_i){
        dump_inventory();
      }
      else if (event.key.keysym.sym == SDLK_p){
        struct item item;
        for (int i = 0; i < 10; i++){
          item = items[i];

          if (item.pos.x == hero_pos_x && item.pos.y == hero_pos_y){
            add_to_inventory(item);
            /* TODO remove the item from the map */
          }
        }
      }
      break;
    default:
      break;
  }
}

void cap_frame_rate(void)
{
  static unsigned wait_time = 1000.0f / FPS_CAP;
  static unsigned frame_start_time = 0;
  static int delay_time;

  delay_time = wait_time - (SDL_GetTicks() - frame_start_time);

  if (delay_time > 0)
    SDL_Delay((unsigned)delay_time);

  frame_start_time = SDL_GetTicks();
}

void count_fps(void)
{
  static float alpha = 0.05;
  static Uint32 get_ticks, frame_time_delta, frame_time_last;
  static float frame_time;

  get_ticks = SDL_GetTicks();
  frame_time_delta = get_ticks - frame_time_last;
  frame_time_last = get_ticks;
  frame_time = alpha * frame_time_delta + (1.0 - alpha) * frame_time;
  frames_per_second = 1000.0 / frame_time;
}

void draw_infobar(void)
{
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

  map_init();

  map_render();
  /*draw_infobar();*/
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

    map_render();
    /*draw_infobar();*/
    SDL_Flip(screen);
    cap_frame_rate();
  }

  map_fini();
  text_fini();

  atexit(SDL_Quit);

  return 0;
}

