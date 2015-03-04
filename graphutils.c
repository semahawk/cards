/*
 *
 * graphutils.c
 *
 * Created at:  Wed Mar  4 15:48:13 2015 15:48:13
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>

Uint32 get_pixel(SDL_Surface *surf, unsigned x, unsigned y)
{
  int bpp = surf->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;

  switch (bpp){
    case 1: return *p;
    case 2: return *(Uint16 *)p;
    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;
    case 4: return *(Uint32 *)p;
    default: return 0;
  }
}

void set_pixel(SDL_Surface *surf, unsigned x, unsigned y, Uint32 color)
{
  int bpp = surf->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;

  switch (bpp){
    case 1:
      *p = color;
      break;
    case 2:
      *(Uint16 *)p = color;
      break;
    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN){
        p[0] = (color >> 16) & 0xff;
        p[1] = (color >> 8) & 0xff;
        p[2] = (color) & 0xff;
      }
      else {
        p[0] = (color) & 0xff;
        p[1] = (color >> 8) & 0xff;
        p[2] = (color >> 16) & 0xff;
      }
      break;
    case 4:
      *(Uint32 *)p = color;
      break;
  }
}

void replace_pixels(SDL_Surface *surf, Uint32 old, Uint32 new)
{
  for (int x = 0; x < surf->w; x++)
    for (int y = 0; y < surf->h; y++)
      if (get_pixel(surf, x, y) == old)
        set_pixel(surf, x, y, new);
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

