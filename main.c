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

#include <SDL/SDL.h>

#include "main.h"
#include "text.h"

unsigned WINDOW_COLS = 80, WINDOW_ROWS = 36;

SDL_Surface *screen = NULL;

bool running = true;

void dispatch_event(SDL_Event event)
{
  switch (event.type){
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
    default:
      /* nop */;
  }
}

int main(int argc, char *argv[])
{
  SDL_Event event;

  /* suspress warnings */
  (void)argc;
  (void)argv;

  SDL_Init(SDL_INIT_EVERYTHING);

  text_init1("./font.bmp");
  screen = SDL_SetVideoMode(WINDOW_COLS * font_width, WINDOW_ROWS * font_height, 32, SDL_HWSURFACE);
  text_init2();

  while (running){
    SDL_PollEvent(&event);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    dispatch_event(event);

    drawch('C', 1, 2);
    draws("`wh`rh`gh`bh`ch`mh`yh`kh", 0, 0);

    SDL_Flip(screen);
  }

  text_fini();

  atexit(SDL_Quit);

  return 0;
}

