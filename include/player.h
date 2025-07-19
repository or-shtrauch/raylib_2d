#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "types.h"

int  player_init(player_t *player);
void player_draw(player_t *player);
void player_tick(player_t *player);

#endif /* __PLAYER_H__ */