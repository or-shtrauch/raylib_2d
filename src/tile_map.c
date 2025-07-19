#include <stdlib.h>
#include "raylib.h"
#include <string.h>

#include "types.h"
#include "tile_map.h"

#define TILE_SCALE 3

extern game_data_t s_game;

// 1 horizontal = 1 * TILE_WIDTH
// 1 vertical = 1 * TILE_HEIGHT
tile_info_t s_level_tile_map[E_TILE_COUNT] = {
    [E_TILE_BACKGROUND] = MAKE_TILE(0, 0, E_NOT_SOLID),

    [E_TILE_FLOOR_LEFT_EDGE]             = MAKE_TILE(2, 2, E_SOLID),
    [E_TILE_UNDER_FLOOR_LEFT_EDGE]       = MAKE_TILE(2, 3, E_SOLID),
    [E_TILE_UNDER_UNDER_FLOOR_LEFT_EDGE] = MAKE_TILE(2, 4, E_SOLID),

    [E_TILE_FLOOR_RIGHT_EDGE]             = MAKE_TILE(7, 2, E_SOLID),
    [E_TILE_UNDER_FLOOR_RIGHT_EDGE]       = MAKE_TILE(7, 3, E_SOLID),
    [E_TILE_UNDER_UNDER_FLOOR_RIGHT_EDGE] = MAKE_TILE(7, 4, E_SOLID),

    [E_TILE_FLOOR_STRAIGHT_A]           = MAKE_TILE(3, 2, E_SOLID),
    [E_TILE_FLOOR_STRAIGHT_B]           = MAKE_TILE(4, 2, E_SOLID),
    [E_TILE_FLOOR_STRAIGHT_C]           = MAKE_TILE(5, 2, E_SOLID),
    [E_TILE_FLOOR_STRAIGHT_D]           = MAKE_TILE(6, 2, E_SOLID),
    [E_TILE_UNDER_FLOOR_STRAIGHT]       = MAKE_TILE(3, 3, E_SOLID),
    [E_TILE_UNDER_UNDER_FLOOR_STRAIGHT] = MAKE_TILE(3, 4, E_SOLID),
};

int tile_map_init(map_t *map)
{
    if (!map) {
        return E_RAYLIB_ERR_EINVAL;
    }

    memset(map, 0, sizeof(map_t));

    map->scale = TILE_SCALE;

    map->map_texture = LoadTexture(MAP_SPRITE);
    if (map->map_texture.id == 0) {
        return E_RAYLIB_ERR_EFAILURE;
    }

    return E_RAYLIB_ERR_OK;
}

void tile_map_draw_tile(map_t *map, int tile, Vector2 pos)
{
    Rectangle dst = {
        .x      = pos.x,
        .y      = pos.y,
        .width  = SPRITE_WIDTH * map->scale,
        .height = SPRITE_HEIGHT * map->scale,
    };
    Vector2 zero = {
        .x = 0,
        .y = 0,
    };

    if (!map || map->map_texture.id == 0 || tile < 0 || tile >= E_TILE_COUNT) {
        return;
    }

    DrawTexturePro(map->map_texture, s_level_tile_map[tile].source_rect, dst,
                   zero, 0.0f, WHITE);
}

void tile_map_draw_chunk(map_t *map, int height, int width,
                         int chunk[height][width], Vector2 pos)
{
    int     i;
    int     j;
    Vector2 tile_pos = pos;

    if (!map || !map->map_texture.id || !chunk) {
        return;
    }

    for (i = 0; i < height; i++) {
        if (!chunk[0]) {
            return;
        }


        for (j = 0; j < width; j++) {
            tile_map_draw_tile(map, chunk[i][j], tile_pos);
            tile_pos.x += SPRITE_WIDTH * map->scale;
        }

        tile_pos.x = pos.x;
        tile_pos.y -= SPRITE_HEIGHT * map->scale;
    }


    while (tile_pos.y >= 0) {
        for (j = 0; j < width; j++) {
            tile_map_draw_tile(map, E_TILE_BACKGROUND, tile_pos);
            tile_pos.x += SPRITE_WIDTH * map->scale;
        }

        tile_pos.x = pos.x;
        tile_pos.y -= SPRITE_HEIGHT * map->scale;
    }
}