#pragma once

//https://msdn.microsoft.com/en-us/library/windows/desktop/dd940435(v=vs.85).aspx
template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}