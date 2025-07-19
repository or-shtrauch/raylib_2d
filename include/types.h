#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include "raylib.h"

#define FPS_TARGET 60
#define WIN_WIDTH  800
#define WIN_HEIGHT 800

#define WIN_TITLE         "SuperMario"
#define SPRITE_DIR        "../sprites"
#define SPRITE(file_name) SPRITE_DIR "/" file_name

#define PLAYER_SPRITE SPRITE("mario.png")
#define MAP_SPRITE    SPRITE("map.png")

#define GRAVITY_CONST   5000.0f
#define DOUBLE_JUMP     2
#define SPRITE_WIDTH    16
#define SPRITE_HEIGHT   16
#define SPRITE_SCALE    5
#define ANIMATION_SPEED 0.1f

#define MAP_CHUNK_HEIGHT 6
#define MAP_CHUNK_WIDTH  6

enum
{
    E_RAYLIB_ANIMATION_IDLE = 0,
    E_RAYLIB_ANIMATION_WALK,
    E_RAYLIB_ANIMATION_JUMP,
    E_RAYLIB_ANIMATION_FALL
};

enum
{
    E_RAYLIB_ERR_OK       = 0,
    E_RAYLIB_ERR_EFAILURE = -1,
    E_RAYLIB_ERR_EINVAL   = EINVAL,
};

typedef struct
{
    Vector2   position;
    Texture2D texture;
    float     vertical_speed;
    float     scale;
    float     rotation;
    float     jump_force;
    float     horizontal_speed;
    bool      is_on_ground;
    uint8_t   jumps_available;

    uint8_t   current_animation;
    int       current_frame;
    float     animation_timer;
    bool      facing_right;
    Rectangle source_rect;
} player_t;

typedef struct
{
    int start_frame;
    int frame_count;
    int row;
} animation_data_t;

typedef struct
{
    Rectangle source_rect;
    bool      is_solid;
} tile_info_t;

typedef struct
{
    Texture2D map_texture;
    float     scale;
} map_t;

typedef struct
{
    float    ground_level;
    float    delta_time;
    player_t player;
    bool     shutdown;
    map_t    map;
} game_data_t;


#endif /* __TYPES_H__ */