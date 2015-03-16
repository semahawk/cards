/*
 *
 * hero.h
 *
 * Created at:  Tue Mar  3 17:47:38 2015 17:47:38
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef HERO_H
#define HERO_H

#include "list.h"
#include "inventory.h"
#include "item.h"

#define MAX_HERO_NAME_LEN 32

struct hero {
  char name[MAX_HERO_NAME_LEN + 1];
  SLIST_HEAD(, inv_elem) inv;
  struct position pos;
};

#endif /* HERO_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

