/*
 *
 * duel.h
 *
 * Created at:  Fri Mar 13 15:45:38 2015 15:45:38
 *
 * Author:  Szymon Urbaś <szymon.urbas@aol.com>
 *
 * License:  please visit the LICENSE file for details.
 *
 */

#ifndef DUEL_H
#define DUEL_H

#include "scene.h"

#define CARD_TOP_LEFT     218
#define CARD_BOTTOM_RIGHT 217
#define CARD_TOP_RIGHT    191
#define CARD_BOTTOM_LEFT  192

#define CARD_HORZ_EDGE    196
#define CARD_VERT_EDGE    179

#define CARD_JOINT_LTB    180
#define CARD_JOINT_TRB    195

#define CARD_WIDTH 5
#define CARD_HEIGHT 8

#define CARD_FULL_WIDTH 30
#define CARD_FULL_HEIGHT 42

void duel_begin(void);
void duel_end(void);

void duel_scene_preswitch(void);
void duel_scene_render(void);

extern scene_t duel_scene;

#endif /* DUEL_H */

/*
 * vi: ft=c:ts=2:sw=2:expandtab
 */

