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

void projectile_new(double origin_x, double origin_y, double angle, double speed)
{
  struct projectile *n;

  n = malloc(sizeof(struct projectile));

  n->origin_x = origin_x;
  n->origin_y = origin_y;
  n->angle = angle;
  n->speed = speed;

  SLIST_INSERT_HEAD(&projectiles, n, projectile);
}

void shoot_projectile(void)
{
  static double angle = 0;

  if (target == NULL){
    /*printf("no target set\n");*/
    /*fflush(stdout);*/

    /*return;*/
  }

  /*angle = calc_angle((struct position){ hero_pos_x, hero_pos_y }, target->pos);*/
  /*projectile_new(hero_pos_x + 0.5, hero_pos_y + 0.5, angle, 1);*/

  for (unsigned i = 0; i < 13; i++){
    projectile_new(hero_pos_x + 0.5, hero_pos_y + 0.5, deg2rad(360 / 13 * i + (angle += 13)), 1);
  }

  action_issued = true;
}

void update_projectiles(void)
{
  struct actor *actor;
  struct projectile *p;

  SLIST_FOREACH(p, &projectiles, projectile){
    double x = p->speed * cos(p->angle);
    double y = p->speed * sin(p->angle);

    /*printf("updating projectile %p, speed %f, old x %f, old y %f, x %f, y %f, trig %f\n", (void *)p, p->speed, p->origin_x, p->origin_y, x, y, cos(p->angle * 3.14159265 / 180));*/

    p->origin_x += x;
    p->origin_y += y;

    if (!is_passable((int)p->origin_x, (int)p->origin_y)){
      if ((actor = actor_is_at_pos((struct position){ p->origin_x, p->origin_y }))){
        actor->hp--;
      }

      SLIST_REMOVE(&projectiles, p, projectile, projectile);
      free(p);
    }
  }
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

