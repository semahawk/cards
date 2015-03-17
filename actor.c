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
#include "list.h"

struct actors actors = SLIST_HEAD_INITIALIZER();

void actor_new(char face, char color, struct position pos)
{
  struct actor *n = malloc(sizeof(struct actor));

  n->face = face;
  n->color = color;
  n->pos = pos;
  n->effect_num = 0;
  n->hp = 12;

  memset(n->effects, EFFECT_NONE, sizeof(n->effects));

  SLIST_INSERT_HEAD(&actors, n, actor);
}

void update_actors(void)
{
  struct actor *actor;
  struct position new_pos;

  SLIST_FOREACH(actor, &actors, actor){
    /* let's check if he's alive at all */
    if (actor->hp <= 0){
      SLIST_REMOVE(&actors, actor, actor, actor);
      printf("actor %p has died.\n", (void *)actor);
    }

    /* 15% chance to make a move in random direction */
    if (rand() % 100 > 15)
      continue;

    new_pos.x = actor->pos.x + ((rand() % 3) - 1);
    new_pos.y = actor->pos.y + ((rand() % 3) - 1);

    if (is_passable(new_pos.x, new_pos.y))
      actor->pos = new_pos;
  }
}

struct actor *actor_is_at_pos(struct position pos)
{
  struct actor *actor;

  SLIST_FOREACH(actor, &actors, actor){
    if (actor->pos.x == pos.x && actor->pos.y == pos.y)
      return actor;
  }

  return NULL;
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

