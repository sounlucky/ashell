#pragma once
#include <vector>

class token {
public:
    enum class type {
        command, argument
    };
};

class tokenizer
{
public:
    tokenizer() = default;
    std::vector<token> tokenize();
};

