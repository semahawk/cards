/*
 *
 * actor.c
 *
 * Created at:  Sat Mar  7 13:20:50 2015 13:20:50
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "actor.h"
#include "map.h"

struct actor actors[10 /* meh */];

void update_actors(void)
{
  struct actor *actor;
  struct position new_pos;

  for (unsigned i = 0; i < 10; i++){
    actor = &actors[i];

    if (rand() % 100 > 15)
      continue;

    new_pos.x = actor->pos.x + ((rand() % 3) - 1);
    new_pos.y = actor->pos.y + ((rand() % 3) - 1);

    if (is_passable(new_pos.x, new_pos.y))
      actor->pos = new_pos;
  }
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

