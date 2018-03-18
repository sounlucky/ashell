#pragma once
#include <chrono>


namespace {
    namespace settings {
        auto width = 1366;
        auto height = 768;
        
        auto msg_input_delay = std::chrono::milliseconds(10);//experimentally
        auto delay = std::chrono::milliseconds(42); //24 fps is 1/24 spf is 1000/24 ~ 42 mspf
        auto fps = 24;

        wchar_t* background_name = L"bg.jpg";
        
        auto open_console_key = VK_OEM_3;
        auto console_width_offset = width * 0.25f / 2;
        auto console_width = width * 0.75f;
        auto console_height_offset = 30;
        auto console_height = 40;
        auto console_fade = std::chrono::milliseconds(100);
    }
}

