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
#define YY_USER_ACTION UpdateCurLocation_();
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
\n          { real_cur_column_ = 1; }

{DIGIT}     { CreateToken_(slr_parser::lexer::Token::NUM     , YYText()); return slr_parser::lexer::Token::NUM     ; }
{ALPHA}+    { CreateToken_(slr_parser::lexer::Token::ID      , YYText()); return slr_parser::lexer::Token::ID      ; }
"+"         { CreateToken_(slr_parser::lexer::Token::PLUS    , YYText()); return slr_parser::lexer::Token::PLUS    ; }
"-"         { CreateToken_(slr_parser::lexer::Token::MINUS   , YYText()); return slr_parser::lexer::Token::MINUS   ; }
"*"         { CreateToken_(slr_parser::lexer::Token::MUL     , YYText()); return slr_parser::lexer::Token::MUL     ; }
"/"         { CreateToken_(slr_parser::lexer::Token::DIV     , YYText()); return slr_parser::lexer::Token::DIV     ; }
"("         { CreateToken_(slr_parser::lexer::Token::LPAREN  , YYText()); return slr_parser::lexer::Token::LPAREN  ; }
")"         { CreateToken_(slr_parser::lexer::Token::RPAREN  , YYText()); return slr_parser::lexer::Token::RPAREN  ; }
.           { CreateToken_(slr_parser::lexer::Token::UNKNOWN , YYText()); return slr_parser::lexer::Token::UNKNOWN ; }

%%