#pragma once
#include <chrono>

namespace {
    namespace settings {
        namespace internal{
            auto msg_input_delay = std::chrono::milliseconds(10);//experimentally
            auto delay = std::chrono::milliseconds(42); //24 fps is 1/24 spf is 1000/24 ~ 42 mspf
            auto fps = 24;
        }
        namespace user {
            wchar_t* background_name = L"bg.jpg";
        }
        
        namespace console {
            auto open_key = VK_OEM_3;
            auto background_color = RGB(0xFF - 50, 0xFF - 50, 0xFF);//blue-ish
            auto text_color = RGB(0, 0, 0);
            auto width_offset = system::display_width * 0.25f / 2;
            auto width = system::display_width * 0.75f;
            auto height_offset = 30;
            auto height = 40;
            auto fade = std::chrono::milliseconds(100);
        }
    }
}

