/*
 *
 * text.h
 *
 * Created at:  Tue Mar  3 21:35:21 2015 21:35:21
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef TEXT_H
#define TEXT_H

#include <SDL/SDL.h>

void drawch(Uint16 ch, unsigned x, unsigned y);
void draws(const char *s, unsigned x, unsigned y);
void text_init1(const char *font_path);
void text_init2(void);
void text_fini(void);

extern SDL_Rect font_bitmap_chars[256];

extern unsigned font_height, font_width;

#endif /* TEXT_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

