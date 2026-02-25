#include "syntaxer/grammar.hpp"
#include "lexer/token.hpp"

namespace slr_parser {
namespace syntaxer   {

Grammar::Grammar()
{
    // S' -> E
    rules_.emplace_back(NonTerminalT::DODIK, std::initializer_list<Symbol>{NonTerminalT::EXPR});

    // E -> E + T | T
    rules_.emplace_back(NonTerminalT::EXPR, std::initializer_list<Symbol>{NonTerminalT::EXPR, lexer::Token::PLUS, NonTerminalT::TERM});
    rules_.emplace_back(NonTerminalT::EXPR, std::initializer_list<Symbol>{NonTerminalT::TERM});

    // T -> T * F
    // T -> F
    rules_.emplace_back(NonTerminalT::TERM, std::initializer_list<Symbol>{NonTerminalT::TERM, lexer::Token::MUL, NonTerminalT::FACTOR});
    rules_.emplace_back(NonTerminalT::TERM, std::initializer_list<Symbol>{NonTerminalT::FACTOR});

    // F -> (E)
    // F -> id
    // F -> num
    rules_.emplace_back(NonTerminalT::FACTOR, std::initializer_list<Symbol>{lexer::Token::LPAREN, lexer::Token::ID, lexer::Token::RPAREN});
    rules_.emplace_back(NonTerminalT::FACTOR, std::initializer_list<Symbol>{lexer::Token::ID});
    rules_.emplace_back(NonTerminalT::FACTOR, std::initializer_list<Symbol>{lexer::Token::NUM});
}

}
}