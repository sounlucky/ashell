#pragma once
#include <thread>
#include <windows.h>

extern HINSTANCE global_instance;

class desktop {
    HBITMAP h_backgroundbitmap;
    HDC main_device;
    HWND hwnd;

    desktop();

    void update(HDC);
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