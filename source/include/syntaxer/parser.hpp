#pragma once

#include <vector>
#include <map>
#include <string>
#include <iomanip> // Для красивого вывода таблички
#include <memory>

#include "syntaxer/dfa_builder.hpp"
#include "syntaxer/grammar_worker.hpp"
#include "lexer/token.hpp"

namespace slr_parser {
namespace syntaxer   {

struct Action
{
    enum Type {
        Shift,
        Reduce,
        Accept,
        Error
    };

    Type type;
    int operand; // Для Shift -> номер нового состояния. Для Reduce -> номер правила.

    std::string ToString() const {
        switch (type) {
            case Type::Shift:  return "Shift "       + std::to_string(operand);
            case Type::Reduce: return "Reduce rule " + std::to_string(operand);
            case Type::Accept: return "Accept";
            case Type::Error:  return "Error";
        }
        return "";
    }
};

class Parser
{
public:
    Parser(const Grammar& grammar, const DfaBuilder& collection, const GrammarWorker::FirstFollow& ff);

    void Run(std::vector<std::unique_ptr<lexer::Token>> tokens);

private:
    const Grammar& grammar_;
    
    // Таблица ACTION: {StateID, Terminal} -> Action
    // Используем Symbol, так как терминалы у нас завернуты в Symbol
    std::map<std::pair<int, Grammar::Symbol>, Action> action_table_;
    
    std::map<std::pair<int, Grammar::Symbol>, int> goto_table_;

    void BuildTables(const DfaBuilder& collection, const GrammarWorker::FirstFollow& ff);

    std::string StackToString(const std::vector<int>& state_stack) const;

    std::map<int, std::string> state_symbol_map_;
    std::string GetSymbolName_(const Grammar::Symbol& sym) const;
};

} // namespace syntaxer
} // namespace slr_parser