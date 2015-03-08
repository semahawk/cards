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

#include "item.h"

bool add_to_inventory(struct item);
void dump_inventory(void);

#endif /* INVENTORY_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

