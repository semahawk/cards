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
#include <limits.h>

#include "actor.h"
#include "map.h"
#include "position.h"
#include "list.h"

static int actor_update_radius = 4;

struct rendered_actors rendered_actors = SLIST_HEAD_INITIALIZER();
struct unrendered_actors unrendered_actors = SLIST_HEAD_INITIALIZER();

void ai_flee(struct actor *actor, void *data)
{
  /* run! */
  struct position new_pos;
  int delta_x = 0, delta_y = 0;
  struct actor *danger = (struct actor *)data;

  /*printf("actor %p at (%d,%d) is running from %p at (%d,%d)\n", (void*)actor, actor->pos.x, actor->pos.y, (void*)danger, danger->pos.x, danger->pos.y);*/
  /*fflush(stdout);*/

  if (danger->pos.x > actor->pos.x)
    delta_x--;
  else
    delta_x++;

  if (danger->pos.y > actor->pos.y)
    delta_y--;
  else
    delta_y++;

  new_pos.x = actor->pos.x + delta_x;
  new_pos.y = actor->pos.y + delta_y;

  if (is_passable(new_pos.x, new_pos.y))
    actor->pos = new_pos;
}

void ai_wander(struct actor *actor, void *data)
{
  /* wander about */
  struct position new_pos;
  /* supress warning */
  (void)data;

  /* 15% chance to make a move in random direction */
  if (rand() % 100 > 15)
    return;

  new_pos.x = actor->pos.x + ((rand() % 3) - 1);
  new_pos.y = actor->pos.y + ((rand() % 3) - 1);

  if (is_passable(new_pos.x, new_pos.y))
    actor->pos = new_pos;
}

void ai_attack(struct actor *actor, void *data)
{
  /* head towards the target */
  struct position new_pos;
  struct actor *prey = (struct actor *)data;

  /*printf("actor '%c' (%p) at (%d,%d) is chasing '%c' (%p) at (%d,%d)\n", actor->face, (void*)actor, actor->pos.x, actor->pos.y, prey->face, (void*)prey, prey->pos.x, prey->pos.y);*/
  /*fflush(stdout);*/

  if (distance(actor->pos, prey->pos) <= 1){
    /* the actor is close enough to the prey to attack it */
    prey->hp--;
  } else {
    new_pos = path_find_next_step(actor->pos, prey->pos);

    if (is_passable(new_pos.x, new_pos.y))
      actor->pos = new_pos;
  }
}

void (*ai_func_handlers[])(struct actor *, void *) =
{
  [AI_STATE_FLEE]   = ai_flee,
  [AI_STATE_WANDER] = ai_wander,
  [AI_STATE_ATTACK] = ai_attack,
};

struct ai_transition ai_predator[] =
{
  { AI_STATE_WANDER, AI_COND_DANGER, AI_STATE_ATTACK },
  { AI_STATE_WANDER, AI_COND_TRANQUILLITY, AI_STATE_WANDER },
  { AI_STATE_ATTACK, AI_COND_DANGER, AI_STATE_ATTACK },
  { AI_STATE_ATTACK, AI_COND_TRANQUILLITY, AI_STATE_WANDER },
  { -1, -1, -1 }
};

struct ai_transition ai_prey[] =
{
  { AI_STATE_WANDER, AI_COND_DANGER, AI_STATE_FLEE },
  { AI_STATE_WANDER, AI_COND_TRANQUILLITY, AI_STATE_WANDER },
  { AI_STATE_FLEE, AI_COND_DANGER, AI_STATE_FLEE },
  { AI_STATE_FLEE, AI_COND_TRANQUILLITY, AI_STATE_WANDER },
  { -1, -1, -1 }
};

void actor_new(char face, char color, struct position pos, struct ai_transition *archetype)
{
  struct actor *n = malloc(sizeof(struct actor));
  struct ai *ai = malloc(sizeof(struct ai));

  ai->action_handler = ai_wander;
  ai->state = AI_STATE_WANDER;
  ai->archetype = archetype;

  n->is_dead = false;
  n->face = face;
  n->color = color;
  n->pos = pos;
  n->effect_num = 0;
  n->hp = 12;
  n->ai = ai;

  printf("created a new actor at (%d,%d)\n", pos.x, pos.y);

  memset(n->effects, EFFECT_NONE, sizeof(n->effects));

  int dist = distance(pos, the_hero.pos);

  if (dist < 2 * CHUNK_WIDTH){
    SLIST_INSERT_HEAD(&rendered_actors, n, actor);
  } else if (dist > actor_update_radius){
    /* huh... nop? */
  } else {
    SLIST_INSERT_HEAD(&unrendered_actors, n, actor);
  }
}

void update_actors(void)
{
  struct actor *actor, *temp;

  SLIST_FOREACH_SAFE(actor, &rendered_actors, actor, temp){
    /* pretty much ignore them */
    if (actor->is_dead)
      continue;

    /* let's check if he's alive at all */
    if (actor->hp <= 0){
      /* "Dead men do not squat, lass, they sprawl." */
      free(actor->ai);
      actor->ai = NULL;
      /* let's make him dead */
      actor->is_dead = true;

      printf("actor %p has died.\n", (void *)actor);

      target = NULL;

      continue;
    }

    /* let's see if the actor is outside of the rendered portion of the map */
    /* if so, let's place him in the second list */
    if (distance(actor->pos, the_hero.pos) >= 2 * CHUNK_WIDTH){
      SLIST_REMOVE(&rendered_actors, actor, actor, actor);
      SLIST_INSERT_HEAD(&unrendered_actors, actor, actor);

      printf("actor %p is outside of the render radius\n", (void *)actor);

      continue;
    }

    void *data = NULL;
    enum ai_condition cond;

    /* here we can actually update the actor */
    /* let's see how things have changed */
    if ((data = danger_nearby(actor)) != NULL){
      cond = AI_COND_DANGER;
    } else {
      cond = AI_COND_TRANQUILLITY;
    }

    for (struct ai_transition *t = actor->ai->archetype; t->from != -1; t++){
      if (t->from == actor->ai->state && t->cond == cond){
      /*printf("actor %p: %d -> %d -> %d\n", (void*)actor, t->from, t->cond, t->to);*/
        actor->ai->state = t->to;
        actor->ai->action_handler = ai_func_handlers[t->to];
        break;
      }
    }

    /* update the actor according to his state */
    actor->ai->action_handler(actor, data);
  }

  SLIST_FOREACH_SAFE(actor, &unrendered_actors, actor, temp){
    /* see if the actor came inside of the viewport, and if so, transfer him to
     * the appropriate list */
    if (distance(actor->pos, the_hero.pos) < 2 * CHUNK_WIDTH){
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

struct actor *danger_nearby(struct actor *actor)
{
  struct actor *other;
  struct actor *ret = NULL;
  int shortest = INT_MAX;
  int dist;

  SLIST_FOREACH(other, &rendered_actors, actor){
    if (other == actor) continue;
    if (other->is_dead) continue;

    if ((dist = distance(actor->pos, other->pos)) < 130){
      if (dist < shortest){
        shortest = dist;
        ret = other;
      }
    }
  }

  /* treat the hero as a potential danger */
  /*if (distance(actor->pos, the_hero.pos) < 13){*/
    /* well.. yeaah, don't try that at home */
    /*ret = (struct actor *)&the_hero;*/
  /*}*/

  return ret;
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

