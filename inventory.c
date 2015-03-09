/*
 *
 * inventory.c
 *
 * Created at:  Sun Mar  8 15:38:12 2015 15:38:12
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "item.h"
#include "stack.h"
#include "text.h"
#include "inventory.h"
#include "event.h"

STACK_DECLARE(inv, struct item);

scene_t inventory_scene =
  (scene_t){ inventory_scene_preswitch, inventory_scene_render };

void inventory_init(void)
{
  STACK_DEFINE(inv, 16);
}

void inventory_fini(void)
{
  STACK_FINI(inv);
}

void inventory_scene_preswitch(void)
{
  event_clear_all();

  event_handlers[SDLK_ESCAPE] = (event_handler_t){ false, inventory_close };
  event_handlers[SDLK_r] = (event_handler_t){ false, dump_scenes };
}

void inventory_scene_render(void)
{
  unsigned y = 0;

  for (unsigned x = 0; x < WINDOW_COLS; x++){
    for (y = 0; y < WINDOW_ROWS; y++){
      drawch(' ', x, y);
    }
  }

  y = 3;
  draws("`yinventory list:", 3, y++);

  STACK_ITER(inv, struct item, item){
    current_color = 'w';
    draws("- ", 3, y);
    draws(item->name, 5, y++);
  }
}

void inventory_close(void)
{
  scene_previous();
}

void inventory_open(void)
{
  scene_setnew(inventory_scene);
}

void add_to_inventory(struct item item)
{
  STACK_PUSH(inv, item);
}

void dump_inventory(void)
{
  STACK_ITER(inv, struct item, item){
    printf("#%p - %s\n", (void *)item, item->name);
    fflush(stdout);
  }
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

