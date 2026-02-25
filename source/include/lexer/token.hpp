#pragma once

#include <iostream>
#include <variant>

namespace slr_parser {
namespace lexer      {

struct Token
{
public:
    using VarT = std::string;
    using NumT = double;
    using TokenAttributeT = std::variant<VarT, NumT, std::monostate>;

    enum Type {
        EOF_ = 0,
        NUM = 258,
        ID,
        PLUS,
        MINUS,
        MUL,
        DIV,
        LPAREN,
        RPAREN,
        UNKNOWN
    };

    struct CodeLocation
    {
        size_t column = 777;
        size_t line    = 999;
    };

public:
    Token(const Type token_type, const TokenAttributeT token_attr, const CodeLocation token_code_place)
        : type       (token_type)
        , attr       (token_attr)
        , code_place (token_code_place)
    {}

public:
    const Type type;
    const TokenAttributeT attr;
    const CodeLocation code_place;
};

}
}