#include <windows.h>
#include "render_state.h"
#include "input.h"
#include "game.h" 

Input input;
Render_State render_state;

#include "render.cpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
   
    ShowCursor(FALSE);
    
    MSG msg{}; // A structure that contains message information
    HWND hwnd{}; // A window that stores information about our registered window
    WNDCLASSEX wc{ sizeof(WNDCLASSEX) }; // The structure responsible for the characteristics of the window

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        switch (uMsg) {
        case WM_DESTROY: {
            PostQuitMessage(EXIT_SUCCESS);
            return 0;
        }

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hWnd, &rect);
            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;

            int size = render_state.width * render_state.height * sizeof(unsigned int);

            if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
            render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;
            render_state.bitmap_info.bmiHeader.biBitCount = 32;
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;


            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
            EndPaint(hWnd, &ps);
            return 0;
        }



        default: {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        }
        };

    wc.lpszClassName = L"MyAppClass";
    wc.lpszMenuName = nullptr;
    wc.style = CS_VREDRAW | CS_HREDRAW;

    if (!RegisterClassEx(&wc)) {
        return EXIT_FAILURE;
    }

    hwnd = CreateWindow(wc.lpszClassName, L"First Game", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1980, 720, nullptr, nullptr, wc.hInstance, nullptr);
    if (hwnd == NULL)
        return EXIT_FAILURE;
   
    SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);

    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);


    LARGE_INTEGER frame_begin_time;
    LARGE_INTEGER frequency;
    QueryPerformanceCounter(&frame_begin_time);
    QueryPerformanceFrequency(&frequency);

    while (GetMessage(&msg, nullptr, 0, 0)) {
        for (int i = 0; i < BUTTON_COUNT; ++i) {
            input.buttons[i].changed = false;
        }

        switch (msg.message) {
        case WM_KEYUP:
        case WM_KEYDOWN: {

            if (msg.wParam == VK_ESCAPE) {
                PostQuitMessage(0); 
            }

            unsigned int vk_code = (unsigned int)msg.wParam;
            bool is_down = ((msg.lParam & (1 << 31)) == 0);

#define process_button(b, VK) \
    case VK: { \
        input.buttons[b].is_down = is_down; \
        input.buttons[b].changed = true; \
    } break;

            switch (vk_code) {
                process_button(BUTTON_UP, VK_UP)
                process_button(BUTTON_DOWN, VK_DOWN)
                process_button(BUTTON_W, 'W')
                process_button(BUTTON_S, 'S')
                process_button(BUTTON_LEFT, VK_LEFT)
                process_button(BUTTON_RIGHT, VK_RIGHT)
                process_button(BUTTON_ENTER, VK_RETURN)
            }
        } break;
            
        default: {
            TranslateMessage(&msg);
            DispatchMessage(&msg); //Passes the message to the window procedure for processing
        
        }
    }

        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        float delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / frequency.QuadPart;
        frame_begin_time = frame_end_time;

        simulate_game( delta_time);
        InvalidateRect(hwnd, NULL, FALSE); // Screen update
    }

    return msg.wParam;

    
}
