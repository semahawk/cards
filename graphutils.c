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

Uint32 getPixel32(SDL_Surface *surf, unsigned x, unsigned y)
{
  /* convert the pixels to 32 bit */
  Uint32 *pixels = (Uint32 *)surf->pixels;

  /* get the pixel requested */
  return pixels[(y * (surf->pitch / 4)) + x];
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

