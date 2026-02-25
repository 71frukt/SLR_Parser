#pragma once

#include <map>

#include "syntaxer/grammar.hpp"

namespace slr_parser {
namespace syntaxer   {

class GrammarWorker
{
public:
    explicit GrammarWorker(const Grammar& grammar)
        : grammar_(grammar)
    {}

    Grammar::State GetClosure (const Grammar::State& core_items)                           const;
    Grammar::State GetGoto    (const Grammar::State& state, const Grammar::Symbol& symbol) const;

    class FirstFollow;

    [[nodiscard]] const Grammar& GetGrammar() const noexcept { return grammar_; }
private:
    const Grammar& grammar_;
};



class GrammarWorker::FirstFollow
{
public:
    explicit FirstFollow(const Grammar& grammar);

    const std::set<Grammar::Symbol>& GetFirst(const Grammar::Symbol& s) const;
    
    // Получить множество FOLLOW для нетерминала
    const std::set<Grammar::Symbol>& GetFollow(Grammar::NonTerminalT nt) const;

    // Специальный маркер конца ввода ($)
    static Grammar::Symbol EndMarker() { return Grammar::Symbol(lexer::Token::EOF_); }

private:
    const Grammar& grammar_;
    
    std::map<Grammar::Symbol      , std::set<Grammar::Symbol>> first_;
    std::map<Grammar::NonTerminalT, std::set<Grammar::Symbol>> follow_;

    // Вспомогательная функция: FIRST для цепочки символов (beta)

    std::set<Grammar::Symbol> GetFirstForSequence_(const std::vector<Grammar::Symbol>& sequence) const;

    void ComputeFirst_ ();
    void ComputeFollow_();
};


}
}