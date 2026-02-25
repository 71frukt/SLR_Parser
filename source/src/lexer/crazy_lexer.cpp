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
        
        default             : attr = std::monostate{};             break;
    };

    cur_token = std::make_unique<Token>(type, attr, cur_location_);
}

}
}