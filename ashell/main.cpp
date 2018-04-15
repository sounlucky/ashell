#include <windows.h>
#include <chrono>
#include "desktop.h"

HINSTANCE global_instance;

//entry point
int CALLBACK WinMain(
    HINSTANCE   instance,
    HINSTANCE   ,
    LPSTR       ,
    int         
) { 
    global_instance = instance;
    borderless_window w(desktop::msg_callback);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}