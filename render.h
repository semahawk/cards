/*
 *
 * render.h
 *
 * Created at:  Mon Mar  9 17:08:31 2015 17:08:31
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef RENDER_H
#define RENDER_H

#include "stack.h"

typedef struct {
  void (*preswitch)(void);
  void (*render)(void);
} renderer_t;

STACK_DECLARE(renderers, renderer_t);

void render_init(void);
void render_setnew(renderer_t);
void dump_renderers(void);

#endif /* RENDER_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

