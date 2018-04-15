#pragma once
#include <thread>
#include <memory>
#include <windows.h>
#include <d2d1.h>
#include "settings.h"
#include "window.h"
#include "console.h"
#include "layer_hierarchy.h"

extern HINSTANCE global_instance;

class desktop {
    borderless_window background_window;

    desktop() : background_window(msg_callback) {};

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