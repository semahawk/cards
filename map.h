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
#include <math.h>

#include "main.h"
#include "actor.h"
#include "position.h"
#include "scene.h"

#define CHUNK_WIDTH  64
#define CHUNK_HEIGHT 64

typedef unsigned char tile_t;

/* align value <v> down to a <b> boundary */
#define ALIGNDOWN(v,b) ((v) - mod((v), (b)))

struct chunk {
  tile_t tiles[CHUNK_WIDTH][CHUNK_HEIGHT];
};

struct tiletype {
  const char *face;
  char color;
  uint8_t flags;
};

/* defines whether the hero can walk on the tile */
#define TILE_UNPASSABLE    (0x80)

/* all the terrain tile types */
#define TILE_GRASS         (0x00)
#define TILE_TREE          (0x01)
#define TILE_RIVER         (0x02)
#define TILE_MAGMA         (0x03)

#define TILE_UNKNOWN       (0xff)

extern int map_origin_x, map_origin_y;
extern struct chunk *chunks[3][3];

extern int hero_pos_x;
extern int hero_pos_y;

extern int map_width;
extern int map_height;

extern scene_t map_scene;

extern struct actor *target;

extern struct tiletype tiletypes[];

void move_hero_up(void);
void move_hero_down(void);
void move_hero_left(void);
void move_hero_right(void);

void map_init(void);
void map_fini(void);
void map_scene_preswitch(void);
void map_scene_render(void);

struct chunk *load_chunk(int x, int y);

static inline struct chunk *get_chunk(int x, int y)
{
  unsigned idx_x = abs((map_origin_x - (x + (CHUNK_WIDTH  - mod(x, CHUNK_WIDTH))))  / CHUNK_WIDTH);
  unsigned idx_y = abs((map_origin_y - (y + (CHUNK_HEIGHT - mod(y, CHUNK_HEIGHT)))) / CHUNK_HEIGHT);

  if (idx_x >= 3 || idx_y >= 3){
    /*fprintf(stderr, "get_chunk: error (%d,%d) - (%d,%d) -> (%u,%u)\n", map_origin_x, map_origin_y, x, y, idx_x, idx_y);*/
    return NULL;
  }

  return chunks[idx_x][idx_y];
}

/*#define tile(x,y) _tile(x, y, __FILE__, __LINE__, __func__)*/

/* access a tile in position (x, y) */
/* it has to be within the range of the loaded chunks */
/*static inline tile_t _tile(int x, int y, const char *file, unsigned line, const char *func)*/
static inline tile_t tile(int x, int y)
{
  struct chunk *chunk = get_chunk(x, y);

  if (chunk)
    return chunk->tiles[mod(x, CHUNK_WIDTH)][mod(y, CHUNK_HEIGHT)];
  else
  {
    /*printf("calling `tile` with (%d,%d) from %s:%u in `%s`\n", x, y, file, line, func);*/
    /*fflush(stdout);*/
    return TILE_TREE;
  }
}

/*#define is_passable(x,y) _is_passable(x, y, __FILE__, __LINE__, __func__)*/

/*static inline bool _is_passable(int x, int y, const char *file, unsigned line, const char *func)*/
static inline bool is_passable(int x, int y)
{
  /*printf("calling `is_passable` with (%d,%d) from %s:%u in `%s`\n", x, y, file, line, func);*/
  /*fflush(stdout);*/

  if (tiletypes[tile(x, y)].flags & TILE_UNPASSABLE)
    return false;

  if (x == hero_pos_x && y == hero_pos_y)
    return false;

  {
    struct actor *actor;

    SLIST_FOREACH(actor, &rendered_actors, actor){
      if (actor->pos.x == x && actor->pos.y == y)
        return false;
    }
  }

  return true;
}

/*
 * Distance between point <p1> and point <p2> measured in tiles.
 */
static inline int distance(struct position p1, struct position p2)
{
  /* use the Pythagorean formula */
  return round(sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)));
}

#endif /* MAP_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

