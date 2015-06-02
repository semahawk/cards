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

static int actor_update_radius = 4;

struct rendered_actors rendered_actors = SLIST_HEAD_INITIALIZER();
struct unrendered_actors unrendered_actors = SLIST_HEAD_INITIALIZER();

void actor_new(char face, char color, struct position pos)
{
  struct actor *n = malloc(sizeof(struct actor));

  n->face = face;
  n->color = color;
  n->pos = pos;
  n->effect_num = 0;
  n->hp = 12;

  printf("created a new actor at (%d,%d)\n", pos.x, pos.y);

  memset(n->effects, EFFECT_NONE, sizeof(n->effects));

  unsigned dist_x = distance(pos.x, hero_pos_x);
  unsigned dist_y = distance(pos.y, hero_pos_y);

  if (dist_x < 2 && dist_y < 2){
    SLIST_INSERT_HEAD(&rendered_actors, n, actor);
  } else if (dist_x > actor_update_radius && dist_y > actor_update_radius){
    /* huh... nop? */
  } else {
    SLIST_INSERT_HEAD(&unrendered_actors, n, actor);
  }
}

void update_actors(void)
{
  struct actor *actor, *temp;
  struct position new_pos;

  SLIST_FOREACH_SAFE(actor, &rendered_actors, actor, temp){
    /* let's check if he's alive at all */
    if (actor->hp <= 0){
      SLIST_REMOVE(&rendered_actors, actor, actor, actor);
      free(actor);

      printf("actor %p has died.\n", (void *)actor);

      target = NULL;

      continue;
    }

    /*printf("actor %p (%d,%d): distance %d, %d\n", (void *)actor, actor->pos.x, actor->pos.y, distance(actor->pos.x, hero_pos_x), distance(actor->pos.y, hero_pos_y));*/

    /* let's see if the actor is outside of the rendered portion of the map */
    /* if so, let's place him in the second list */
    if ((distance(actor->pos.x, hero_pos_x) >= 2) ||
        (distance(actor->pos.y, hero_pos_y) >= 2)){
      SLIST_REMOVE(&rendered_actors, actor, actor, actor);
      SLIST_INSERT_HEAD(&unrendered_actors, actor, actor);

      printf("actor %p is outside of the render radius\n", (void *)actor);

      continue;
    }

    /* 15% chance to make a move in random direction */
    if (rand() % 100 > 15)
      continue;

    new_pos.x = actor->pos.x + ((rand() % 3) - 1);
    new_pos.y = actor->pos.y + ((rand() % 3) - 1);

    if (is_passable(new_pos.x, new_pos.y))
      actor->pos = new_pos;
  }

  SLIST_FOREACH_SAFE(actor, &unrendered_actors, actor, temp){
    /* see if the actor came inside of the viewport, and if so, transfer him to
     * the appropriate list */
    if ((distance(actor->pos.x, hero_pos_x) < 2) &&
        (distance(actor->pos.y, hero_pos_y) < 2)){
      SLIST_REMOVE(&unrendered_actors, actor, actor, actor);
      SLIST_INSERT_HEAD(&rendered_actors, actor, actor);

      printf("actor %p is inside of the render radius\n", (void *)actor);
    }
  }
}

struct actor *actor_is_at_pos(struct position pos)
{
  struct actor *actor;

  SLIST_FOREACH(actor, &rendered_actors, actor){
    if (actor->pos.x == pos.x && actor->pos.y == pos.y)
      return actor;
  }

  SLIST_FOREACH(actor, &unrendered_actors, actor){
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

