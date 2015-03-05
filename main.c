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

#include "main.h"
#include "text.h"
#include "map.h"

unsigned WINDOW_COLS = 80, WINDOW_ROWS = 36;

float alpha = 0.05;
Uint32 get_ticks, frame_time_delta, frame_time_last;
float frame_time, frames_per_second;

SDL_Surface *screen = NULL;

bool running = true;

void dispatch_event(SDL_Event event)
{
  Uint8* keystate = SDL_GetKeyState(NULL);

  if (keystate[SDLK_LEFT] || keystate[SDLK_h]){
    move_hero_left();
  }

  if (keystate[SDLK_RIGHT] || keystate[SDLK_l]){
    move_hero_right();
  }

  if (keystate[SDLK_UP] || keystate[SDLK_k]){
    move_hero_up();
  }

  if (keystate[SDLK_DOWN] || keystate[SDLK_j]){
    move_hero_down();
  }

  switch (event.type){
    case SDL_QUIT:
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
          running = false;
      break;
    default:
      /* nop */;
  }
}

void cap_frame_rate(void)
{
  static unsigned wait_time = 1000.0f / FRAMES_PER_SECOND_CAP;
  static unsigned frame_start_time = 0;
  static int delay_time;

  delay_time = wait_time - (SDL_GetTicks() - frame_start_time);

  if (delay_time > 0)
    SDL_Delay((unsigned)delay_time);

  frame_start_time = SDL_GetTicks();
}

int main(int argc, char *argv[])
{
  SDL_Event event;

  /* suspress warnings */
  (void)argc;
  (void)argv;

  SDL_Init(SDL_INIT_EVERYTHING);

  srand(time(NULL));

  text_init1("./font.bmp");
  screen = SDL_SetVideoMode(WINDOW_COLS * font_width, WINDOW_ROWS * font_height, 32, SDL_HWSURFACE);
  text_init2();

  map_init();

  while (running){
    SDL_PollEvent(&event);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    dispatch_event(event);

    map_render();

    draws("`bfps `cm", 0, 0);
    drawd((int)frames_per_second, 4, 0);

    draws("`bx:`cm", 7, 0);
    drawd(hero_pos_x, 9, 0);
    draws("`by:`cm", 12, 0);
    drawd(hero_pos_y, 14, 0);

    SDL_Flip(screen);

    get_ticks = SDL_GetTicks();
    frame_time_delta = get_ticks - frame_time_last;
    frame_time_last = get_ticks;
    frame_time = alpha * frame_time_delta + (1.0 - alpha) * frame_time;
    frames_per_second = 1000.0 / frame_time;

    cap_frame_rate();
  }

  text_fini();

  atexit(SDL_Quit);

  return 0;
}

