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
#include "text.h"

struct item inventory[10];

static unsigned curr = 0;

bool add_to_inventory(struct item item)
{
  if (curr >= 10)
    return false;

  inventory[curr++] = item;

  return true;
}

void dump_inventory(void)
{
  for (unsigned i = 0; i < curr; i++){
    printf("#%d - %s\n", i, inventory[i].name);
    fflush(stdout);
  }
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

