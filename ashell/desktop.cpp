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
#include "msdn_releaser.h"

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

    //init factory
    ID2D1Factory* tmp_pD2D1Factory = nullptr;
    auto res = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &tmp_pD2D1Factory
    );
    assert(res == S_OK);
    unq_factory.reset(tmp_pD2D1Factory);
    //init render target
    RECT rc;
    GetClientRect(hwnd, &rc);

    // Create a Direct2D render target			
    ID2D1HwndRenderTarget* temp_rt = nullptr;
    res = unq_factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            hwnd,
            D2D1::SizeU(
                rc.right - rc.left,
                rc.bottom - rc.top)
        ),
        &temp_rt
    );
    assert(res == S_OK);
    unq_render_target.reset(temp_rt);
    ShowWindow(hwnd, SW_RESTORE);
    UpdateWindow(hwnd);
}

void desktop::refresh_cycle(){
    SetLastError(0);//std::thread may set random 126 value
    size_t frame_counter = 0;
    auto second_start = std::chrono::high_resolution_clock::now();
    layers.add_layer(new static_image(L"bg.jpg", unq_render_target, {100, 0}));
    while (true) {
        auto lasterror = GetLastError();
        assert(lasterror == 0);
        auto frame_start = std::chrono::high_resolution_clock::now();
        frame_counter++;

        //draw
        unq_render_target->BeginDraw();
        layers.apply(unq_render_target);
        unq_render_target->EndDraw();

        //calculate graphics


        auto elapsed = std::chrono::high_resolution_clock::now() - frame_start;
        std::this_thread::sleep_for(settings::internal::delay - elapsed);
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - second_start).count() >= 1) {
            //every second
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