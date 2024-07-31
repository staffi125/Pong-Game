#pragma once

void render_background();
void draw_number(int number, float x, float y, float size, unsigned int color);
void draw_rect(float x, float y, float half_size_x, float half_size_y, unsigned int color);
void draw_text(const char* text, float x, float y, float size, unsigned int color);