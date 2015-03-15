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

enum effect {
  EFFECT_NONE = 0,
  EFFECT_TARGETED,
  EFFECT_BURNING,
};

struct actor {
  char face;
  char color;
  struct position pos;
  unsigned effect_num;
  enum effect effects[12];
};

void update_actors(void);
void actor_apply_effect(struct actor *, enum effect);
void actor_remove_effect(struct actor *, enum effect);

extern struct actor actors[10];

#endif /* ACTOR_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

