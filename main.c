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

  text_init("./font.bmp");

  screen = SDL_SetVideoMode(WINDOW_COLS * font_width, WINDOW_ROWS * font_height, 32, SDL_SWSURFACE);

  while (running){
    SDL_PollEvent(&event);

    dispatch_event(event);

    drawch('C', 1, 2);
    draws("hello, world!", 0, 0);

    SDL_Flip(screen);
  }

  text_fini();

  atexit(SDL_Quit);

  return 0;
}

