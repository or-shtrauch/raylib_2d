#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "types.h"

#define MAKE_TILE(x, y, solid)                                                 \
    { (Rectangle){ x * SPRITE_WIDTH, y * SPRITE_HEIGHT, SPRITE_WIDTH,          \
                   SPRITE_HEIGHT },                                            \
      solid }

enum
{
    E_NOT_SOLID = 0,
    E_SOLID
};

enum
{
    E_TILE_BACKGROUND = 0,

    E_TILE_FLOOR_RIGHT_EDGE,
    E_TILE_UNDER_FLOOR_RIGHT_EDGE,
    E_TILE_UNDER_UNDER_FLOOR_RIGHT_EDGE,

    E_TILE_FLOOR_LEFT_EDGE,
    E_TILE_UNDER_FLOOR_LEFT_EDGE,
    E_TILE_UNDER_UNDER_FLOOR_LEFT_EDGE,

    E_TILE_FLOOR_STRAIGHT_A,
    E_TILE_FLOOR_STRAIGHT_B,
    E_TILE_FLOOR_STRAIGHT_C,
    E_TILE_FLOOR_STRAIGHT_D,
    E_TILE_UNDER_FLOOR_STRAIGHT,
    E_TILE_UNDER_UNDER_FLOOR_STRAIGHT,

    E_TILE_COUNT

};

int  tile_map_init(map_t *map);
void tile_map_draw_tile(map_t *map, int tile, Vector2 pos);
void tile_map_draw_chunk(map_t *map, int height, int width,
                         int chunk[height][width], Vector2 pos);

#endif /* __TILE_MAP_H__ */
