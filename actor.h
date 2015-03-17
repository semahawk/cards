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

enum effect {
  EFFECT_NONE = 0,
  EFFECT_TARGETED,
  EFFECT_BURNING,
};

struct actor {
  char face;
  char color;
  struct position pos;
  int hp;
  unsigned effect_num;
  enum effect effects[12];
  SLIST_ENTRY(actor) actor;
};

SLIST_HEAD(actors, actor);

extern struct actors actors;

void update_actors(void);
void actor_new(char, char, struct position);
void actor_apply_effect(struct actor *, enum effect);
void actor_remove_effect(struct actor *, enum effect);

struct actor *actor_is_at_pos(struct position);

#endif /* ACTOR_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

