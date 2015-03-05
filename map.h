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

typedef unsigned char tile_t;

/* defines whether the hero can walk on the tile */
#define TILE_UNPASSABLE    (0x80)

#define TILE_GRASS         (0x00)
#define TILE_TREE          (0x01 | TILE_UNPASSABLE)

extern unsigned hero_pos_x;
extern unsigned hero_pos_y;

void move_hero_up(void);
void move_hero_down(void);
void move_hero_left(void);
void move_hero_right(void);

void map_init(void);
void map_fini(void);
void map_render(void);

#endif /* MAP_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

