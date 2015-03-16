/*-
 * Copyright (c) 1991, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *  @(#)queue.h 8.5 (Berkeley) 8/20/94
 * $FreeBSD: release/9.1.0/sys/sys/queue.h 221843 2011-05-13 15:49:23Z mdf $
 */

#ifndef LIST_H
#define LIST_H

/*
 * Singly-linked List declarations.
 */
#define SLIST_HEAD(name, type)            \
struct name {               \
  struct type *slh_first; /* first element */     \
}

#define SLIST_HEAD_INITIALIZER(head)          \
  { NULL }

#define SLIST_ENTRY(type)           \
struct {                \
  struct type *sle_next;  /* next element */      \
}

/*
 * Singly-linked List functions.
 */
#define SLIST_EMPTY(head) ((head)->slh_first == NULL)

#define SLIST_FIRST(head) ((head)->slh_first)

#define SLIST_FOREACH(var, head, field)         \
  for ((var) = SLIST_FIRST((head));       \
      (var);              \
      (var) = SLIST_NEXT((var), field))

#define SLIST_FOREACH_SAFE(var, head, field, tvar)      \
  for ((var) = SLIST_FIRST((head));       \
      (var) && ((tvar) = SLIST_NEXT((var), field), 1);    \
      (var) = (tvar))

#define SLIST_FOREACH_PREVPTR(var, varp, head, field)     \
  for ((varp) = &SLIST_FIRST((head));       \
      ((var) = *(varp)) != NULL;          \
      (varp) = &SLIST_NEXT((var), field))

#define SLIST_INIT(head) do {           \
  SLIST_FIRST((head)) = NULL;         \
} while (0)

#define SLIST_INSERT_AFTER(slistelm, elm, field) do {     \
  SLIST_NEXT((elm), field) = SLIST_NEXT((slistelm), field); \
  SLIST_NEXT((slistelm), field) = (elm);        \
} while (0)

#define SLIST_INSERT_HEAD(head, elm, field) do {      \
  SLIST_NEXT((elm), field) = SLIST_FIRST((head));     \
  SLIST_FIRST((head)) = (elm);          \
} while (0)

#define SLIST_NEXT(elm, field)  ((elm)->field.sle_next)

#define SLIST_REMOVE(head, elm, type, field) do {     \
  QMD_SAVELINK(oldnext, (elm)->field.sle_next);     \
  if (SLIST_FIRST((head)) == (elm)) {       \
    SLIST_REMOVE_HEAD((head), field);     \
  }               \
  else {                \
    struct type *curelm = SLIST_FIRST((head));    \
    while (SLIST_NEXT(curelm, field) != (elm))    \
      curelm = SLIST_NEXT(curelm, field);   \
    SLIST_REMOVE_AFTER(curelm, field);      \
  }               \
  TRASHIT(*oldnext);            \
} while (0)

#define SLIST_REMOVE_AFTER(elm, field) do {       \
  SLIST_NEXT(elm, field) =          \
      SLIST_NEXT(SLIST_NEXT(elm, field), field);      \
} while (0)

#define SLIST_REMOVE_HEAD(head, field) do {       \
  SLIST_FIRST((head)) = SLIST_NEXT(SLIST_FIRST((head)), field); \
} while (0)

#define SLIST_SWAP(head1, head2, type) do {       \
  struct type *swap_first = SLIST_FIRST(head1);     \
  SLIST_FIRST(head1) = SLIST_FIRST(head2);      \
  SLIST_FIRST(head2) = swap_first;        \
} while (0)

#endif /* LIST_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

