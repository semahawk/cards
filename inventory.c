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

#include "stack.h"
#include "text.h"
#include "inventory.h"
#include "item.h"
#include "event.h"

scene_t inventory_scene =
  (scene_t){ inventory_scene_preswitch, inventory_scene_render };

void inventory_init(void)
{
}

void inventory_fini(void)
{
  struct inv_elem *e;

  while (!SLIST_EMPTY(&the_hero.inv)){
    e = SLIST_FIRST(&the_hero.inv);
    SLIST_REMOVE_HEAD(&the_hero.inv, inv_elem);
    free(e);
  }
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

  struct inv_elem *e;

  SLIST_FOREACH(e, &the_hero.inv, inv_elem){
    current_color = 'w';
    draws("- ", 3, y);
    draws(e->item->name, 5, y++);
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

void add_to_inventory(struct item *item)
{
  struct inv_elem *n = malloc(sizeof(struct inv_elem));

  n->item = item;

  SLIST_INSERT_HEAD(&the_hero.inv, n, inv_elem);

  printf("added %s to inventory\n", item->name);
}

void dump_inventory(void)
{
  struct inv_elem *e;

  SLIST_FOREACH(e, &the_hero.inv, inv_elem){
    printf("#%p - %s\n", (void *)e, e->item->name);
    fflush(stdout);
  }
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

