/*
 *
 * duel.c
 *
 * Created at:  Fri Mar 13 15:29:45 2015 15:29:45
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>

#include "card.h"
#include "duel.h"
#include "event.h"
#include "main.h"
#include "scene.h"
#include "text.h"

scene_t duel_scene = (scene_t){ duel_scene_preswitch, duel_scene_render };

void duel_draw_full_card(void);

void duel_begin(void)
{
  scene_setnew(duel_scene);
}

void duel_end(void)
{
  scene_previous();
}

static bool update = true;
static bool draw_card = false;

void duel_switch_draw_full_card(void)
{
  draw_card = !draw_card;

  update = true;
}

void duel_scene_preswitch(void)
{
  event_clear_all();

  event_handlers[SDLK_ESCAPE] = (event_handler_t){ false, duel_end };
  event_handlers[SDLK_c] = (event_handler_t){ false, duel_switch_draw_full_card };
  event_handlers[SDLK_r] = (event_handler_t){ false, dump_scenes };

  update = true;
}

void duel_draw_full_card(void)
{
  unsigned x = WINDOW_COLS / 2 - CARD_FULL_WIDTH  / 2,
           y = WINDOW_ROWS / 2 - CARD_FULL_HEIGHT / 2;

  unsigned i, j;

  current_color = 'w';

  /* black out the card's background */
  for (i = 1; i < CARD_FULL_WIDTH; i++)
    for (j = 1; j < CARD_FULL_HEIGHT; j++)
      drawch(' ', x + i, y + j);

  /* draw the four corners */
  drawch(CARD_TOP_LEFT, x, y);
  drawch(CARD_TOP_RIGHT, x + CARD_FULL_WIDTH, y);
  drawch(CARD_BOTTOM_LEFT, x, y + CARD_FULL_HEIGHT);
  drawch(CARD_BOTTOM_RIGHT, x + CARD_FULL_WIDTH, y + CARD_FULL_HEIGHT);

  /* draw the creature's name */
  draws("Generic Monster", x + 1, y + 1);

  for (i = 1; i < CARD_FULL_WIDTH; i++){
    /* draw the top edge */
    drawch(CARD_HORZ_EDGE, x + i, y);
    /* draw the bottom edge */
    drawch(CARD_HORZ_EDGE, x + i, y + CARD_FULL_HEIGHT);
    /* draw the line under the creature's name */
    drawch(CARD_HORZ_EDGE, x + i, y + 2);
    /* draw the line above the creature's stats */
    drawch(CARD_HORZ_EDGE, x + i, y + (CARD_FULL_HEIGHT - 2));
  }

  for (i = 1; i < CARD_FULL_HEIGHT; i++){
    /* draw the left edge */
    drawch(CARD_VERT_EDGE, x, y + i);
    /* draw the right edge */
    drawch(CARD_VERT_EDGE, x + CARD_FULL_WIDTH, y + i);
  }

  /* draw the joints of the line under the creature's name */
  drawch(CARD_JOINT_TRB, x, y + 2);
  drawch(CARD_JOINT_LTB, x + CARD_FULL_WIDTH, y + 2);

  /* draw the joints of the line above the creature's stats */
  drawch(CARD_JOINT_TRB, x, y + (CARD_FULL_HEIGHT - 2));
  drawch(CARD_JOINT_LTB, x + CARD_FULL_WIDTH, y + (CARD_FULL_HEIGHT - 2));

  /* draw the stats */
  draws("`g12`k/`b09`k/`y12`k/`r13", x + 1, y + (CARD_FULL_HEIGHT - 1));

  current_color = 'w';
  /* throw in some random text */
  for (i = 1; i < CARD_FULL_WIDTH; i++)
    for (j = 3; j < CARD_FULL_HEIGHT - 2; j++)
      drawch(rand() % 3 ? (rand() % 26 + 'a') : ' ', x + i, y + j);
}

void duel_draw_sheathed_card_in_hand(unsigned x, unsigned y)
{
  current_color = 'w';

  draws("GnrcM", x, y);
  draws("Mnstr", x, y + 1);
  draws("`g12`k/`r13", x, y + 2);
  draws("`b 9`k/`y12", x, y + 3);
}

void duel_draw_hand(void)
{
  unsigned x = 16 + 4 * (CARD_WIDTH + 2), y = WINDOW_ROWS - 5;

  for (unsigned i = 0; i < 12; i++){
    duel_draw_sheathed_card_in_hand(i * (CARD_WIDTH + 2) + x, y);
  }
}

void duel_draw_card(struct card *card, unsigned x, unsigned y)
{
  /* suspress a warning */
  (void)card;

  current_color = 'w';

  draws("GnrcM", x, y);
  draws("nstr ", x, y + 1);

  current_color = 'k';

  draws(".....", x, y + 2);
  draws(".....", x, y + 3);
  draws(".....", x, y + 4);
  draws(".....", x, y + 5);

  draws("`g12`k/`r11", x, y + 6);
  draws("`b 9`k/`y12", x, y + 7);
}

void duel_draw_battlefield(void)
{
  unsigned i;
  unsigned x = 2, y = WINDOW_ROWS / 2 + 2;

  for (i = 0; i < 18; i++)
    duel_draw_card(NULL, i * (CARD_WIDTH + 2) + x, y);

  y += CARD_HEIGHT + 2;

  for (i = 0; i < 18; i++)
    duel_draw_card(NULL, i * (CARD_WIDTH + 2) + x, y);

  y += CARD_HEIGHT + 2;

  for (i = 0; i < 18; i++)
    duel_draw_card(NULL, i * (CARD_WIDTH + 2) + x, y);

  x = 2;
  y = WINDOW_ROWS / 2 - CARD_HEIGHT - 2;

  for (i = 0; i < 18; i++)
    duel_draw_card(NULL, i * (CARD_WIDTH + 2) + x, y);

  y -= CARD_HEIGHT + 2;

  for (i = 0; i < 18; i++)
    duel_draw_card(NULL, i * (CARD_WIDTH + 2) + x, y);

  y -= CARD_HEIGHT + 2;

  for (i = 0; i < 18; i++)
    duel_draw_card(NULL, i * (CARD_WIDTH + 2) + x, y);
}

void duel_draw_hero_stats(void)
{
  unsigned x = 2, y = WINDOW_ROWS - 7;
  unsigned i;

  /* draw the user's nick */
  draws("`wveryveryveryveryveryveryverylongusername", x, y);

  /* draw the counts (cards in hand, in the deck, in the graveyard) */
  x = 30;
  y = WINDOW_ROWS - 5;

  drawch(235, x, y);
  drawd(12, x + 1, y++);

  drawch(234, x, y);
  drawd(37, x + 1, y++);

  drawch(237, x, y);
  drawd(0, x + 1, y++);

  /* draw the mana count */
  x = 35;
  y = WINDOW_ROWS - 4;

  drawd(10, x, y);
  drawch('/', x += 2, y);
  drawd(12, ++x, y);

  /* draw all the vitality bars */
  x = 5;
  y = WINDOW_ROWS - 5;

  /* draw the health bar */
  current_color = 'r';
  drawd(12, x - 3, y);
  for (i = 0; i < 24; i++){
    drawch(219, x + i, y);
  }

  y += 1;

  /* draw the 'second' bar */
  current_color = 'y';
  drawd(12, x - 3, y);
  for (i = 0; i < 24; i++){
    drawch(219, x + i, y);
  }

  y += 1;

  /* draw the 'third' bar */
  current_color = 'b';
  drawd(12, x - 3, y);
  for (i = 0; i < 24; i++){
    drawch(219, x + i, y);
  }



  x = WINDOW_COLS - 2 - 40, y = 6;

  draws("`wveryveryveryveryveryveryverylongusername", x, y);

  x = WINDOW_COLS - 2 - 24 - 4 - 5;
  y = 2;

  drawch(235, x, y);
  drawd(12, x + 1, y++);

  drawch(234, x, y);
  drawd(37, x + 1, y++);

  drawch(237, x, y);
  drawd(0, x + 1, y++);

  /* draw the mana count */
  x -= 7;
  y = 3;

  drawd(10, x, y);
  drawch('/', x += 2, y);
  drawd(12, ++x, y);

  x = WINDOW_COLS - 30;
  y = 2;

  /* draw the health bar */
  current_color = 'r';
  drawd(12, x + 26, y);
  for (i = 0; i < 24; i++){
    drawch(219, x + i, y);
  }

  y += 1;

  /* draw the 'second' bar */
  current_color = 'y';
  drawd(12, x + 26, y);
  for (i = 0; i < 24; i++){
    drawch(219, x + i, y);
  }

  y += 1;

  /* draw the 'third' bar */
  current_color = 'b';
  drawd(12, x + 26, y);
  for (i = 0; i < 24; i++){
    drawch(219, x + i, y);
  }
}

void duel_scene_render(void)
{
  if (!update)
    return;

  update = false;

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  duel_draw_hero_stats();
  duel_draw_battlefield();
  duel_draw_hand();

  current_color = 'k';

  /* draw the horizontal line in the middle */
  for (unsigned x = 0; x < WINDOW_COLS; x++)
    drawch(196, x, WINDOW_ROWS / 2);

  unsigned x = 1, y = WINDOW_ROWS / 2;

  current_color = 'k';
  draws(" Phase 1 ", x, y);
  draws(" Phase 2 ", x += 10, y);
  current_color = 'y';
  draws(" Phase 3 ", x += 10, y);
  current_color = 'k';
  draws(" Phase 4 ", x += 10, y);
  draws(" Phase 5 ", x += 10, y);
  draws(" Phase 6 ", x += 10, y);

  if (draw_card)
    duel_draw_full_card();
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

