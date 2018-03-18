#pragma once
#include <windows.h>
#include <string>

class console_drawer
{
    HDC dc;
public:
    console_drawer(HWND);
    void update_text(const std::wstring&) const;
};

