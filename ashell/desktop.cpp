#include "desktop.h"

void desktop::refresh_cycle(){
    SetLastError(0);//std::thread may set random 126 value
    size_t frame_counter = 0;
    auto second_start = std::chrono::high_resolution_clock::now();
    while (true) {
        auto lasterror = GetLastError();
        assert(lasterror == 0);
        auto frame_start = std::chrono::high_resolution_clock::now();
        frame_counter++;

        //draw

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