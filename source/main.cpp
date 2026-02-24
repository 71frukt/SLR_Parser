#include <iostream>
#include "lexer/token.hpp"

#include "lexer/crazy_lexer.hpp"
int main()
{
   
    slr_parser::lexer::CrazyLexer lexer(&std::cin);

    int token_type;

    while ((token_type = lexer.yylex()) != slr_parser::lexer::Token::EOF_)
    {
        std::cout << "Got token ID: " << token_type << std::endl;

        auto& token = *lexer.cur_token; // ссылка на объект внутри unique_ptr

        std::cout << "Token ID: " << token_type 
                  << " [L:" << token.code_place.line 
                  << ", C:" << token.code_place.column << "]";

        if (token_type == slr_parser::lexer::Token::ID)
        {
            std::cout << "attr: " << std::get<slr_parser::lexer::Token::VarT>(token.attr) << std::endl;
        }

        else if (token_type == slr_parser::lexer::Token::NUM)
        {
            std::cout << "attr: " << std::get<slr_parser::lexer::Token::NumT>(token.attr) << std::endl;
        }
    }
    return 0;
}