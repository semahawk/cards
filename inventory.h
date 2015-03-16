/*
 *
 * inventory.h
 *
 * Created at:  Sun Mar  8 16:11:00 2015 16:11:00
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "scene.h"
#include "list.h"
#include "item.h"

struct inv_elem {
  struct item *item;
  SLIST_ENTRY(inv_elem) inv_elem;
};

void add_to_inventory(struct item *);
void dump_inventory(void);

void inventory_init(void);
void inventory_fini(void);
void inventory_open(void);
void inventory_close(void);
void inventory_scene_preswitch(void);
void inventory_scene_render(void);

extern scene_t inventory_scene;

#endif /* INVENTORY_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

