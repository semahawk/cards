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

#define COLORS_NUM 8

struct color {
  char code;
  Uint8 r, g, b;
  SDL_Surface *bitmap;
} colors[COLORS_NUM + 1] = {
  {  'w', 0xfa, 0xfa, 0xfa, NULL },
  {  'r', 0xef, 0x29, 0x29, NULL },
  {  'g', 0x56, 0xf7, 0x00, NULL },
  {  'b', 0x57, 0x91, 0xff, NULL },
  {  'c', 0x00, 0xf7, 0xa1, NULL },
  {  'm', 0xb9, 0x36, 0xff, NULL },
  {  'y', 0xff, 0xff, 0x00, NULL },
  {  'k', 0x26, 0x26, 0x26, NULL },
  { '\0', 0x00, 0x00, 0x00, NULL },
};

char current_color = 'w';

SDL_Rect screen_chars[128][128]/* meh */;

SDL_Surface *font_bitmap;
SDL_Rect font_bitmap_chars[256];
Uint32 font_bitmap_bgcolor;

unsigned font_height, font_width;

void drawch(Uint16 ch, unsigned x, unsigned y)
{
  SDL_Surface *bitmap;

  if (x > WINDOW_COLS || y > WINDOW_ROWS || ch >= 256)
    return;

  for (struct color *p = colors; p->code != '\0'; p++)
    if (p->code == current_color)
      bitmap = p->bitmap;

  SDL_BlitSurface(bitmap, &font_bitmap_chars[ch], screen, &screen_chars[x][y]);
}

void draws(const char *s, unsigned x, unsigned y)
{
  for (const char *p = s; *p != '\0'; p++){
    if (*p == '`'){
      p++;

      if (*p != '`'){
        current_color = *p;
        p++;
      }
    }

    if (x >= WINDOW_COLS){
      x = 0; y++;
    }

    drawch(*p, x++, y);
  }
}

void drawd(int d, unsigned x, unsigned y)
{
  char out[10] = { 0 };
  int i = 0;

  if (d < 0){
    drawch('-', x++, y);
    d = -d;
  }

  for (; d > 0; d /= 10)
    out[i++] = d % 10 + '0';

  i--;

  while (i >= 0)
    drawch(out[i--], x++, y);
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

  font_bitmap_bgcolor = get_pixel(font_bitmap, 0, 0);

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

void text_init1(const char *font_path)
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

void text_init2(void)
{
  SDL_SetColorKey(font_bitmap, SDL_SRCCOLORKEY, font_bitmap_bgcolor);

  for (struct color *color = colors; color->code != '\0'; color++){
    color->bitmap = SDL_CreateRGBSurface(0, font_bitmap->w, font_bitmap->h, 32, 0, 0, 0, 0);
    SDL_BlitSurface(font_bitmap, NULL, color->bitmap, NULL);
    /*SDL_SetColorKey(color->bitmap, SDL_SRCCOLORKEY, font_bitmap_bgcolor);*/
    replace_pixels(color->bitmap, SDL_MapRGB(color->bitmap->format, 0xff, 0xff, 0xff), SDL_MapRGB(color->bitmap->format, color->r, color->g, color->b));
  }
}

void text_fini(void)
{
  SDL_FreeSurface(font_bitmap);
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

