#pragma once

void simulate_game(float dt);
void reset_position(int& player_score);
void update_player(float& player_p, float& player_dp, float player_ddp, float dt);