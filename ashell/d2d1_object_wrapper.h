#pragma once

//https://msdn.microsoft.com/en-us/library/windows/desktop/dd940435(v=vs.85).aspx
template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}

template<class T>
class d2d1_unique_object {
    std::unique_ptr<T, void(*)(T*)> ptr;
public:
    d2d1_unique_object() :
        d2d1_unique_object(nullptr)
    {}

    template<class ctr_t>
    d2d1_unique_object(ctr_t arg):
        ptr(
            arg,
            [](T* item) {
                SafeRelease(&item);
            })
    {}

    void reset(T* new_ptr) {
        ptr.reset(new_ptr);
    }

    T* get() {
        return ptr.get();
    }

    T* operator->() const throw() {
        return ptr.get();
    }
};