#include <assert.h>
#include <string>
#include <limits>
#include <Windows.h>
#include <memory>
#include <set>
#include <d2d1.h>

using callback_ret_t = LRESULT CALLBACK;
typedef callback_ret_t(*msg_callback_t)(HWND, UINT, WPARAM, LPARAM);

class borderless_window
{
    HWND hwnd;
    RECT rect;//size and offset basically
    std::unique_ptr<WNDCLASSEX> wndclass_p;
    std::unique_ptr<ID2D1Factory> unq_factory;
    std::unique_ptr<ID2D1HwndRenderTarget> unq_render_target;

    using id_t = uint32_t;
    static std::set<id_t> class_ids;
    id_t id;
public:
    borderless_window(msg_callback_t);
    ~borderless_window() = default;
};
