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

#include <SDL/SDL_gfxPrimitives.h>

#include "actor.h"
#include "main.h"
#include "map.h"
#include "event.h"
#include "item.h"
#include "scene.h"
#include "inventory.h"
#include "text.h"
#include "duel.h"
#include "list.h"
#include "projectile.h"

struct chunk *chunks[3][3];

struct tiletype tiletypes[] = {
  [TILE_GRASS]   = { ",", 'k', 0 },
  [TILE_TREE]    = { "&", 'g', 0 },
  [TILE_RIVER]   = { "2", 'b', 0 },
  [TILE_MAGMA]   = { "3", 'r', 0 },
  [TILE_UNKNOWN] = { "?", 'r', TILE_UNPASSABLE }
};

int map_origin_x;
int map_origin_y;

static int center_x;
static int center_y;

static int shown_chunk_x;
static int shown_chunk_y;

static unsigned effect_counter = 0;

int hero_pos_x = 0;
int hero_pos_y = 0;

scene_t map_scene = (scene_t){ map_scene_preswitch, map_scene_render };

struct actor *target = NULL;
static struct position target_cursor_pos;

struct chunk *load_chunk(int x, int y)
{
  struct chunk *n = malloc(sizeof(struct chunk));
  unsigned i, j;

  srand(seed);

  printf("loading chunk (%d,%d)\n", x, y);

  for (i = 0; i < CHUNK_WIDTH; i++)
    for (j = 0; j < CHUNK_HEIGHT; j++)
      n->tiles[i][j] = rand() % 2 ? TILE_TREE : TILE_GRASS;

  printf("smoothing the chunk...\n");

  for (int g = 0; g < 7; g++){
    for (i = 0; i < CHUNK_WIDTH; i++){
      for (j = 0; j < CHUNK_HEIGHT; j++){
        unsigned neighbour_count =
            n->tiles[i-1][j-1] + n->tiles[i][j-1] + n->tiles[i+1][j-1]
          + n->tiles[i-1][j]   + 0                + n->tiles[i+1][j]
          + n->tiles[i-1][j+1] + n->tiles[i][j+1] + n->tiles[i+1][j+1];

        if (neighbour_count > 4)
          n->tiles[i][j] = TILE_TREE;
        else if (neighbour_count < 4)
          n->tiles[i][j] = TILE_GRASS;
      }
    }
  }

  return n;
}

void move_hero_up(void)
{
  /* user stepped over to the northern chunk */
  if (hero_pos_y - 1 < map_origin_y){
    map_origin_y -= CHUNK_HEIGHT;

    printf("crossed the northern chunk boundary\n"
      "new origins = (%d, %d)\n", map_origin_x, map_origin_y);

    printf("removing old chunks...\n");
    fflush(stdout);

    /* free the bottom-most row */
    free(chunks[0][2]);
    free(chunks[1][2]);
    free(chunks[2][2]);

    printf("adjusting present chunks...\n");
    fflush(stdout);

    /* move the top two rows down by one */
    chunks[0][2] = chunks[0][1];
    chunks[1][2] = chunks[1][1];
    chunks[2][2] = chunks[2][1];

    chunks[0][1] = chunks[0][0];
    chunks[1][1] = chunks[1][0];
    chunks[2][1] = chunks[2][0];

    printf("loading new chunks...\n");
    fflush(stdout);

    /* fetch three new chunks at the top row */
    chunks[0][0] = load_chunk(map_origin_x - CHUNK_WIDTH, map_origin_y - CHUNK_HEIGHT);
    chunks[1][0] = load_chunk(map_origin_x, map_origin_y - CHUNK_HEIGHT);
    chunks[2][0] = load_chunk(map_origin_x + CHUNK_WIDTH, map_origin_y - CHUNK_HEIGHT);
  }

  if (is_passable(hero_pos_x, hero_pos_y - 1))
    hero_pos_y--;

  action_issued = true;
}

void move_hero_down(void)
{
  /* user stepped over to the southern chunk */
  if (hero_pos_y + 1 >= map_origin_y + CHUNK_HEIGHT){
    map_origin_y += CHUNK_HEIGHT;

    printf("crossed the southern chunk boundary\n"
      "new origins = (%d, %d)\n", map_origin_x, map_origin_y);

    printf("removing old chunks...\n");
    fflush(stdout);

    /* free the top-most row */
    free(chunks[0][0]);
    free(chunks[1][0]);
    free(chunks[2][0]);

    printf("adjusting present chunks...\n");
    fflush(stdout);

    /* move the two bottom-most rows up by one */
    chunks[0][0] = chunks[0][1];
    chunks[1][0] = chunks[1][1];
    chunks[2][0] = chunks[2][1];

    chunks[0][1] = chunks[0][2];
    chunks[1][1] = chunks[1][2];
    chunks[2][1] = chunks[2][2];

    printf("loading new chunks...\n");
    fflush(stdout);

    /* fetch three new chunks at the bottom row */
    chunks[0][2] = load_chunk(map_origin_x - CHUNK_WIDTH, map_origin_y + CHUNK_HEIGHT);
    chunks[1][2] = load_chunk(map_origin_x, map_origin_y + CHUNK_HEIGHT);
    chunks[2][2] = load_chunk(map_origin_x + CHUNK_WIDTH, map_origin_y + CHUNK_HEIGHT);
  }

  if (is_passable(hero_pos_x, hero_pos_y + 1))
    hero_pos_y++;

  action_issued = true;
}

void move_hero_left(void)
{
  /* user stepped over to the western chunk */
  if (hero_pos_x - 1 < map_origin_x){
    map_origin_x -= CHUNK_WIDTH;

    printf("crossed the western chunk boundary\n"
      "new origins = (%d, %d)\n", map_origin_x, map_origin_y);

    printf("removing old chunks...\n");
    fflush(stdout);

    /* free the right-most row */
    free(chunks[2][0]);
    free(chunks[2][1]);
    free(chunks[2][2]);

    printf("adjusting current chunks...\n");
    fflush(stdout);

    /* move the two left-most columns to the right */
    chunks[2][0] = chunks[1][0];
    chunks[2][1] = chunks[1][1];
    chunks[2][2] = chunks[1][2];

    chunks[1][0] = chunks[0][0];
    chunks[1][1] = chunks[0][1];
    chunks[1][2] = chunks[0][2];

    printf("loading new chunks...\n");
    fflush(stdout);

    /* fetch three new chunks at the top row */
    chunks[0][0] = load_chunk(map_origin_x - CHUNK_WIDTH, map_origin_y - CHUNK_HEIGHT);
    chunks[0][1] = load_chunk(map_origin_x - CHUNK_WIDTH, map_origin_y);
    chunks[0][2] = load_chunk(map_origin_x - CHUNK_WIDTH, map_origin_y + CHUNK_HEIGHT);
  }

  if (is_passable(hero_pos_x - 1, hero_pos_y))
    hero_pos_x--;

  action_issued = true;
}

void move_hero_right(void)
{
  /* user stepped over to the eastern chunk */
  if (hero_pos_x + 1 >= map_origin_x + CHUNK_WIDTH){
    map_origin_x += CHUNK_WIDTH;

    printf("crossed the eastern chunk boundary\n"
      "new origins = (%d, %d)\n", map_origin_x, map_origin_y);

    printf("removing old chunks...\n");
    fflush(stdout);

    /* free the left-most column */
    free(chunks[0][0]);
    free(chunks[0][1]);
    free(chunks[0][2]);

    printf("adjusting present chunks...\n");
    fflush(stdout);

    /* move the right-most two columns to the left */
    chunks[0][0] = chunks[1][0];
    chunks[0][1] = chunks[1][1];
    chunks[0][2] = chunks[1][2];

    chunks[1][0] = chunks[2][0];
    chunks[1][1] = chunks[2][1];
    chunks[1][2] = chunks[2][2];

    printf("loading new chunks...\n");
    fflush(stdout);

    /* fetch three new chunks at the right column */
    chunks[2][0] = load_chunk(map_origin_x + CHUNK_WIDTH, map_origin_y - CHUNK_HEIGHT);
    chunks[2][1] = load_chunk(map_origin_x + CHUNK_WIDTH, map_origin_y);
    chunks[2][2] = load_chunk(map_origin_x + CHUNK_WIDTH, map_origin_y + CHUNK_HEIGHT);
  }

  if (is_passable(hero_pos_x + 1, hero_pos_y))
    hero_pos_x++;

  action_issued = true;
}

void map_draw_tile(tile_t tile, unsigned x, unsigned y)
{
  current_color = tiletypes[tile].color;

  drawch(tiletypes[tile].face[0], x, y);
}

void move_target_left(void)
{
  if (target_cursor_pos.x > 0)
    target_cursor_pos.x--;
}

void move_target_down(void)
{
  if (target_cursor_pos.y < (signed)WINDOW_ROWS - 1)
    target_cursor_pos.y++;
}

void move_target_up(void)
{
  if (target_cursor_pos.y > 0)
    target_cursor_pos.y--;
}

void move_target_right(void)
{
  if (target_cursor_pos.x < (signed)WINDOW_COLS - 1)
    target_cursor_pos.x++;
}

void target_scene_end(void);

void target_set(void)
{
  static struct actor *prev = NULL;
  struct actor *actor;

  SLIST_FOREACH(actor, &rendered_actors, actor){
    if (actor->pos.x == target_cursor_pos.x + shown_chunk_x && actor->pos.y == target_cursor_pos.y + shown_chunk_y){
      target = actor;

      if (prev)
        actor_remove_effect(prev, EFFECT_TARGETED);

      actor_apply_effect(target, EFFECT_TARGETED);

      prev = target;

      target_scene_end();
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

  target_cursor_pos = (struct position){ (signed)WINDOW_COLS / 2, (signed)WINDOW_ROWS / 2 };
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

void pick_item(void)
{
  struct item *item;

  for (int i = 0; i < 10; i++){
    item = &items[i];

    if (item->pos.x == hero_pos_x && item->pos.y == hero_pos_y){
      add_to_inventory(item);
      /* TODO remove the item from the map */
    }
  }
}

void map_scene_preswitch(void)
{
  event_clear_all();

  event_handlers[SDLK_h] = (event_handler_t){ true, move_hero_left };
  event_handlers[SDLK_j] = (event_handler_t){ true, move_hero_down };
  event_handlers[SDLK_k] = (event_handler_t){ true, move_hero_up };
  event_handlers[SDLK_l] = (event_handler_t){ true, move_hero_right };
  event_handlers[SDLK_i] = (event_handler_t){ false, inventory_open };
  event_handlers[SDLK_r] = (event_handler_t){ false, dump_scenes };
  event_handlers[SDLK_d] = (event_handler_t){ false, duel_begin };
  event_handlers[SDLK_t] = (event_handler_t){ false, target_scene_begin };
  event_handlers[SDLK_p] = (event_handler_t){ false, pick_item };
  event_handlers[SDLK_s] = (event_handler_t){ true, shoot_projectile };
  event_handlers[SDLK_ESCAPE] = (event_handler_t){ false, scene_previous };
  event_handlers[SDLK_PERIOD] = (event_handler_t){ true, skip_turn };
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

  /* draw the chunk of the map onto the screen */
  /* starting from 1 to start below the infobar */
  for (int y = 0; y < (signed)WINDOW_ROWS; y++){
    for (int x = 0; x < (signed)WINDOW_COLS; x++){
      map_draw_tile(tile(shown_chunk_x + x, shown_chunk_y + y), x, y);
    }
  }

  struct item item;
  /* draw the items */
  for (unsigned i = 0; i < 10; i++){
    item = items[i];

    /* see if the actor's position is contained within the displayed chunk */
    if ((item.pos.x >= shown_chunk_x && item.pos.x < shown_chunk_x + (signed)WINDOW_COLS) &&
        (item.pos.y >= shown_chunk_y && item.pos.y < shown_chunk_y + (signed)WINDOW_ROWS)){
      unsigned on_the_screen_x = item.pos.x - shown_chunk_x;
      unsigned on_the_screen_y = item.pos.y - shown_chunk_y;

      current_color = item.color;
      drawch(item.face, on_the_screen_x, on_the_screen_y);
    }
  }

  struct actor *actor;
  /* draw the actors */
  SLIST_FOREACH(actor, &rendered_actors, actor){
    /* see if the actor's position is contained within the displayed chunk */
    if ((actor->pos.x >= shown_chunk_x && actor->pos.x < shown_chunk_x + (signed)WINDOW_COLS) &&
        (actor->pos.y >= shown_chunk_y && actor->pos.y < shown_chunk_y + (signed)WINDOW_ROWS)){
      unsigned on_the_screen_x = actor->pos.x - shown_chunk_x;
      unsigned on_the_screen_y = actor->pos.y - shown_chunk_y;

      /* FIXME */
      switch (effect_counter % (actor->effect_num + 1)){
        case EFFECT_NONE:
          current_color = actor->color;
          drawch(actor->face, on_the_screen_x, on_the_screen_y);
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

  x = center_x;
  y = center_y;

  for (int i = 0; i < 10; i++){
    item = items[i];

    if (hero_pos_x == item.pos.x && hero_pos_y == item.pos.y){
      draws(item.name, x - strlen(item.name) / 2, y - 2);
    }
  }

  draws("`b@", x, y);

  /* draw all the projectiles on the map */
  {
    struct projectile *p;

    SLIST_FOREACH(p, &projectiles, projectile){
      unsigned on_the_screen_x = (p->origin_x - shown_chunk_x) * font_width;
      unsigned on_the_screen_y = (p->origin_y - shown_chunk_y) * font_height;

      unsigned x = cos(p->angle) * font_width;
      unsigned y = sin(p->angle) * font_height;

      /*lineRGBA(screen, on_the_screen_x, on_the_screen_y, x + on_the_screen_x, y + on_the_screen_y, 0x00, 0xff, 0x00, 0xff);*/
      current_color = 'w';
      drawch(249, (on_the_screen_x + x) / 8, (on_the_screen_y + y) / 8);
    }
  }
}

void map_init(void)
{
  unsigned x, y, i, j;

  srand(seed);

  center_x = WINDOW_COLS / 2;
  center_y = WINDOW_ROWS / 2;

  hero_pos_x = rand() % CHUNK_WIDTH;
  hero_pos_y = rand() % CHUNK_HEIGHT;

  map_origin_x = 0;
  map_origin_y = 0;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      chunks[i][j] = load_chunk(map_origin_x + (i - 1) * CHUNK_WIDTH,
                                map_origin_y + (j - 1) * CHUNK_HEIGHT);

  actor_new('g', 'y', (struct position){ 3, 3 });

  j = 0;

  for (i = 0; i < 10; i++){
    struct item item;
    x = (rand() % (3 * CHUNK_WIDTH)) - CHUNK_WIDTH;
    y = (rand() % (3 * CHUNK_HEIGHT)) - CHUNK_HEIGHT;

    while (!is_passable(x, y)){
      x = (rand() % (3 * CHUNK_WIDTH)) - CHUNK_WIDTH;
      y = (rand() % (3 * CHUNK_HEIGHT)) - CHUNK_HEIGHT;
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

void map_fini(void)
{
  unsigned i, j;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      free(chunks[i][j]);
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

