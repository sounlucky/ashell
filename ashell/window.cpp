#include "window.h"

extern HINSTANCE global_instance;

std::set<id_t> borderless_window::class_ids = std::set<id_t>();

RECT get_monitorsize() {
    //https://stackoverflow.com/questions/2156212/how-to-get-the-monitor-screen-resolution-from-a-hwnd
    HMONITOR monitor = MonitorFromPoint({ 0,0 }, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    return info.rcMonitor;
}

borderless_window::borderless_window(msg_callback_t callback) :
    borderless_window(callback, get_monitorsize()) {
}

borderless_window::borderless_window(msg_callback_t callback, RECT rect) :
    wndclass_p(new WNDCLASSEX), 
    rect(rect), 
    hwnd(nullptr, hwnd_deleter())
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
        //generate id, redefine windows.h's max
#undef max
    auto iter = class_ids.begin();
    id = 0;
    for (; id < std::numeric_limits<id_t>::max() - 1; ++id)
        if ((iter = class_ids.find(id)) == class_ids.cend())
            break;
    if (iter != class_ids.cend())
        throw std::runtime_error("too many open windows, out of ids");
    class_ids.emplace(id);
    auto classname_copy = get_classname();
    wndclass_p->lpszClassName = classname_copy.c_str();

    auto regged = RegisterClassEx(wndclass_p.get());
    assert(regged);

    auto temp_hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        classname_copy.c_str(),
        nullptr,//no need for title (:
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        global_instance,
        nullptr);
    hwnd.reset(&temp_hwnd);
    assert(hwnd);

    MoveWindow(
        *hwnd.get(),
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        true
    );
    ShowWindow(*hwnd.get(), SW_RESTORE);
    UpdateWindow(*hwnd.get());

    //init factory  
    ID2D1Factory* tmp_pD2D1Factory = nullptr;
    auto res = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &tmp_pD2D1Factory
    );
    assert(res == S_OK);
    factory.reset(tmp_pD2D1Factory);

    //init Direct2D render target	
    RECT rc;
    GetClientRect(*hwnd.get(), &rc);
    ID2D1HwndRenderTarget* temp_rt = nullptr;
    res = factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            *hwnd.get(),
            D2D1::SizeU(
                rc.right - rc.left,
                rc.bottom - rc.top)
        ),
        &temp_rt
    );
    assert(res == S_OK);
    render_target.reset(temp_rt);
}

std::wstring borderless_window::get_classname() {
    return (std::wstring(L"class_") + std::to_wstring(id));
}
