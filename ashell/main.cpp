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
    std::this_thread::sleep_for(std::chrono::seconds(15));
    dbg << "start";
    while (true){
        std::vector<std::unique_ptr<borderless_window>> windows;
        for (int i = 0; i < 1; ++i)
            windows.emplace_back(new borderless_window(desktop::msg_callback));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    dbg << "finish";
}