#pragma once
#include <thread>
#include <windows.h>
#include "layer_hierarchy.h"

extern HINSTANCE global_instance;

class desktop {
    HDC main_device;
    HWND hwnd;
    layer_hierarchy layers;

    desktop();

    void refresh_cycle();
public:
    std::thread start_refresh_cycle();
    static LRESULT CALLBACK msg_callback(HWND, UINT, WPARAM, LPARAM);

    static desktop& instance() {
        static desktop instance;
        return instance;
    }
};
 
void manage_desktop();