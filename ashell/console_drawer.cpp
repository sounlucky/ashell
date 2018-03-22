#include <cassert>
#include "console_drawer.h"
#include "settings.h"

console_drawer::console_drawer(HWND hwnd) :
    dc(GetDC(hwnd)),
    rect{
        static_cast<LONG>(settings::console::width_offset),
        static_cast<LONG>(settings::console::height_offset + settings::console::height / 4),
        static_cast<LONG>(settings::console::width_offset + settings::console::width),
        static_cast<LONG>(settings::console::height_offset + settings::console::height)
    }
{
    assert(dc);
    LOGBRUSH logbrush { BS_NULL, DIB_RGB_COLORS, 0 };
    pen = ExtCreatePen(PS_GEOMETRIC, PS_NULL, &logbrush, 0, nullptr);
    SelectObject(dc, pen);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, settings::console::text_color);
    Rectangle(dc, 
        settings::console::width_offset, 
        settings::console::height_offset, 
        settings::console::width_offset + settings::console::width, 
        settings::console::height_offset + settings::console::height
    );
}

console_drawer::~console_drawer() {
    DeleteObject(pen);
}

void console_drawer::update_text(const std::wstring & str) {
    DrawText(dc, str.c_str(), static_cast<int>(str.size()), &rect, DT_LEFT);
}
