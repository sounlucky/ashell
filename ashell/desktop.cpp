#include <windows.h>
#include <gdiplus.h>
#include <cassert>
#include <chrono>
#include <memory>
#include <thread>
#include "window_classes.h"
#include "console.h"
#include "dbg.h"
#include "desktop.h"
#include "settings.h"
#include "layer_hierarchy.h"
#include "static_image.h"

desktop::desktop() {
    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        L"background_class",
        nullptr,//no need for title (:
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        settings::system::display_width,
        settings::system::display_height,
        nullptr,
        nullptr,
        global_instance,
        nullptr);
    assert(hwnd);
    main_device = GetDC(hwnd);
    ShowWindow(hwnd, SW_RESTORE);
    UpdateWindow(hwnd);
}

void desktop::refresh_cycle(){
    SetLastError(0);//std::thread may set random 126 value
    auto front_device = CreateCompatibleDC(main_device);
    decltype(front_device) back_device = nullptr;
    size_t frame_counter = 0;
    auto second_start = std::chrono::high_resolution_clock::now();
    layers.add_layer(new static_image(L"girl.jpg", {50,200}));
    layers.add_layer(new static_image(L"Banana.png", { 100,200 }));
    auto image = static_image(L"Banana.png", { 100,200 });
    while (true) {
        auto lasterror = GetLastError();
        assert(lasterror == 0);
        auto frame_start = std::chrono::high_resolution_clock::now();
        frame_counter++;

        //draw
        Rectangle(front_device, 100, 20, 300, 400);
        BitBlt(main_device, 0, 0, settings::system::display_width, settings::system::display_height,
            front_device, 0, 0, SRCCOPY
        );
        DeleteDC(front_device);

        //calculate graphics
        back_device = CreateCompatibleDC(main_device);
        layers.apply(main_device);

        std::swap(front_device, back_device);

        auto elapsed = std::chrono::high_resolution_clock::now() - frame_start;
        //dbg << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << '\n';
        std::this_thread::sleep_for(settings::internal::delay - elapsed);
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - second_start).count() >= 1) {
            if (frame_counter < settings::internal::fps)
                dbg << "dropping fps! got " << frame_counter << " frames, expected " << settings::internal::fps << "; elapsed " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << "ms, expected " << settings::internal::delay.count() <<"ms\n";
            frame_counter = 0;
            second_start = std::chrono::high_resolution_clock::now();
        }
    }
}

std::thread desktop::start_refresh_cycle()
{
    return std::thread([this](){
        refresh_cycle();
    });
}

//message callback
LRESULT CALLBACK desktop::msg_callback(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
    switch (msg) {
    case WM_KEYDOWN:
        dbg << "WM_KEYDOWN";
        return true;

    case WM_PAINT://to ignore (:
    case WM_CLOSE:
    case WM_DESTROY:
        return true;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

void manage_desktop() {
    //handle messages
    std::thread refresh_cycle = desktop::instance().start_refresh_cycle();
    refresh_cycle.detach();

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (GetAsyncKeyState(settings::console::open_key)) {
            open_console();
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        std::this_thread::sleep_for(settings::internal::msg_input_delay);
    }
}