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

STACK_DECLARE(inv, struct item);

void inventory_init(void)
{
  STACK_DEFINE(inv, 16);
}

void inventory_fini(void)
{
  STACK_FINI(inv);
}

void inventory_renderer(void)
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

