/*
 *
 * stack.h
 *
 * Created at:  Sun Mar  8 17:03:50 2015 17:03:50
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef STACK_H
#define STACK_H

#define STACK_DECLARE(name, type)        \
  struct {          \
    unsigned nmemb;                      \
    type *curr;                          \
    /* an array of stack slots */        \
    type *slots;                         \
  }                                      \
                                         \
  var_##name##_stack

#define STACK_DECLARE_EXTERN(name, type) \
  extern struct {          \
    unsigned nmemb;                      \
    type *curr;                          \
    /* an array of stack slots */        \
    type *slots;                         \
  }                                      \
                                         \
  var_##name##_stack

#define STACK_DEFINE(name, size)                                              \
  do {                                                                        \
    var_##name##_stack.nmemb = size;                                          \
    var_##name##_stack.slots = malloc(sizeof(*var_##name##_stack.slots)*size);\
    var_##name##_stack.curr = var_##name##_stack.slots;                       \
                                                                              \
    if (var_##name##_stack.slots == NULL){                                    \
      fprintf(stderr, "couldn't allocate %d bytes of memory, aborting\n",     \
          sizeof(*var_##name##_stack.slots) * size);                          \
      exit(1);                                                                \
    }                                                                         \
  } while (0)

#define STACK_NEW(name, type, size)       \
  STACK_DECLARE(name, type);              \
  STACK_DEFINE(name, size);

#define STACK_FINI(name)                  \
  do {                                    \
    free(var_##name##_stack.slots);       \
  } while (0)

/* TODO handle overflow */
#define STACK_PUSH(name, value)           \
  do {                                    \
    *(var_##name##_stack.curr++) = value; \
  } while (0);

/* TODO handle underflow */
#define STACK_POP(name) \
  (*(--var_##name##_stack.curr))

#define STACK_TOP(name) \
  (*(var_##name##_stack.curr - 1))

#define STACK_EMPTY(name) \
  (var_##name##_stack.curr == var_##name##_stack.slots)

#define STACK_ITER(name, type, p) \
  for (type *p = var_##name##_stack.slots; p != var_##name##_stack.curr; p++)

#endif /* STACK_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

