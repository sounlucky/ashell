#include "console_drawer.h"
#include "settings.h"
#include <cassert>

console_drawer::console_drawer(HWND hwnd) {
    dc = GetDC(hwnd);
    assert(dc);
}

void console_drawer::update_text(const std::wstring & str) const {
    RECT rect;
    SetRect(&rect, 
        0, 
        settings::console_height / 4, 
        settings::console_width_offset + settings::console_width, 
        settings::console_height_offset + settings::console_width_offset
    );
    DrawText(dc, str.c_str(), str.size(), &rect, DT_LEFT);
}
