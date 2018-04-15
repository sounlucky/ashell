#include "window.h"

extern HINSTANCE global_instance;

borderless_window::borderless_window(msg_callback_t callback) :
    wndclass_p(new WNDCLASSEX)
{
    wndclass_p->cbSize = sizeof(WNDCLASSEX);
    wndclass_p->style = CS_HREDRAW | CS_VREDRAW;
    wndclass_p->lpfnWndProc = reinterpret_cast<decltype(wndclass_p->lpfnWndProc)>(callback);
    wndclass_p->cbClsExtra = 0;
    wndclass_p->cbWndExtra = 0;
    wndclass_p->hInstance = global_instance;
    wndclass_p->hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndclass_p->hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndclass_p->hbrBackground = nullptr;
    wndclass_p->lpszMenuName = nullptr;
    wndclass_p->hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    {
        //generate id, redefine windows.h's max
#define max max
        auto iter = class_ids.begin();
        id_t i = 0;
        for (; i < std::numeric_limits<id_t>::max() - 1; ++i)
            if ((iter = class_ids.find(i)) == class_ids.cend())
                break;
        if (iter != class_ids.cend())
            throw std::runtime_error("too many open windows, out of ids");
        class_ids.emplace(i);
        wndclass_p->lpszClassName = (std::wstring(L"class_") + std::to_wstring(id)).c_str();
    }

    auto regged = RegisterClassEx(wndclass_p.get());
    assert(regged);
}


window::~window()
{
}
