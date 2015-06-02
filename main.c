/*
 *
 * main.c
 *
 * Created at:  Tue Mar  3 02/03/2015 16:30:53 PM
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License: the MIT license
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <SDL/SDL.h>

#include "actor.h"
#include "event.h"
#include "duel.h"
#include "hero.h"
#include "text.h"
#include "inventory.h"
#include "main.h"
#include "map.h"
#include "scene.h"
#include "stack.h"
#include "list.h"
#include "projectile.h"

struct hero the_hero;

unsigned WINDOW_COLS = 96, WINDOW_ROWS = 96;

float frames_per_second;

SDL_Surface *screen = NULL;

bool running = true;
bool action_issued  = false;
unsigned turn = 0;

uint32_t seed = 0;

void main_menu_resume(void);

static struct main_menu_option {
  const char *text;
  void (*func)(void);
} main_menu_options[] = {
  { "Resume", main_menu_resume },
  { "Quit", stop_running },
};

static unsigned main_menu_current_option = 0;

void cap_frame_rate(void)
{
  /* {{{ */
  static unsigned wait_time = 1000.0f / FPS_CAP;
  static unsigned frame_start_time = 0;
  static int delay_time;

  delay_time = wait_time - (SDL_GetTicks() - frame_start_time);

  if (delay_time > 0)
    SDL_Delay((unsigned)delay_time);

  frame_start_time = SDL_GetTicks();
  /* }}} */
}

void count_fps(void)
{
  /* {{{ */
  static float alpha = 0.05;
  static Uint32 get_ticks, frame_time_delta, frame_time_last;
  static float frame_time;

  get_ticks = SDL_GetTicks();
  frame_time_delta = get_ticks - frame_time_last;
  frame_time_last = get_ticks;
  frame_time = alpha * frame_time_delta + (1.0 - alpha) * frame_time;
  frames_per_second = 1000.0 / frame_time;
  /* }}} */
}

void draw_infobar(void)
{
  /* {{{ */
  int x = 0;

  count_fps();

  draws("`bfps `c", x, 0);
  x += 5;
  draws("   ", x, 0);

  drawd((int)frames_per_second, x, 0);
  x += 4;

  draws("`bx:`c", x, 0);
  x += 6;
  draws("   ", x, 0);

  drawd(hero_pos_x, x, 0);
  x += 5;

  draws("`by:`c", x, 0);
  x += 6;
  draws("   ", x, 0);

  drawd(hero_pos_y, x, 0);
  x += 4;

  draws("`bturn:`c", x, 0);
  x += 8;
  drawd(turn, x, 0);
  /* }}} */
}

void stop_running(void)
{
  running = false;
}

void skip_turn(void)
{
  action_issued = true;
}

void next_turn(void)
{
  update_projectiles();
  update_actors();

  turn++;
}

void main_menu_resume(void)
{
  scene_setnew(map_scene);
}

void main_menu_option_next(void)
{
  if (main_menu_current_option < ARRAY_SIZE(main_menu_options) - 1)
    main_menu_current_option++;
}

void main_menu_option_prev(void)
{
  if (main_menu_current_option > 0)
    main_menu_current_option--;
}

void main_menu_select_option(void)
{
  main_menu_options[main_menu_current_option].func();
}

void main_menu_scene_preswitch(void)
{
  event_clear_all();

  event_handlers[SDLK_j] = (event_handler_t){ false, main_menu_option_next };
  event_handlers[SDLK_k] = (event_handler_t){ false, main_menu_option_prev };
  event_handlers[SDLK_RETURN] = (event_handler_t){ false, main_menu_select_option };
  event_handlers[SDLK_ESCAPE] = (event_handler_t){ false, stop_running };
}

void main_menu_scene_render(void)
{
  unsigned y = 6, i;

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  for (i = 0; i < ARRAY_SIZE(main_menu_options); i++){
    if (i == main_menu_current_option)
      current_color = 'y';
    else
      current_color = 'b';

    draws(main_menu_options[i].text, WINDOW_COLS / 2 - strlen(main_menu_options[i].text) / 2, y++);
  }
}

scene_t main_menu_scene = (scene_t){ main_menu_scene_preswitch, main_menu_scene_render };

int main(int argc, char *argv[])
{
  SLIST_INIT(&the_hero.inv);

  /* suspress warnings */
  (void)argc;
  (void)argv;

  SDL_Init(SDL_INIT_EVERYTHING);

  srand(time(NULL));

  seed = rand();
  seed = 0xdeadbeef;

  printf("seed 0x%8x\n", seed);

  text_init1("./font.bmp");
  screen = SDL_SetVideoMode(WINDOW_COLS * font_width, WINDOW_ROWS * font_height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  text_init2();

  event_init();
  scene_init();
  map_init();

  scene_setnew(main_menu_scene);
  scene_setnew(map_scene);

  (STACK_TOP(scenes)).render();

  draw_infobar();
  inventory_init();

  SDL_Flip(screen);

  Uint32 next_game_tick = SDL_GetTicks();
  int loops;

  while (running){
    loops = 0;

    while (SDL_GetTicks() > next_game_tick && loops < MAX_FRAMESKIP){
      handle_events();

      if (action_issued)
        next_turn();

      action_issued = false;

      next_game_tick += 1000 / GAME_SPEED;
      loops++;
    }

    (STACK_TOP(scenes)).render();

    draw_infobar();
    SDL_Flip(screen);

    cap_frame_rate();
  }

  inventory_fini();
  map_fini();
  text_fini();

  atexit(SDL_Quit);

  return 0;
}

