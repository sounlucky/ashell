#include "window_classes.h"
#include <cassert>
#include <windows.h>
#include "console.h"
#include "desktop.h"

using callback_ret_t = LRESULT CALLBACK;
typedef callback_ret_t(*msg_callback_t)(HWND, UINT, WPARAM, LPARAM);

void init_console_class() {
    WNDCLASSEX& wc = *(new WNDCLASSEX);//an arg to RegisterClassEx
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = reinterpret_cast<decltype(wc.lpfnWndProc)>(console::msg_callback);
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = global_instance;
    wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = L"console_class";
    wc.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);

    auto regged = RegisterClassEx(&wc);
    assert(regged);
}

void init_background_class() {
    WNDCLASSEX& wc = *(new WNDCLASSEX);//an arg to RegisterClassEx
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = reinterpret_cast<decltype(wc.lpfnWndProc)>(desktop::msg_callback);
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = global_instance;
    wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = L"background_class";
    wc.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);

    auto regged = RegisterClassEx(&wc);
    assert(regged);
}

void init_window_classes(){
    init_console_class();
    init_background_class();
}
