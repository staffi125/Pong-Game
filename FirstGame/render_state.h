#pragma once
#include <windows.h>

struct Render_State {
    void* memory;
    int height, width;
    BITMAPINFO bitmap_info;
};

extern Render_State render_state;
