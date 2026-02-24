#pragma once

#include <cstddef>
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <memory>

#include "lexer/token.hpp"

namespace slr_parser {
namespace lexer {


class CrazyLexer : public yyFlexLexer
{
public:
    explicit CrazyLexer(std::istream *in) : yyFlexLexer(in) {};
    virtual ~CrazyLexer() {};

    virtual int yylex() override;
    std::unique_ptr<Token> cur_token;

private:
    void CreateToken_(Token::Type type, const char* attribute);

    Token::CodeLocation cur_location_;  // location before cur token
    size_t real_cur_column_ = 1;

    void UpdateCurLocation_();
};

}
}
