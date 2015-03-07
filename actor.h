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

struct position {
  unsigned x, y;
};

struct actor {
  char face;
  struct position pos;
};

void update_actors(void);

extern struct actor actors[10];

#endif /* ACTOR_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

