/*
 *
 * card.h
 *
 * Created at:  Tue Mar  3 17:41:46 2015 17:41:46
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef CARD_H
#define CARD_H

#define MAX_CARD_NAME_LEN 64

struct card {
  char name[MAX_CARD_NAME_LEN + 1];
  int power;
  int life;
};

#endif /* CARD_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

