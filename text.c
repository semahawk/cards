/*
 *
 * text.c
 *
 * Created at:  Tue Mar  3 21:36:52 2015 21:36:52
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>

#include "text.h"
#include "main.h"
#include "graphutils.h"

SDL_Rect screen_chars[128][128]/* meh */;

SDL_Surface *font_bitmap;
SDL_Rect font_bitmap_chars[256];
Uint32 font_bitmap_bgcolor;

unsigned font_height, font_width;

void drawch(Uint16 ch, unsigned x, unsigned y)
{
  if (x > WINDOW_COLS || y > WINDOW_ROWS || ch >= 256)
    return;

  SDL_BlitSurface(font_bitmap, &font_bitmap_chars[ch], screen, &screen_chars[x][y]);
}

void draws(const char *s, unsigned x, unsigned y)
{
  for (const char *p = s; *p != '\0'; p++){
    if (x >= WINDOW_COLS){
      x = 0; y++;
    }

    drawch(*p, x++, y);
  }
}

void load_font(const char *font_path)
{
  font_bitmap = SDL_LoadBMP(font_path);

  if (!font_bitmap){
    fprintf(stderr, "load_font: %s\n", SDL_GetError());
    exit(1);
  }

  /* height / width of a single character */
  font_height = font_bitmap->h / 16;
  font_width  = font_bitmap->w / 16;

  font_bitmap_bgcolor = getPixel32(font_bitmap, 0, 0);

  unsigned current_char = 0;

  for (int row = 0; row < 16; row++){
    for (int col = 0; col < 16; col++){
      font_bitmap_chars[current_char].x = font_width * col;
      font_bitmap_chars[current_char].y = font_height * row;
      font_bitmap_chars[current_char].w = font_width;
      font_bitmap_chars[current_char].h = font_height;

      current_char++;
    }
  }
}

void text_init(const char *font_path)
{
  load_font(font_path);

  for (unsigned col = 0; col < WINDOW_COLS; col++){
    for (unsigned row = 0; row < WINDOW_ROWS; row++){
      screen_chars[col][row].x = font_width * col;
      screen_chars[col][row].y = font_height * row;
      screen_chars[col][row].w = font_width;
      screen_chars[col][row].h = font_height;
    }
  }
}

void text_fini(void)
{
  SDL_FreeSurface(font_bitmap);
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

