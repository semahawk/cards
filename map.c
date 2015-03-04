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

static unsigned center_x;
static unsigned center_y;

unsigned hero_pos_x = 0;
unsigned hero_pos_y = 0;

static unsigned map_width = 120;
static unsigned map_height = 57;

void move_hero_up(void)
{
  if (hero_pos_y > 0)
    hero_pos_y--;
}

void move_hero_down(void)
{
  if (hero_pos_y < map_height - 1)
    hero_pos_y++;
}

void move_hero_left(void)
{
  if (hero_pos_x > 0)
    hero_pos_x--;
}

void move_hero_right(void)
{
  if (hero_pos_x < map_width - 1)
    hero_pos_x++;
}

void map_render(void)
{
  unsigned x, y;

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

  drawch(1, x, y);
}

void map_init(void)
{
  center_x = WINDOW_COLS / 2;
  center_y = WINDOW_ROWS / 2;

  hero_pos_x = map_width / 2;
  hero_pos_y = map_height / 2;
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

