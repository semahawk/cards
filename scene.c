/*
 *
 * scene.c
 *
 * Created at:  Mon Mar  9 17:09:09 2015 17:09:09
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "scene.h"
#include "stack.h"
#include "event.h"
#include "inventory.h"

void scene_init(void)
{
  STACK_DEFINE(scenes, 5);
}

void scene_setnew(scene_t scene)
{
  scene.preswitch();

  STACK_PUSH(scenes, scene);
}

void scene_previous(void)
{
  if (!STACK_EMPTY(scenes)){
    (void)STACK_POP(scenes);
  }

  (STACK_TOP(scenes)).preswitch();
}

void dump_scenes(void)
{
  printf("scenes: ");

  STACK_ITER(scenes, scene_t, r){
    printf("%p ", r);
  }

  printf("\n");
}

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

