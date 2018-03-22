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

desktop::desktop() {
    using namespace Gdiplus;

    std::unique_ptr<Bitmap> background;
    std::unique_ptr<Bitmap> originalBitmap(Bitmap::FromFile(L"bg.jpg"));

    //load and scale
    float horizontalScalingFactor = static_cast<float>(settings::system::display_width) / originalBitmap->GetWidth();
    float verticalScalingFactor = static_cast<float>(settings::system::display_height) / originalBitmap->GetHeight();
    background.reset(new Bitmap(settings::system::display_width, settings::system::display_height));

    Graphics g(background.get());
    g.ScaleTransform(horizontalScalingFactor, verticalScalingFactor);
    g.DrawImage(originalBitmap.get(), 0, 0);
    Status s1 = background.get()->GetHBITMAP(Color::Black, &h_backgroundbitmap);
    assert(s1 == Ok);

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

void desktop::update(HDC device){
    SelectObject(device, h_backgroundbitmap);
    auto temp = CreateCompatibleDC(main_device);
   
    DeleteDC(temp);
}

void desktop::refresh_cycle(){
    //main + two buffers
    SetLastError(0);//std::thread may set random 126 value
    auto front_device = CreateCompatibleDC(main_device);
    decltype(front_device) back_device;

    size_t frame_counter = 0;
    auto second_start = std::chrono::high_resolution_clock::now();
    while (true) {
        auto lasterror = GetLastError();
        assert(lasterror == 0);

        auto frame_start = std::chrono::high_resolution_clock::now();
        frame_counter++;

        //draw
        BitBlt(main_device, 0, 0, settings::system::display_width, settings::system::display_height,
            front_device, 0, 0, SRCCOPY
        );
        DeleteDC(front_device);

        //calculate graphics
        back_device = CreateCompatibleDC(main_device);
        update(back_device);

        std::swap(front_device, back_device);

        auto elapsed = std::chrono::high_resolution_clock::now() - frame_start;
        //dbg << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << '\n';
        std::this_thread::sleep_for(settings::internal::delay - elapsed);
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - second_start).count() >= 1) {
            if (frame_counter < settings::internal::fps)
                dbg << "fps:[" << frame_counter << "]\n";
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