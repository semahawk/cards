/*
 *
 * event.h
 *
 * Created at:  Sun Mar  8 21:10:32 2015 21:10:32
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>

void handle_events(void);
void event_init(void);
void event_clear_all(void);

typedef struct {
  bool continuous;
  void (*handler)(void);
} event_handler_t;

extern event_handler_t *event_handlers;

#endif /* EVENT_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

