/*
 *
 * render.c
 *
 * Created at:  Mon Mar  9 17:09:09 2015 17:09:09
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"
#include "stack.h"
#include "event.h"
#include "inventory.h"

void render_init(void)
{
  STACK_DEFINE(renderers, 5);
}

void render_setnew(renderer_t renderer)
{
  renderer.preswitch();

  STACK_PUSH(renderers, renderer);
}

void render_previous(void)
{
  if (!STACK_EMPTY(renderers)){
    (void)STACK_POP(renderers);
  }

  (STACK_TOP(renderers)).preswitch();
}

void inventory_close(void)
{
  render_previous();
}

void inventory_open(void)
{
  event_clear_all();

  render_setnew(inventory_renderer);
}

void dump_renderers(void)
{
  printf("Renderers:\n");

  STACK_ITER(renderers, renderer_t, r){
    printf("%p ", r);
  }

  printf("\n\n");
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

