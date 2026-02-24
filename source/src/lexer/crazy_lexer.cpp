#include "lexer/crazy_lexer.hpp"
#include "lexer/token.hpp"
#include <stdexcept>
#include <string>
#include <variant>

namespace slr_parser {
namespace lexer      {

void CrazyLexer::CreateToken_(Token::Type type, const char* attribute)
{
    Token::TokenAttributeT attr;

    switch (type) {
        case Token::NUM     : attr = std::atof  (attribute); break;
        case Token::ID      : attr = std::string(attribute);    break;
        case Token::EOF_    : attr = std::monostate{};             break;
        case Token::PLUS    : attr = std::monostate{};             break;
        case Token::MUL     : attr = std::monostate{};             break;
        case Token::LPAREN  : attr = std::monostate{};             break;
        case Token::RPAREN  : attr = std::monostate{};             break;
        case Token::UNKNOWN : attr = std::monostate{};             break;
        
        default: throw std::runtime_error("unknown Token::Type");
    };

    cur_token = std::make_unique<Token>(type, attr, cur_location_);
}

}
}