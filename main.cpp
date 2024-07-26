#include <windows.h>

// #pragma comment(lib,"C:/Windows/System32/gdi32.dll")

struct Render_State
{
    int height, width, size;
    void* memory;
    BITMAPINFO bitmap_info;
};
Render_State render_state;
#include "renderer.cpp" //需要放在Render_State定义之后

bool running = true;
LRESULT WND_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
        case WM_CLOSE: // 关闭窗口但程序在后台运行
        case WM_DESTROY: {
            running = false;
            } break;
        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;
            render_state.size = render_state.width * render_state.height * sizeof(u32);
            if(render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
            render_state.memory = VirtualAlloc(0, render_state.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader); //只是为了让编译器知道你设备用的大小
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;
            render_state.bitmap_info.bmiHeader.biBitCount = 32;
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
            } break;
        default:
            result = DefWindowProc(hwnd, uMsg, wParam, lParam); // Default window procedure
    }
    return result;
}

int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    // Create a window class
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "Game Window Class";
    window_class.lpfnWndProc = WND_callback;

    // Register the window class
    RegisterClass(&window_class);
    
    // Create the window
    HWND window = CreateWindow("Game Window Class", "C++ Game!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInst, 0);
    HDC hdc = GetDC(window);

    while (running)
    {
        // Input
        MSG message;
        while(PeekMessageA( &message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message); // Translates virtual-key messages into character messages
            DispatchMessage(&message); // Dispatches a message to a window procedure
        }
        // Simulate
        render_background();
        // Render 渲染
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
    }
    ReleaseDC(window, hdc);
    return 0;
}