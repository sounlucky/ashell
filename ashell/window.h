#pragma once
#include <assert.h>
#include <string>
#include <limits>
#include <memory>
#include <set>
#include <d2d1.h>
#include <windows.h>
#include "dbg.h"
#include "d2d1_object_wrapper.h"
#include "static_image.h"

using callback_ret_t = LRESULT CALLBACK;
typedef callback_ret_t(*msg_callback_t)(HWND, UINT, WPARAM, LPARAM);

using id_t = uint16_t;

class borderless_window
{
    HWND hwnd;
    RECT rect;
    std::unique_ptr<WNDCLASSEX> wndclass_p;
    d2d1_unique_object<ID2D1Factory> factory;
    d2d1_unique_object<ID2D1HwndRenderTarget> render_target;

    static std::set<id_t> class_ids;
    id_t id;

    std::wstring get_classname();
public:
    borderless_window(msg_callback_t, D2D1_SIZE_U);
    borderless_window(msg_callback_t);//fullscreen
    ~borderless_window() = default;
};