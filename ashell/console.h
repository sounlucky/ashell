#pragma once
#include <windows.h>
#include <string>
#include <atomic>
#include <memory>
#include "console_drawer.h"

class console
{
    friend class console_drawer;

    HWND hwnd;
    std::unique_ptr<console_drawer> drawer;
    std::wstring content;
    void handle_keypress(wchar_t);
public:
    static LRESULT CALLBACK msg_callback(HWND, UINT, WPARAM, LPARAM);
    console();
    ~console();
};

void open_console();