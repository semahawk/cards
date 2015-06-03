/*
 *
 * actor.h
 *
 * Created at:  Sat Mar  7 13:20:55 2015 13:20:55
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef ACTOR_H
#define ACTOR_H

#include "main.h"
#include "position.h"

struct actor;

enum ai_state {
  AI_STATE_WANDER,
  AI_STATE_FLEE,
  AI_STATE_ATTACK,
};

enum ai_condition {
  AI_COND_DANGER,
  AI_COND_TRANQUILLITY,
};

/*enum ai_archetype {
  AI_PREDATOR,
  AI_PREY,
  AI_ARCHETYPES_NUM
};*/

struct ai_transition {
  enum ai_state     from;
  enum ai_condition cond;
  enum ai_state     to;
};

struct ai {
  struct ai_transition *archetype;
  enum ai_state state;
  void (*action_handler)(struct actor *, void *);
};

enum effect {
  EFFECT_NONE = 0,
  EFFECT_TARGETED,
  EFFECT_BURNING,
};

struct actor {
  struct position pos;
  char face;
  char color;
  int hp;
  unsigned effect_num;
  enum effect effects[12];
  struct ai *ai;
  SLIST_ENTRY(actor) actor;
};

SLIST_HEAD(rendered_actors, actor);
SLIST_HEAD(unrendered_actors, actor);

extern struct rendered_actors rendered_actors;
extern struct unrendered_actors unrendered_actors;

extern struct ai_transition ai_predator[];
extern struct ai_transition ai_prey[];

void update_actors(void);
void actor_new(char, char, struct position, struct ai_transition *);
void actor_apply_effect(struct actor *, enum effect);
void actor_remove_effect(struct actor *, enum effect);

struct actor *actor_is_at_pos(struct position);

struct actor *danger_nearby(struct actor *);

#endif /* ACTOR_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

