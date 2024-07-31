
#include "render_state.h"

void render_background() {
    unsigned int* pixel = (unsigned int*) render_state.memory;
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *pixel++ = 0xFF221f1c; 
        }
    }
}

void clear_screen(unsigned int color) {
    unsigned int* pixel = (unsigned int*)render_state.memory;
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *pixel++ = color;
        }
    }
}
inline int
clamp(int min, int val, int max) {
    if (val < min) return min;
    if (val > max)return max;
    return val;
}
void draw_rect_in_pixels(int x0, int y0, int x1, int y1, unsigned int color) {
    x0 = clamp(0, x0, render_state.width);
    x1 = clamp(0, x1, render_state.width);
    y0 = clamp(0, y0, render_state.height);
    y1 = clamp(0, y1, render_state.height);


    for (int y = y0; y < y1; y++) { 
        for (int x = x0; x < x1; x++) { 
                // Calculates the address of a pixel in memory
                unsigned int* pixel = (unsigned int*)render_state.memory + y * render_state.width + x;
               
                *pixel = color;
            }
        }
    }


void draw_rect(float x, float y, float half_size_x, float half_size_y, unsigned int color) {
    x *= render_state.height;
    y *= render_state.height;
    half_size_x *= render_state.height;
    half_size_y *= render_state.height;

    x += render_state.width / 2.f;
    y += render_state.height / 2.f;

    int x0 = x - half_size_x;
    int x1 = x + half_size_x;
    int y0 = y - half_size_y;
    int y1 = y + half_size_y;

    draw_rect_in_pixels(x0, y0, x1, y1, color);
}

const char* letters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

void draw_text(const char* text, float x, float y, float size, unsigned int color) {
	float half_size = size * .5f;
	float original_y = y;

	while (*text) {
		if (*text != 32) {
			const char** letter;
			letter = letters[*text - 'A'];
			float original_x = x;

			for (int i = 0; i < 7; i++) {
				const char* row = letter[i];
				while (*row) {
					if (*row == '0') {
						draw_rect(x, y, half_size, half_size, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		text++;
		x += size * 6.f;
		y = original_y;
	}
}



void draw_number(int number, float x, float y, float size, unsigned color) {
    float half_size = size * 0.5f;
    

    do {
        int digit = number % 10;
        number /= 10;

        switch (digit) {
        case 0: {
            draw_rect(x - size, y, half_size, 2.5 * size, color);
            draw_rect(x + size, y, half_size, 2.5 * size, color);
            draw_rect(x, y + size * 2.f, half_size, half_size, color);
            draw_rect(x, y - size * 2.f, half_size, half_size, color);
        } break;
        case 1: {
            draw_rect(x + size, y, half_size, 2.5 * size, color);
        } break;
        case 2: {
            draw_rect(x, y + size * 2.f, 1.5 * size, half_size, color);
            draw_rect(x + size, y + size, half_size, half_size, color);
            draw_rect(x, y, 1.5 * size, half_size, color);
            draw_rect(x - size, y - size, half_size, half_size, color);
            draw_rect(x, y - size * 2.f, 1.5 * size, half_size, color);
        } break;
        case 3: {
            draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
            draw_rect(x, y, 1.5f * size, half_size, color);
            draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
            draw_rect(x + size, y + size, half_size, half_size, color);
            draw_rect(x + size, y - size, half_size, half_size, color);
        } break;
        case 4: {
            draw_rect(x + size, y, half_size, 2.5 * size, color);
            draw_rect(x, y, half_size, half_size, color);
            draw_rect(x - size, y + size, half_size, 1.5 * size, color);
        } break;
        case 5: {
            draw_rect(x, y + size * 2.f, 1.5 * size, half_size, color);
            draw_rect(x - size, y + size, half_size, half_size, color);
            draw_rect(x, y, 1.5 * size, half_size, color);
            draw_rect(x + size, y - size, half_size, half_size, color);
            draw_rect(x, y - size * 2.f, 1.5 * size, half_size, color);
        } break;
        case 6: {
            draw_rect(x, y + size * 2.f, 1.5 * size, half_size, color);
            draw_rect(x - size, y + size, half_size, half_size, color);
            draw_rect(x, y, 1.5 * size, half_size, color);
            draw_rect(x - size, y - size, half_size, half_size, color);
            draw_rect(x + size, y - size, half_size, half_size, color);
            draw_rect(x, y - size * 2.f, 1.5 * size, half_size, color);
        } break;
        case 7: {
            draw_rect(x, y + size * 2.f, 1.5 * size, half_size, color);
            draw_rect(x + size, y + size, half_size, half_size, color);
            draw_rect(x + size, y, half_size, 2.5 * size, color);
        } break;
        case 8: {
            draw_rect(x, y + size * 2.f, half_size, half_size, color);
            draw_rect(x - size, y + size, half_size, half_size, color);
            draw_rect(x + size, y + size, half_size, half_size, color);
            draw_rect(x, y, half_size, half_size, color);
            draw_rect(x - size, y - size, half_size, half_size, color);
            draw_rect(x + size, y - size, half_size, half_size, color);
            draw_rect(x, y - size * 2.f, half_size, half_size, color);
            draw_rect(x - size, y, half_size, 2.5 * size, color);
            draw_rect(x + size, y, half_size, 2.5 * size, color);
        } break;
        case 9: {
            draw_rect(x, y + size * 2.f, 1.5 * size, half_size, color);
            draw_rect(x - size, y + size, half_size, half_size, color);
            draw_rect(x + size, y + size, half_size, half_size, color);
            draw_rect(x, y, 1.5 * size, half_size, color);
            draw_rect(x + size, y - size, half_size, half_size, color);
            draw_rect(x, y - size * 2.f, 1.5 * size, half_size, color);
        } break;
        }

        x -= size * 4.f;
    } while (number);
    }
