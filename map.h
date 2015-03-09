/*
 *
 * map.h
 *
 * Created at:  Wed Mar  4 23:02:48 2015 23:02:48
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include "main.h"
#include "actor.h"
#include "scene.h"

typedef unsigned char tile_t;

/* defines whether the hero can walk on the tile */
#define TILE_UNPASSABLE    (0x80)

#define TILE_GRASS         (0x00)
#define TILE_TREE          (0x01 | TILE_UNPASSABLE)
#define TILE_ACTOR         (0x02)

extern unsigned hero_pos_x;
extern unsigned hero_pos_y;

extern tile_t *map_tiles;
extern unsigned map_width;
extern unsigned map_height;

void move_hero_up(void);
void move_hero_down(void);
void move_hero_left(void);
void move_hero_right(void);

void map_init(void);
void map_fini(void);
void map_scene_preswitch(void);
void map_scene_render(void);

extern scene_t map_scene;

/* use this macro to access a tile in position (x, y) from the map */
#define tile(x,y) (map_tiles)[(x) * map_height + (y)]

static inline bool is_passable(unsigned x, unsigned y)
{
  if (x >= map_width || y >= map_height)
    return false;

  if (tile(x, y) & TILE_UNPASSABLE)
    return false;

  if (x == hero_pos_x && y == hero_pos_y)
    return false;

  {
    struct actor actor;

    for (unsigned i = 0; i < 10; i++){
      actor = actors[i];

      if (actor.pos.x == x && actor.pos.y == y)
        return false;
    }
  }

  return true;
}

#endif /* MAP_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

