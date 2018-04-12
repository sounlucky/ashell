#pragma once
#include <thread>
#include <memory>
#include <windows.h>
#include <d2d1.h>
#include "layer_hierarchy.h"

extern HINSTANCE global_instance;

class desktop {
    HDC main_device;
    HWND hwnd;
    std::unique_ptr<ID2D1Factory> unq_factory;
    std::unique_ptr<ID2D1HwndRenderTarget> unq_render_target;
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