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

#define CHUNK_WIDTH  256
#define CHUNK_HEIGHT 256

typedef unsigned char tile_t;

struct chunk {
  tile_t tiles[CHUNK_WIDTH][CHUNK_HEIGHT];
};

extern unsigned map_origin_x, map_origin_y;
extern struct chunk *chunks[3][3];

/* defines whether the hero can walk on the tile */
#define TILE_UNPASSABLE    (0x80)

#define TILE_GRASS         (0x00)
#define TILE_TREE          (0x01 | TILE_UNPASSABLE)
#define TILE_RIVER         (0x02)
#define TILE_MAGMA         (0x03)

extern unsigned hero_pos_x;
extern unsigned hero_pos_y;

extern unsigned map_width;
extern unsigned map_height;

extern scene_t map_scene;

extern struct actor *target;

void move_hero_up(void);
void move_hero_down(void);
void move_hero_left(void);
void move_hero_right(void);

void map_init(void);
void map_fini(void);
void map_scene_preswitch(void);
void map_scene_render(void);

struct chunk *load_chunk(unsigned x, unsigned y);

/* use this macro to access a tile in position (x, y) from the map */
static inline tile_t tile(unsigned x, unsigned y)
{
  unsigned idx_x = ((x - x % CHUNK_WIDTH)  - map_origin_x) / CHUNK_WIDTH;
  unsigned idx_y = ((y - y % CHUNK_HEIGHT) - map_origin_y) / CHUNK_HEIGHT;

  struct chunk *chunk = chunks[idx_x][idx_y];

  return chunk->tiles[(x - map_origin_x) % CHUNK_WIDTH][(y - map_origin_y) % CHUNK_HEIGHT];
}

static inline bool is_passable(unsigned x, unsigned y)
{
  if (x >= map_width || y >= map_height)
    return false;

  if (tile(x, y) & TILE_UNPASSABLE)
    return false;

  if (x == hero_pos_x && y == hero_pos_y)
    return false;

  {
    struct actor *actor;

    SLIST_FOREACH(actor, &actors, actor){
      if (actor->pos.x == x && actor->pos.y == y)
        return false;
    }
  }

  return true;
}

#endif /* MAP_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

