/*
 *
 * event.c
 *
 * Created at:  Sun Mar  8 21:10:22 2015 21:10:22
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>

#include "main.h"
#include "map.h"
#include "item.h"
#include "inventory.h"
#include "event.h"

static int numkeys;

event_handler_t *event_handlers;

void handle_events()
{
  SDL_Event event;

  SDL_PollEvent(&event);

  Uint8* keystate = SDL_GetKeyState(NULL);

  for (int i = 0; i < numkeys; i++){
    if (keystate[i]){
      if (event_handlers[i].handler != NULL && event_handlers[i].continuous){
        /*printf("[%d - %s](%p) ", i, SDL_GetKeyName(event.key.keysym.sym), event_handlers[i]);*/
        /*fflush(stdout);*/
        event_handlers[i].handler();
      }
    }
  }

  if (event.type == SDL_KEYDOWN){
    for (int i = 0; i < numkeys; i++){
      if (event.key.keysym.sym == i &&
          event_handlers[i].handler != NULL &&
          !event_handlers[i].continuous){
        event_handlers[i].handler();
      }
    }
  }
}

void event_clear_all(void)
{
  memset(event_handlers, 0x0, sizeof(event_handler_t) * numkeys);
}

void event_init(void)
{
  SDL_GetKeyState(&numkeys);

  size_t size = sizeof(event_handler_t) * numkeys;

  event_handlers = malloc(size);

  if (!event_handlers){
    fprintf(stderr, "couldn't allocate %d bytes of memory\n", size);
    exit(1);
  }

  event_clear_all();
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

