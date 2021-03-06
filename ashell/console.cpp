#include "console.h"

extern HINSTANCE global_instance;

//helper class to fight data races
class console_access_manager {
    std::unique_ptr<console> console_ptr;
    std::recursive_mutex console_access;

    console_access_manager() : console_ptr(nullptr) {}
public:
    std::atomic<bool> console_routine_running = false;//should be replaced with std::atomic_lock

    static console_access_manager& access() {
        static console_access_manager manager;
        return manager;
    }

    bool console_running() {
        std::lock_guard<decltype(console_access)> guard(console_access);
        return console_ptr != nullptr;
    }

    void exit() {
        std::lock_guard<decltype(console_access)> guard(console_access);
        if (console_ptr.get() != nullptr)
            console_ptr.reset(nullptr);
    }

    void create() {
        std::lock_guard<decltype(console_access)> guard(console_access);
        if (console_ptr == nullptr)
            console_ptr.reset(new console);
    }

    template <class F>
    void use(F&& f) {
        std::lock_guard<decltype(console_access)> guard(console_access);
        if (console_ptr.get() != nullptr)
            f(*console_ptr.get());
    }
};

void console::handle_keypress(wchar_t key_code){
    bool legit_space = key_code == L' ' && !content.empty() && content.back() != L' ';
    if (iswalpha(key_code) || legit_space) {
        content += key_code;
        drawer->update_text(content);
    }
}

LRESULT CALLBACK console::msg_callback
(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
    switch (msg) {
    case WM_KILLFOCUS:
        console_access_manager::access().exit();
            //console_ptr.reset(nullptr);
        return true;
    case WM_KEYDOWN:
        if (wParam == settings::console::open_key) {
            console_access_manager::access().exit();
        }
        else {
            console_access_manager::access().use([&](auto& console){
                console.handle_keypress(wParam);
            });
        }
        return true;
    case WM_PAINT://to ignore (:
    case WM_CLOSE:
    case WM_DESTROY:
        return true;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}


console::~console(){    
    console_access_manager::access().console_routine_running = false;
    while (!console_access_manager::access().console_routine_running)
        ;
    console_access_manager::access().console_routine_running = false;
    DestroyWindow(hwnd);
}

void console_routine() {
    console_access_manager::access().console_routine_running = true;
    MSG msg; 
    while (true) {
        PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
        if (!console_access_manager::access().console_routine_running) {
            std::this_thread::sleep_for(settings::console::fade);
            console_access_manager::access().console_routine_running = true;
            return;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        std::this_thread::sleep_for(settings::internal::msg_input_delay);
    }
}

void open_console(){
    if (console_access_manager::access().console_running()) {
        return;
    }
    console_access_manager::access().create();
    std::thread console_routine_thread(console_routine);
    console_routine_thread.detach();
}