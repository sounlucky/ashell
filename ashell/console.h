#pragma once
#include <windows.h>
#include "console_drawer.h"
#include <string>
#include <atomic>

class console
{
    friend class console_drawer;

    HWND hwnd;
    console_drawer drawer;
    std::wstring content;
    void handle_keypress(wchar_t);
public:
    static LRESULT CALLBACK msg_callback(HWND, UINT, WPARAM, LPARAM);
    console();
    ~console();
};

void open_console();