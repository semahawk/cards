/*
 *
 * projectile.c
 *
 * Created at:  Mon Mar 16 21:08:38 2015 21:08:38
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <math.h>

#include "main.h"
#include "map.h"
#include "projectile.h"
#include "list.h"
#include "text.h"

/* a list of all the projectiles in the world */
struct projectiles projectiles = SLIST_HEAD_INITIALIZER();

void shoot_projectile(void)
{
  struct projectile *n;

  if (target == NULL){
    printf("no target set\n");
    fflush(stdout);

    return;
  }

  n = malloc(sizeof(struct projectile));

  n->origin_x = hero_pos_x + 0.5;
  n->origin_y = hero_pos_y + 0.5;
  n->angle = calc_angle((struct position){ hero_pos_x, hero_pos_y }, target->pos);
  /* one character per turn */
  n->speed = 1;

  SLIST_INSERT_HEAD(&projectiles, n, projectile);

  action_issued = true;

  printf("new projectile %p, angle %f, speed %f, origin x %f, origin y %f\n", (void *)n, n->angle, n->speed, n->origin_x, n->origin_y);
}

void update_projectiles(void)
{
  struct projectile *p;

  SLIST_FOREACH(p, &projectiles, projectile){
    double x = p->speed * cos(p->angle);
    double y = p->speed * sin(p->angle);

    /*printf("updating projectile %p, speed %f, old x %f, old y %f, x %f, y %f, trig %f\n", (void *)p, p->speed, p->origin_x, p->origin_y, x, y, cos(p->angle * 3.14159265 / 180));*/

    p->origin_x += x;
    p->origin_y += y;

    if (!is_passable((int)p->origin_x, (int)p->origin_y)){
      SLIST_REMOVE(&projectiles, p, projectile, projectile);
    }
  }
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

