/*
 *
 * graphutils.h
 *
 * Created at:  Wed Mar  4 15:50:38 2015 15:50:38
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef GRAPHUTILS_H
#define GRAPHUTILS_H

Uint32 get_pixel(SDL_Surface *surf, unsigned x, unsigned y);
void set_pixel(SDL_Surface *surf, unsigned x, unsigned y, Uint32 color);
void replace_pixels(SDL_Surface *surf, Uint32 old, Uint32 new);

#endif /* GRAPHUTILS_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

