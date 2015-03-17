/*
 *
 * projectile.h
 *
 * Created at:  Mon Mar 16 20:11:45 2015 20:11:45
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <math.h>

#include "list.h"
#include "position.h"

struct projectile {
  double speed;
  double origin_x;
  double origin_y;
  double angle; /* in radians */
  SLIST_ENTRY(projectile) projectile;
};

SLIST_HEAD(projectiles, projectile);

extern struct projectiles projectiles;

void shoot_projectile(void);
void update_projectiles(void);

static inline double calc_angle(struct position p1, struct position p2)
{
  return atan2((int)(p2.y - p1.y), (int)(p2.x - p1.x));
}

#endif /* PROJECTILE_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

