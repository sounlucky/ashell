#pragma once
#include <string>
#include <future>

class console_parser
{
    option_type define_option(std::wstring&);
public:
    console_parser() = default;
    ~console_parser() = default;

    std::packaged_task<> parse(std::wstring&);
};

