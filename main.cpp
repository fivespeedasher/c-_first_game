#include <windows.h>

bool running = true;
LRESULT WND_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
        case WM_CLOSE: // 关闭窗口但程序在后台运行
        case WM_DESTROY:
            {running = false;
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

    while (running)
    {
        // Input
        MSG message;
        while(PeekMessageA( &message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message); // Translates virtual-key messages into character messages
            DispatchMessage(&message); // Dispatches a message to a window procedure
        }
        // Simulate

        // Render 渲染
    }
    
}