#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "raylib.h"

#define FPS_TARGET 60
#define WIN_WIDTH 1000
#define WIN_HEIGHT 800
#define WIN_TITLE "SuperMario"

#define SPRITE_DIR "../sprites"
#define SPRITE(file_name) SPRITE_DIR "/" file_name

#define PLAYER_SPRITE SPRITE("mario.png")

#define GRAVITY_CONST 5000.0f
#define DOUBLE_JUMP 2

#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16
#define ANIMATION_SPEED 0.1f

enum {
	E_RAYLIB_ANIMATION_IDLE,
	E_RAYLIB_ANIMATION_WALK,
	E_RAYLIB_ANIMATION_JUMP,
	E_RAYLIB_ANIMATION_FALL
};

enum { E_RAYLIB_ERR_OK, E_RAYLIB_ERR_EFAILURE };

typedef struct {
	Vector2 position;
	Texture2D texture;
	float vertical_speed;
	float scale;
	float rotation;
	float jump_force;
	float horizontal_speed;
	bool is_on_ground;
	uint8_t jumps_available;

	uint8_t current_animation;
	int current_frame;
	float animation_timer;
	bool facing_right;
	Rectangle source_rect;
} player_t;

struct {
	float ground_level;
	float delta_time;
	player_t player;
} g_game = {
	.ground_level = 0,
};

struct {
	int start_frame;
	int frame_count;
	int row;
} g_animation_data[] = { [E_RAYLIB_ANIMATION_IDLE] = { 0, 1, 0 },
			 [E_RAYLIB_ANIMATION_WALK] = { 1, 3, 0 },
			 [E_RAYLIB_ANIMATION_JUMP] = { 4, 1, 0 },
			 [E_RAYLIB_ANIMATION_FALL] = { 5, 1, 0 } };

static int init_player(player_t *player)
{
	if (!player) {
		return E_RAYLIB_ERR_EFAILURE;
	}

	player->position.x = 0.0f;
	player->position.y = 0.0f;

	player->vertical_speed = 0.0f;
	player->jump_force = -1100.0f;
	player->horizontal_speed = 250.0f;
	player->rotation = 0.0f;
	player->scale = 5.0f;

	player->is_on_ground = false;
	player->jumps_available = DOUBLE_JUMP;

	player->current_animation = E_RAYLIB_ANIMATION_IDLE;
	player->current_frame = 0;
	player->animation_timer = 0.0f;
	player->facing_right = true;

	player->source_rect.x = 0;
	player->source_rect.y = 0;
	player->source_rect.width = SPRITE_WIDTH;
	player->source_rect.height = SPRITE_HEIGHT;

	player->texture = LoadTexture(PLAYER_SPRITE);
	if (player->texture.id == 0) {
		return E_RAYLIB_ERR_EFAILURE;
	}

	return E_RAYLIB_ERR_OK;
}

static void update_player_animation(player_t *player)
{
	uint8_t new_animation = E_RAYLIB_ANIMATION_IDLE;
	int frame_count = 0;
	int start_frame = 0;
	int row = 0;

	if (!player) {
		return;
	}

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
		player->current_frame = 0;
		player->animation_timer = 0.0f;
	}

	player->animation_timer += g_game.delta_time;

	if (player->animation_timer >= ANIMATION_SPEED) {
		player->animation_timer = 0.0f;

		frame_count =
			g_animation_data[player->current_animation].frame_count;

		if (frame_count > 1) {
			player->current_frame =
				(player->current_frame + 1) % frame_count;
		}
	}

	start_frame = g_animation_data[player->current_animation].start_frame;
	row = g_animation_data[player->current_animation].row;

	player->source_rect.x =
		(start_frame + player->current_frame) * SPRITE_WIDTH;
	player->source_rect.y = row * SPRITE_HEIGHT;

	if (!player->facing_right) {
		player->source_rect.width = -SPRITE_WIDTH;
	} else {
		player->source_rect.width = SPRITE_WIDTH;
	}
}

static void draw_player(player_t *player)
{
	Rectangle dst;

	if (!player) {
		return;
	}

	dst.x = player->position.x;
	dst.y = player->position.y;
	dst.width = SPRITE_WIDTH * player->scale;
	dst.height = SPRITE_HEIGHT * player->scale;

	DrawTexturePro(player->texture, player->source_rect, dst,
		       (Vector2){ 0, 0 }, player->rotation, WHITE);
}

static void handle_player_movement(player_t *player)
{
	if (!player) {
		return;
	}

	// apply gravity
	if (!player->is_on_ground) {
		player->vertical_speed += GRAVITY_CONST * g_game.delta_time;
	}

	// apply speed to position
	player->position.y += player->vertical_speed * g_game.delta_time;

	if (player->position.y >= g_game.ground_level) {
		player->position.y = g_game.ground_level;
		player->vertical_speed = 0.0f;
		player->is_on_ground = true;

		player->jumps_available = DOUBLE_JUMP;
	} else {
		player->is_on_ground = false;
	}

	if (IsKeyPressed(KEY_SPACE) && player->jumps_available > 0) {
		player->vertical_speed = player->jump_force;
		player->is_on_ground = false;

		player->jumps_available--;
	}

	// handle horizontal movement
	if (IsKeyDown(KEY_RIGHT)) {
		player->position.x +=
			player->horizontal_speed * g_game.delta_time;
		player->facing_right = true;
	}
	if (IsKeyDown(KEY_LEFT)) {
		player->position.x -=
			player->horizontal_speed * g_game.delta_time;
		player->facing_right = false;
	}

	// set border
	player->position.x = fmaxf(0, player->position.x);
	player->position.x = fminf(WIN_WIDTH - (SPRITE_WIDTH * player->scale),
				   player->position.x);

	update_player_animation(player);
}

int main(void)
{
	InitWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);

	if (init_player(&g_game.player)) {
		TraceLog(LOG_ERROR, "Failed to init player");
		CloseWindow();
		return E_RAYLIB_ERR_EFAILURE;
	}

	g_game.ground_level = WIN_HEIGHT - g_game.player.texture.height;

	SetTargetFPS(FPS_TARGET);

	while (!WindowShouldClose()) {
		g_game.delta_time = GetFrameTime();

		handle_player_movement(&g_game.player);

		BeginDrawing();
		ClearBackground(RAYWHITE);

		draw_player(&g_game.player);

		DrawFPS(10, 10);

		EndDrawing();
	}

	CloseWindow();

	return E_RAYLIB_ERR_OK;
}