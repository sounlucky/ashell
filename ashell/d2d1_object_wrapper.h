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
    
    template<class ctr_t>
    d2d1_unique_object(ctr_t arg):
        ptr(
            arg,
            [](T* item) {
                SafeRelease(&item);
            })
    {}

    T* operator->() const {
        return ptr.get();
    }
};

template<class T>
class d2d1_shared_object {
    std::shared_ptr<T, void(*)(T*)> ptr;

    template<class ctr_t>
    d2d1_unique_object(ctr_t arg) :
        ptr(
            arg,
            [](T* item) {
        SafeRelease(&item);
    })
    {}

    T* operator->() const {
        return ptr.get();
    }
};