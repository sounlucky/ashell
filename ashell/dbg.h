#pragma once
#include <debugapi.h>
#include <string>

struct dbg_t_helper;

template <class T>
void dbg_print(T str) {
    OutputDebugString(str);
}

struct dbg_t_helper {};

const auto dbg = dbg_t_helper{};

namespace std {
    wstring inline to_wstring(const wchar_t* c) {
        return wstring(c);
    }

    wstring inline to_wstring(const char* c) {//https://stackoverflow.com/questions/8032080/how-to-convert-char-to-wchar-t
        std::wstring res(strlen(c), 0);
        mbstowcs(&res[0], c, res.length());
        return res;
    }

    wstring inline to_wstring(char c) {//https://stackoverflow.com/questions/8032080/how-to-convert-char-to-wchar-t
        std::wstring res(1, 0);
        mbstowcs(&res[0], &c, 1);
        return res;
    }
}

template<class T>
dbg_t_helper operator<<(dbg_t_helper, T e){
    dbg_print(std::to_wstring(e).c_str());
    return {};
}
