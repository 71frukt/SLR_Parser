#pragma once

#include <cstddef>
#include <stdexcept>
#include <variant>
#include <vector>
#include <set>

#include "lexer/token.hpp"

namespace slr_parser {
namespace syntaxer   {


class Grammar
{
public:
    Grammar();

    enum class NonTerminalT {
        DODIK,
        EXPR,
        TERM,
        FACTOR
    };

    struct Symbol;
    struct Production;
    struct Item;
    using State = std::set<Item>;
    
    [[nodiscard]] const std::vector<Production>& GetRules() const noexcept { return rules_; }

private:
    std::vector<Production> rules_ ;
};


struct Grammar::Symbol
{
public:
    Symbol(lexer::Token::Type token_type  ) : is_terminal_(true ), type_(token_type  ) {}
    Symbol(NonTerminalT       non_terminal) : is_terminal_(false), type_(non_terminal) {}    

    [[nodiscard]] NonTerminalT AsNonTerminal() const
    {
        if (is_terminal_)
            throw std::runtime_error("Attempt to cast terminal to non-terminal");
        return std::get<NonTerminalT>(type_);
    }

    [[nodiscard]] lexer::Token::Type AsTerminal() const
    {
        if (!is_terminal_)
            throw std::runtime_error("Attempt to cast non-terminal to terminal");
        return std::get<lexer::Token::Type>(type_);
    }

    [[nodiscard]] bool IsTerminal() const noexcept { return is_terminal_; }

    bool operator==(const Symbol& other) const
    {
        return is_terminal_ == other.is_terminal_ && type_ == other.type_;
    }


    bool operator<(const Symbol& other) const
    {
        // 1. Сначала сравниваем флаг терминальности
        if (is_terminal_ != other.is_terminal_) {
            return is_terminal_ < other.is_terminal_;
        }
        // 2. Если типы равны, сравниваем содержимое variant
        return type_ < other.type_;
    }

private:
    bool is_terminal_;
    std::variant<NonTerminalT, lexer::Token::Type> type_;
};


struct Grammar::Production
{
    NonTerminalT lhs;
    std::vector<Symbol> rhs;
    
    Production(NonTerminalT l, std::initializer_list<Symbol> r) : lhs(l), rhs(r) {}
};


struct Grammar::Item
{
    size_t rule_num;
    size_t dot_pos;


    // for std::set
    [[nodiscard]] bool operator<(const Item& other) const noexcept  
    {
        if (rule_num != other.rule_num) return rule_num < other.rule_num;
        return dot_pos < other.dot_pos;
    }

    // for std::set
    [[nodiscard]] bool operator==(const Item& other) const noexcept
    {
        return rule_num == other.rule_num && dot_pos == other.dot_pos;
    }
};


}
}