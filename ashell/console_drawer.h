#pragma once
#include <windows.h>
#include <string>

class console_drawer
{
    HDC dc;
    RECT rect;
    HPEN pen;
public:
    console_drawer(HWND);
    ~console_drawer();

    void update_text(const std::wstring&);
};

