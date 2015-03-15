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
#include "event.h"
#include "item.h"
#include "scene.h"
#include "inventory.h"
#include "text.h"
#include "duel.h"

/* a one dimensional array */
tile_t *map_tiles;

static unsigned center_x;
static unsigned center_y;

static unsigned shown_chunk_x;
static unsigned shown_chunk_y;

static unsigned effect_counter = 0;

unsigned hero_pos_x = 0;
unsigned hero_pos_y = 0;

unsigned map_width = 256;
unsigned map_height = 256;

scene_t map_scene = (scene_t){ map_scene_preswitch, map_scene_render };

void move_hero_up(void)
{
  if (is_passable(hero_pos_x, hero_pos_y - 1))
    hero_pos_y--;

  action_issued = true;
}

void move_hero_down(void)
{
  if (is_passable(hero_pos_x, hero_pos_y + 1))
    hero_pos_y++;

  action_issued = true;
}

void move_hero_left(void)
{
  if (is_passable(hero_pos_x - 1, hero_pos_y))
    hero_pos_x--;

  action_issued = true;
}

void move_hero_right(void)
{
  if (is_passable(hero_pos_x + 1, hero_pos_y))
    hero_pos_x++;

  action_issued = true;
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

static struct actor *target = NULL;
static struct position target_cursor_pos;

void move_target_left(void)
{
  if (target_cursor_pos.x > 0)
    target_cursor_pos.x--;
}

void move_target_down(void)
{
  if (target_cursor_pos.y < WINDOW_ROWS - 1)
    target_cursor_pos.y++;
}

void move_target_up(void)
{
  if (target_cursor_pos.y > 0)
    target_cursor_pos.y--;
}

void move_target_right(void)
{
  if (target_cursor_pos.x < WINDOW_COLS - 1)
    target_cursor_pos.x++;
}

void target_scene_end(void);

void target_set(void)
{
  static struct actor *prev = NULL;

  for (unsigned i = 0; i < 10; i++){
    if (actors[i].pos.x == target_cursor_pos.x + shown_chunk_x && actors[i].pos.y == target_cursor_pos.y + shown_chunk_y){
      target = &actors[i];

      if (prev)
        actor_remove_effect(prev, EFFECT_TARGETED);

      actor_apply_effect(target, EFFECT_TARGETED);

      prev = target;
    }
  }
}

void target_scene_preswitch(void)
{
  event_clear_all();

  event_handlers[SDLK_h] = (event_handler_t){ true, move_target_left };
  event_handlers[SDLK_j] = (event_handler_t){ true, move_target_down };
  event_handlers[SDLK_k] = (event_handler_t){ true, move_target_up };
  event_handlers[SDLK_l] = (event_handler_t){ true, move_target_right };
  event_handlers[SDLK_t] = (event_handler_t){ false, target_set };
  event_handlers[SDLK_ESCAPE] = (event_handler_t){ false, target_scene_end };

  target_cursor_pos = (struct position){ WINDOW_COLS / 2, WINDOW_ROWS / 2 };
}

void target_scene_render(void)
{
  map_scene_render();

  current_color = 'y';
  drawch('X', target_cursor_pos.x, target_cursor_pos.y);
}

scene_t target_scene = (scene_t){ target_scene_preswitch, target_scene_render };

void target_scene_begin(void)
{
  scene_setnew(target_scene);
}

void target_scene_end(void)
{
  scene_previous();
}

void map_scene_preswitch(void)
{
  event_clear_all();

  event_handlers[SDLK_ESCAPE] = (event_handler_t){ false, stop_running };
  event_handlers[SDLK_h] = (event_handler_t){ true, move_hero_left };
  event_handlers[SDLK_j] = (event_handler_t){ true, move_hero_down };
  event_handlers[SDLK_k] = (event_handler_t){ true, move_hero_up };
  event_handlers[SDLK_l] = (event_handler_t){ true, move_hero_right };
  event_handlers[SDLK_i] = (event_handler_t){ false, inventory_open };
  event_handlers[SDLK_r] = (event_handler_t){ false, dump_scenes };
  event_handlers[SDLK_PERIOD] = (event_handler_t){ true, next_turn };
  event_handlers[SDLK_d] = (event_handler_t){ false, duel_begin };
  event_handlers[SDLK_t] = (event_handler_t){ false, target_scene_begin };
}

void map_scene_render(void)
{
  static unsigned frames = 0;
  unsigned x, y;

  if (++frames % 15 == 0)
    effect_counter++;

  /* default top-left coordinate of the map's chunk we want to display */
  shown_chunk_x = hero_pos_x - center_x;
  shown_chunk_y = hero_pos_y - center_y;

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
  for (unsigned y = 0; y < WINDOW_ROWS && y < map_height; y++){
    for (unsigned x = 0; x < WINDOW_COLS && x < map_width; x++){
      map_draw_tile(tile(shown_chunk_x + x, shown_chunk_y + y), x, y);
    }
  }

  struct item item;
  /* draw the items */
  for (unsigned i = 0; i < 10; i++){
    item = items[i];

    /* see if the actor's position is contained within the displayed chunk */
    if ((item.pos.x >= shown_chunk_x && item.pos.x < shown_chunk_x + WINDOW_COLS) &&
        (item.pos.y >= shown_chunk_y && item.pos.y < shown_chunk_y + WINDOW_ROWS)){
      unsigned on_the_screen_x = item.pos.x - shown_chunk_x;
      unsigned on_the_screen_y = item.pos.y - shown_chunk_y;

      current_color = item.color;
      drawch(item.face, on_the_screen_x, on_the_screen_y);
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

      switch (effect_counter % (actor.effect_num + 1)){
        case EFFECT_NONE:
          current_color = actor.color;
          drawch(actor.face, on_the_screen_x, on_the_screen_y);
          break;
        case EFFECT_TARGETED:
          current_color = 'c';
          drawch('X', on_the_screen_x, on_the_screen_y);
          break;
        case EFFECT_BURNING:
          current_color = 'r';
          drawch('~', on_the_screen_x, on_the_screen_y);
          break;
      }
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

  for (int i = 0; i < 10; i++){
    item = items[i];

    if (hero_pos_x == item.pos.x && hero_pos_y == item.pos.y){
      draws(item.name, x - strlen(item.name) / 2, y - 2);
    }
  }

  draws("`b@", x, y);
}

void map_init(void)
{
  center_x = WINDOW_COLS / 2;
  center_y = WINDOW_ROWS / 2;

  hero_pos_x = map_width / 2;
  hero_pos_y = map_height / 2;

  map_tiles = malloc(sizeof(tile_t) * map_width * map_height);

  printf("allocating a %ux%u (%d bytes) map\n", map_width, map_height,
      sizeof(tile_t) * map_width * map_height);

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

      while (!is_passable(x, y)){
        x = rand() % map_width;
        y = rand() % map_height;
      }

      actor.face = 'g';
      actor.color = 'y';
      actor.pos.x = x;
      actor.pos.y = y;

      actor.effect_num = 0;
      memset(actor.effects, EFFECT_NONE, sizeof(actor.effects));

      actors[j++] = actor;
    }
  }

  {
    unsigned i = 0, j = 0;

    for (; i < 10; i++){
      struct item item;
      unsigned x = rand() % map_width;
      unsigned y = rand() % map_height;

      while (!is_passable(x, y)){
        x = rand() % map_width;
        y = rand() % map_height;
      }

      unsigned choice = rand() % 3;
      strcpy(item.name, (char *[]){"Sword", "Axe", "Wooden Stick"}[choice]);
      item.face  = (char []){'!', 20, '/'}[choice];
      item.color = (char []){'y', 'r', 'w'}[choice];
      item.pos.x = x;
      item.pos.y = y;

      items[j++] = item;
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

