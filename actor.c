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

void actor_apply_effect(struct actor *actor, enum effect effect)
{
  if (actor == NULL)
    return;

  for (unsigned i = 0; i < 12; i++){
    if (actor->effects[i] == EFFECT_NONE){
      actor->effects[i] = effect;
      actor->effect_num++;
      break;
    }
  }
}

void actor_remove_effect(struct actor *actor, enum effect effect)
{
  if (actor == NULL)
    return;

  for (unsigned i = 0; i < 12; i++){
    if (actor->effects[i] == effect){
      actor->effects[i] = EFFECT_NONE;
      actor->effect_num--;
      break;
    }
  }
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

