/*
 *
 * map.c
 *
 * Created at:  Wed Mar  4 23:02:12 2015 23:02:12
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "map.h"

/* use this macro to access a tile in position (x, y) from the map */
#define tile(x,y) (map_tiles)[(x) * map_height + (y)]

/* a one dimensional array */
static tile_t *map_tiles;

static unsigned center_x;
static unsigned center_y;

unsigned hero_pos_x = 0;
unsigned hero_pos_y = 0;

static unsigned map_width = 128;
static unsigned map_height = 64;

void move_hero_up(void)
{
  if (!(tile(hero_pos_x, hero_pos_y - 1) & TILE_UNPASSABLE))
    if (hero_pos_y > 0)
      hero_pos_y--;
}

void move_hero_down(void)
{
  if (!(tile(hero_pos_x, hero_pos_y + 1) & TILE_UNPASSABLE))
    if (hero_pos_y < map_height - 1)
      hero_pos_y++;
}

void move_hero_left(void)
{
  if (!(tile(hero_pos_x - 1, hero_pos_y) & TILE_UNPASSABLE))
    if (hero_pos_x > 0)
      hero_pos_x--;
}

void move_hero_right(void)
{
  if (!(tile(hero_pos_x + 1, hero_pos_y) & TILE_UNPASSABLE))
    if (hero_pos_x < map_width - 1)
      hero_pos_x++;
}

void map_draw_tile(tile_t tile, unsigned x, unsigned y)
{
  switch (tile){
    case TILE_GRASS:
      draws("`k,", x, y);
      break;
    case TILE_TREE:
      draws("`go", x, y);
      break;
    default:
      draws("`r?", x, y);
      break;
  }
}

void map_render(void)
{
  unsigned x, y;

  /* default top-left coordinate of the chunk we want to display */
  unsigned shown_chunk_x = hero_pos_x - center_x;
  unsigned shown_chunk_y = hero_pos_y - center_y;

  /* calculate the top-left chunk's corner coordinates */
  if (hero_pos_x < center_x)
    shown_chunk_x = 0;
  else if (hero_pos_x > map_width - center_x)
    shown_chunk_x = map_width - WINDOW_COLS;

  if (hero_pos_y < center_y)
    shown_chunk_y = 0;
  else if (hero_pos_y > map_height - center_y)
    shown_chunk_y = map_height - WINDOW_ROWS;

  /* draw the chunk of the map onto the screen */
  for (unsigned x = 0; x < WINDOW_COLS && x < map_width; x++){
    for (unsigned y = 0; y < WINDOW_ROWS && y < map_height; y++){
      map_draw_tile(tile(shown_chunk_x + x, shown_chunk_y + y), x, y);
    }
  }

  /* keep the hero in the center of the screen, or draw him to the borders when
   * close */
  if (hero_pos_x < center_x)
    x = hero_pos_x;
  else if (hero_pos_x > map_width - center_x)
    x = WINDOW_COLS - (map_width - hero_pos_x);
  else
    x = center_x;

  if (hero_pos_y < center_y)
    y = hero_pos_y;
  else if (hero_pos_y > map_height - center_y)
    y = WINDOW_ROWS - (map_height - hero_pos_y);
  else
    y = center_y;

  draws("`b\1", x, y);
}

void map_init(void)
{
  center_x = WINDOW_COLS / 2;
  center_y = WINDOW_ROWS / 2;

  hero_pos_x = map_width / 2;
  hero_pos_y = map_height / 2;

  map_tiles = malloc(sizeof(tile_t) * map_width * map_height);

  if (!map_tiles){
    fprintf(stderr, "couldn't allocate memory for the map (%ux%u)\n", map_width, map_height);
    exit(1);
  }

  /* fill the entire map with grass */
  for (unsigned x = 0; x < map_width; x++){
    for (unsigned y = 0; y < map_height; y++){
      tile(x, y) = TILE_GRASS;
    }
  }

  /* make some of the tiles be trees */
  for (unsigned i = 0; i < (map_width * map_height) / 19; i++){
    unsigned x = rand() % map_width;
    unsigned y = rand() % map_height;

    if (tile(x, y) == TILE_TREE) continue;
    else tile(x, y) = TILE_TREE;
  }
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

