#include <string.h>
#include <stdbool.h>
#include <signal.h>

#include "player.h"
#include "types.h"
#include "tile_map.h"
#include "raylib.h"

game_data_t s_game = { 0 };

static void signal_handler(int sig)
{
    TraceLog(LOG_INFO, "Caught signal %d\n", sig);
    s_game.shutdown = true;
}

int main(void)
{
    int err = E_RAYLIB_ERR_OK;

    InitWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
    SetTargetFPS(FPS_TARGET);

    memset(&s_game, 0, sizeof(game_data_t));

    if (player_init(&s_game.player)) {
        TraceLog(LOG_ERROR, "Failed to init player");
        err = E_RAYLIB_ERR_EFAILURE;
        goto out;
    }

    if (tile_map_init(&s_game.map)) {
        TraceLog(LOG_ERROR, "Failed to init tile map");
        err = E_RAYLIB_ERR_EFAILURE;
        goto out;
    }

    s_game.ground_level = WIN_HEIGHT - (s_game.player.texture.height / 2.0f);

    signal(SIGINT, signal_handler);

    while (!WindowShouldClose() && s_game.shutdown == false) {
        s_game.delta_time = GetFrameTime();

        player_tick(&s_game.player);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // int height = 3;
        // int width = 6;
        int chunk[3][6] = {
            { E_TILE_UNDER_UNDER_FLOOR_LEFT_EDGE,
              E_TILE_UNDER_UNDER_FLOOR_STRAIGHT,
              E_TILE_UNDER_UNDER_FLOOR_STRAIGHT,
              E_TILE_UNDER_UNDER_FLOOR_STRAIGHT,
              E_TILE_UNDER_UNDER_FLOOR_STRAIGHT,
              E_TILE_UNDER_UNDER_FLOOR_RIGHT_EDGE },

            { E_TILE_UNDER_FLOOR_LEFT_EDGE, E_TILE_UNDER_FLOOR_STRAIGHT,
              E_TILE_UNDER_FLOOR_STRAIGHT, E_TILE_UNDER_FLOOR_STRAIGHT,
              E_TILE_UNDER_FLOOR_STRAIGHT, E_TILE_UNDER_FLOOR_RIGHT_EDGE },


            { E_TILE_FLOOR_LEFT_EDGE, E_TILE_FLOOR_STRAIGHT_A,
              E_TILE_FLOOR_STRAIGHT_B, E_TILE_FLOOR_STRAIGHT_C,
              E_TILE_FLOOR_STRAIGHT_D, E_TILE_FLOOR_RIGHT_EDGE },
        };

        for (int i = 0; i < 2; i++) {
            tile_map_draw_chunk(
                &s_game.map, 3, 6, chunk,
                (Vector2){ i * (s_game.map.scale * SPRITE_WIDTH * 6),
                           s_game.ground_level + SPRITE_HEIGHT });
        }

        player_draw(&s_game.player);
        DrawFPS(10, 10);

        EndDrawing();
    }

out:
    CloseWindow();

    return err;
}