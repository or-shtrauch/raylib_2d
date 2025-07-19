#include "raylib.h"
#include <string.h>
#include <math.h>

#include "player.h"
#include "types.h"

extern game_data_t s_game;

static animation_data_t g_player_animation_data[] = {
    [E_RAYLIB_ANIMATION_IDLE] = { 0, 1, 0 },
    [E_RAYLIB_ANIMATION_WALK] = { 1, 3, 0 },
    [E_RAYLIB_ANIMATION_JUMP] = { 4, 1, 0 },
    [E_RAYLIB_ANIMATION_FALL] = { 5, 1, 0 }
};

int player_init(player_t *player)
{
    if (!player) {
        return E_RAYLIB_ERR_EFAILURE;
    }

    memset(player, 0, sizeof(player_t));

    player->jump_force         = -1100.0f;
    player->horizontal_speed   = 250.0f;
    player->scale              = SPRITE_SCALE;
    player->jumps_available    = DOUBLE_JUMP;
    player->facing_right       = true;
    player->source_rect.width  = SPRITE_WIDTH;
    player->source_rect.height = SPRITE_HEIGHT;

    player->texture = LoadTexture(PLAYER_SPRITE);
    if (player->texture.id == 0) {
        return E_RAYLIB_ERR_EFAILURE;
    }

    return E_RAYLIB_ERR_OK;
}

void player_draw(player_t *player)
{
    Rectangle dst;
    Vector2   zero = {
          .x = 0,
          .y = 0,
    };

    if (!player) {
        return;
    }

    dst.x      = player->position.x;
    dst.y      = player->position.y;
    dst.width  = SPRITE_WIDTH * player->scale;
    dst.height = SPRITE_HEIGHT * player->scale;

    DrawTexturePro(player->texture, player->source_rect, dst, zero,
                   player->rotation, WHITE);
}

static void update_player_animation(player_t *player)
{
    uint8_t new_animation = E_RAYLIB_ANIMATION_IDLE;
    int     frame_count   = 0;
    int     start_frame   = 0;
    int     row           = 0;

    if (!player->is_on_ground) {
        if (player->vertical_speed < 0) {
            new_animation = E_RAYLIB_ANIMATION_JUMP;
        } else {
            new_animation = E_RAYLIB_ANIMATION_FALL;
        }
    } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) {
        new_animation = E_RAYLIB_ANIMATION_WALK;
    }

    if (new_animation != player->current_animation) {
        player->current_animation = new_animation;
        player->current_frame     = 0;
        player->animation_timer   = 0.0f;
    }

    player->animation_timer += s_game.delta_time;

    if (player->animation_timer >= ANIMATION_SPEED) {
        player->animation_timer = 0.0f;

        frame_count =
            g_player_animation_data[player->current_animation].frame_count;

        if (frame_count > 1) {
            player->current_frame = (player->current_frame + 1) % frame_count;
        }
    }

    start_frame =
        g_player_animation_data[player->current_animation].start_frame;
    row = g_player_animation_data[player->current_animation].row;

    player->source_rect.x =
        (start_frame + player->current_frame) * SPRITE_WIDTH;
    player->source_rect.y = row * SPRITE_HEIGHT;

    if (!player->facing_right) {
        player->source_rect.width = -SPRITE_WIDTH;
    } else {
        player->source_rect.width = SPRITE_WIDTH;
    }
}

void player_tick(player_t *player)
{
    if (!player) {
        return;
    }

    // apply gravity
    if (!player->is_on_ground) {
        player->vertical_speed += GRAVITY_CONST * s_game.delta_time;
    }

    // apply speed to position
    player->position.y += player->vertical_speed * s_game.delta_time;

    if (player->position.y >= s_game.ground_level) {
        player->position.y     = s_game.ground_level;
        player->vertical_speed = 0.0f;
        player->is_on_ground   = true;

        player->jumps_available = DOUBLE_JUMP;
    } else {
        player->is_on_ground = false;
    }

    if (IsKeyPressed(KEY_SPACE) && player->jumps_available > 0) {
        player->vertical_speed = player->jump_force;
        player->is_on_ground   = false;

        player->jumps_available--;
    }

    // handle horizontal movement
    if (IsKeyDown(KEY_RIGHT)) {
        player->position.x += player->horizontal_speed * s_game.delta_time;
        player->facing_right = true;
    }

    if (IsKeyDown(KEY_LEFT)) {
        player->position.x -= player->horizontal_speed * s_game.delta_time;
        player->facing_right = false;
    }

    // set border
    player->position.x = fmaxf(0, player->position.x);
    player->position.x =
        fminf(WIN_WIDTH - (SPRITE_WIDTH * player->scale), player->position.x);

    update_player_animation(player);
}
