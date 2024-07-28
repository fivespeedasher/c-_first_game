#include <windows.h>
#include "utils.cpp"
#include "platform_common.cpp"
// #pragma comment(lib,"C:/Windows/System32/gdi32.dll")

struct Render_State
{
    int height, width, size;
    void* memory;
    BITMAPINFO bitmap_info;
}; 
global_variable Render_State render_state;
#include "renderer.cpp" //需要放在Render_State定义之后
#include "game.cpp" //需要放在renderer.cpp之后

global_variable bool running = true;
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
//WOC还能宏定义case nb
#define process_button(b, vk)\
case vk: {\
    input.buttons[b].changed = (input.buttons[b].is_down != is_down);\
    input.buttons[b].is_down = is_down;\
} break;

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

    Input input = {};

    float delta_time = 0.016666f; // 60fps
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time); //将当前的高精度计时器值存储到 frame_begin_time 变量中

    float performance_frequency; // {} 创建了局部作用域，避免perf出现冲突
    {
        LARGE_INTEGER perf; 
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)perf.QuadPart; //计时器的频率 
    }

    while (running) // 每一个frame
    {
        // Input
        for(int i = 0; i < BUTTON_COUNT; i++) {
            input.buttons[i].changed = false;
        }
        MSG message;
        while(PeekMessageA( &message, window, 0, 0, PM_REMOVE)) { 
            switch(message.message) {
                case WM_KEYUP: 
                case WM_KEYDOWN: {
                    u32 vk_code = (u32)message.wParam;
                    bool is_down = ((message.lParam & (1 << 31)) == 0); // 31st bit is the sign bit 0的时候表示WM_KEYDOWN
                    
                    switch(vk_code) {
                        process_button(BUTTON_UP ,VK_UP);// VK_UP=0x26 是系统定义的一个32位的常量，如果wParam==VK_UP，表示按下了上键
                        process_button(BUTTON_DOWN, VK_DOWN);
                        process_button(BUTTON_W, 'W');
                        process_button(BUTTON_S, 'S');
                    }
                } break;
                default: {
                    TranslateMessage(&message); // Translates virtual-key messages into character messages
                    DispatchMessage(&message); // Dispatches a message to a window procedure
                }
            }
        }

        // Simulate
        simulate_game(&input, delta_time);
        // Render 渲染
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency; // delta_time是两帧之间的时间,单位是秒
        frame_begin_time = frame_end_time;
    }
    ReleaseDC(window, hdc);
    return 0;
}