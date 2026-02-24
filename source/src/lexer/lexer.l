%{
#include <iostream>
#include <string>

#include "lexer/crazy_lexer.hpp"
%}

%option c++
%option noyywrap

    // count lines
%option yylineno
%option yyclass="slr_parser::lexer::CrazyLexer"

%{
#define YY_USER_ACTION this->UpdateCurLocation_();
%}

DIGIT   (0|[1-9][0-9]*)(\.[0-9]+)?
ALPHA   [a-zA-Z]
WS      [ \t\r]

%{
void slr_parser::lexer::CrazyLexer::UpdateCurLocation_()
{
    cur_location_.line   = lineno();
    cur_location_.column = real_cur_column_;

    // yyleng - длина совпадения (автоматическая переменная Flex)
    real_cur_column_ += yyleng;
}
%}


%%

{WS}+       { /* skip */ }
\n          { this->real_cur_column_ = 1; }

{DIGIT}     { this->CreateToken_(slr_parser::lexer::Token::NUM     , YYText()); return slr_parser::lexer::Token::NUM     ; }
{ALPHA}+    { this->CreateToken_(slr_parser::lexer::Token::ID      , YYText()); return slr_parser::lexer::Token::ID      ; }
"+"         { this->CreateToken_(slr_parser::lexer::Token::PLUS    , YYText()); return slr_parser::lexer::Token::PLUS    ; }
"*"         { this->CreateToken_(slr_parser::lexer::Token::MUL     , YYText()); return slr_parser::lexer::Token::MUL     ; }
"("         { this->CreateToken_(slr_parser::lexer::Token::LPAREN  , YYText()); return slr_parser::lexer::Token::LPAREN  ; }
")"         { this->CreateToken_(slr_parser::lexer::Token::RPAREN  , YYText()); return slr_parser::lexer::Token::RPAREN  ; }
.           { this->CreateToken_(slr_parser::lexer::Token::UNKNOWN , YYText()); return slr_parser::lexer::Token::UNKNOWN ; }

%%