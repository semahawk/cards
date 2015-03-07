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
#include <stdbool.h>

#include "actor.h"
#include "main.h"
#include "map.h"
#include "text.h"

/* a one dimensional array */
tile_t *map_tiles;

static unsigned center_x;
static unsigned center_y;

unsigned hero_pos_x = 0;
unsigned hero_pos_y = 0;

unsigned map_width = 128;
unsigned map_height = 64;

void move_hero_up(void)
{
  if (is_passable(hero_pos_x, hero_pos_y - 1))
    hero_pos_y--;
}

void move_hero_down(void)
{
  if (is_passable(hero_pos_x, hero_pos_y + 1))
    hero_pos_y++;
}

void move_hero_left(void)
{
  if (is_passable(hero_pos_x - 1, hero_pos_y))
    hero_pos_x--;
}

void move_hero_right(void)
{
  if (is_passable(hero_pos_x + 1, hero_pos_y))
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
    case TILE_ACTOR:
      draws("`y@", x, y);
      break;
    default:
      draws("`r?", x, y);
      break;
  }
}

void map_render(void)
{
  unsigned x, y;

  /* default top-left coordinate of the map's chunk we want to display */
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
  /* starting from 1 to start below the infobar */
  for (unsigned y = 1; y < WINDOW_ROWS && y < map_height; y++){
    for (unsigned x = 0; x < WINDOW_COLS && x < map_width; x++){
      map_draw_tile(tile(shown_chunk_x + x, shown_chunk_y + y), x, y);
    }
  }

  struct actor actor;
  /* draw the actors */
  for (unsigned i = 0; i < 10; i++){
    actor = actors[i];

    /* see if the actor's position is contained within the displayed chunk */
    if ((actor.pos.x >= shown_chunk_x && actor.pos.x < shown_chunk_x + WINDOW_COLS) &&
        (actor.pos.y >= shown_chunk_y && actor.pos.y < shown_chunk_y + WINDOW_ROWS)){
      unsigned on_the_screen_x = actor.pos.x - shown_chunk_x;
      unsigned on_the_screen_y = actor.pos.y - shown_chunk_y;

      current_color = actor.color;
      drawch(actor.face, on_the_screen_x, on_the_screen_y);
    }
  }

  /* keep the hero in the center of the screen, or draw him to the borders when
   * close */
  /* getting close to the left border */
  if (hero_pos_x < center_x)
    x = hero_pos_x;
  /* getting close to the right border */
  else if (hero_pos_x > map_width - center_x)
    x = WINDOW_COLS - (map_width - hero_pos_x);
  /* far from either */
  else
    x = center_x;

  /* getting close to the top border */
  if (hero_pos_y < center_y)
    y = hero_pos_y;
  /* getting close to the bottom border */
  else if (hero_pos_y > map_height - center_y)
    y = WINDOW_ROWS - (map_height - hero_pos_y);
  /* far from either */
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

  printf("allocating a %ux%u (%d KiB) map\n", map_width, map_height, map_width * map_height);

  if (!map_tiles){
    fprintf(stderr, "couldn't allocate memory for the map\n");
    exit(1);
  }

  /* fill the entire map with grass */
  for (unsigned x = 0; x < map_width; x++){
    for (unsigned y = 0; y < map_height; y++){
      tile(x, y) = TILE_GRASS;
    }
  }

  /* make some of the tiles be trees */
  for (unsigned i = 0; i < (map_width * map_height) / 73; i++){
    unsigned x = rand() % map_width;
    unsigned y = rand() % map_height;

    if (tile(x, y) == TILE_TREE) continue;
    else tile(x, y) = TILE_TREE;
  }

  {
    unsigned i = 0, j = 0;

    for (; i < 10; i++){
      struct actor actor;
      unsigned x = rand() % map_width;
      unsigned y = rand() % map_height;

      while (tile(x, y) & TILE_UNPASSABLE){
        x = rand() % map_width;
        y = rand() % map_height;
      }

      actor.face = '@';
      actor.color = 'y';
      actor.pos.x = x;
      actor.pos.y = y;

      actors[j++] = actor;
    }
  }
}

void map_fini(void)
{
  free(map_tiles);
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

