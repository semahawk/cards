/*
 *
 * item.h
 *
 * Created at:  Sun Mar  8 00:02:29 2015 00:02:29
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef ITEM_H
#define ITEM_H

#include "main.h"

struct item {
  char name[16];
  char face;
  char color;
  struct position pos;
};

extern struct item items[10];

#endif /* ITEM_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

