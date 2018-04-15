#pragma once
#include <windows.h>
#include <string>
#include <atomic>
#include <memory>
#include <thread>
#include <cassert>
#include <mutex>
#include "dbg.h"
#include "desktop.h"
#include "layer_hierarchy.h"
#include "static_image.h"
#include "settings.h"
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
    ~console();
};

void open_console();