#include "game.h"
#include "render_state.h"
#include "input.h"
#include "render.h" 

extern Input input;

#define is_down(b) (input.buttons[b].is_down)
#define pressed(b) (input.buttons[b].is_down && input.buttons[b].changed)
#define released(b) (!input.buttons[b].is_down && input.buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 0.90, arena_half_size_y = 0.45;
float player_half_size_x = 0.02, player_half_size_y = 0.1f;
float ball_p_x, ball_p_y, ball_dp_x = 1.5, ball_dp_y, ball_half_size = 0.01f;

int player_1_score, player_2_score;

bool is_paused = false;
float timer = 0.f;
const float PAUSE_DURATION = 0.4f;

void reset_position(int& player_score) {
    ball_dp_y = 0;
    ball_dp_x *= -1;
    ball_p_x = 0;
    ball_p_y = 0;

    player_score++;

    player_1_p = 0;
    player_1_dp = 0;
    player_2_p = 0;
    player_2_dp = 0;

    is_paused = true;
    timer = PAUSE_DURATION;
}

void update_player(float& player_p, float& player_dp, float player_ddp, float dt) {
    player_ddp -= player_dp * 3;

    player_p = player_p + player_dp * dt + player_ddp * dt * dt * 0.5f;
    player_dp = player_dp + player_ddp * dt;

    if (player_p + player_half_size_y > arena_half_size_y) {
        player_p = arena_half_size_y - player_half_size_y;
        player_dp = -0.3;
    }
    else if (player_p - player_half_size_y < -arena_half_size_y) {
        player_p = -arena_half_size_y + player_half_size_y;
        player_dp = 0.3;
    }
}


enum Gamemode {
    GM_MENU,
    GM_GAMEPLAY,
};

Gamemode current_gamemode;
int hot_button = 0;

bool enemy_is_ai;



void simulate_game(float dt) {
    render_background();

    draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xFFf7bb6e);

    if (current_gamemode == GM_GAMEPLAY) {

        draw_number(player_1_score, -0.1f, 0.4f, 0.01f, 0xcccccc);
        draw_number(player_2_score, 0.1f, 0.4f, 0.01f, 0xcccccc);

        draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xFFffffff);

        draw_rect(0.85f, player_1_p, player_half_size_x, player_half_size_y, 0xFFcd5c5c);
        draw_rect(-0.85f, player_2_p, player_half_size_x, player_half_size_y, 0xFFcd5c5c);

       

        if (is_paused) {
            timer -= dt;
            if (timer <= 0) {
                is_paused = false;
            }
            return;
        }

        float player_1_ddp = 0.f;

        if (!enemy_is_ai) {
            if (is_down(BUTTON_UP)) player_1_ddp += 6;
            if (is_down(BUTTON_DOWN)) player_1_ddp -= 6;
        }
        else {
            player_1_ddp = (ball_p_y - player_1_p) * 200;
            if (player_1_ddp > 7) player_1_ddp = 7;
            if (player_1_ddp < -7) player_1_ddp = -7;
        }
        

        float player_2_ddp = 0.f;

        if (is_down(BUTTON_W)) player_2_ddp += 6;
        if (is_down(BUTTON_S)) player_2_ddp -= 6;

        update_player(player_1_p, player_1_dp, player_1_ddp, dt);
        update_player(player_2_p, player_2_dp, player_2_ddp, dt);

        ball_p_x += ball_dp_x * dt;
        ball_p_y += ball_dp_y * dt;

        if (ball_p_x + ball_half_size > 0.85f - player_half_size_x &&
            ball_p_x - ball_half_size < 0.85f + player_half_size_x &&
            ball_p_y + ball_half_size < player_1_p + player_half_size_y &&
            ball_p_y - ball_half_size > player_1_p - player_half_size_y) {
            ball_p_x = 0.85f - player_half_size_x - ball_half_size;
            ball_dp_x *= -1;
            ball_dp_y = (ball_p_y - player_1_p) * 2.6 + player_1_dp;
        }
        else if (ball_p_x + ball_half_size > -0.85f - player_half_size_x &&
            ball_p_x - ball_half_size < -0.85f + player_half_size_x &&
            ball_p_y + ball_half_size < player_2_p + player_half_size_y &&
            ball_p_y - ball_half_size > player_2_p - player_half_size_y) {
            ball_p_x = -0.85f + player_half_size_x + ball_half_size;
            ball_dp_x *= -1;
            ball_dp_y = (ball_p_y - player_2_p) * 2.6 + player_2_dp;
        }

        if (ball_p_y + ball_half_size > arena_half_size_y) {
            ball_p_y = arena_half_size_y - ball_half_size;
            ball_dp_y *= -1.1;
        }
        else if (ball_p_y - ball_half_size < -arena_half_size_y) {
            ball_p_y = -arena_half_size_y + ball_half_size;
            ball_dp_y *= -1.1;
        }

        if (ball_p_x + ball_half_size > arena_half_size_x) {
            reset_position(player_1_score);
        }
        else if (ball_p_x - ball_half_size < -arena_half_size_x) {
            reset_position(player_2_score);
        }

    }
    else {

        if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
            hot_button = !hot_button;
        }
        

        if (pressed(BUTTON_ENTER)) {
            current_gamemode = GM_GAMEPLAY;
            enemy_is_ai = hot_button ? 0 : 1;
        }

        if (hot_button == 0) {
            draw_text("SINGLE PLAYER", -0.8, -0.1, 0.01, 0xff0000);
            draw_text("MULTIPLAYER", 0.2, - 0.1, 0.01, 0xaaaaaa);
            
        }
        else {
            
            draw_text("SINGLE PLAYER", -0.8, -0.1, 0.01, 0xaaaaaa);
            draw_text("MULTIPLAYER", 0.2, -0.1, 0.01, 0xff0000);
        }
    }
}
