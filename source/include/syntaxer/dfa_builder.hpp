#pragma once

#include "syntaxer/grammar.hpp"
#include "syntaxer/grammar_worker.hpp"
#include <vector>
#include <map>

namespace slr_parser {
namespace syntaxer {

class DfaBuilder {
public:
    explicit DfaBuilder(const GrammarWorker& grammar_worker)
        : parser_(grammar_worker)
    {}

    void Build();

    const std::vector<Grammar::State>& GetStates() const { return states_; }
    
    // Карта переходов (Граф).
    // Ключ: {Номер_Откуда, Символ_По_Которому_Идем}
    // Значение: Номер_Куда_Пришли
    const std::map<std::pair<int, Grammar::Symbol>, int>& GetTransitions() const { return transitions_; }

private:
    const GrammarWorker& parser_;

    std::vector<Grammar::State> states_;
    std::map<std::pair<int, Grammar::Symbol>, int> transitions_;

    int FindStateIndex_(const Grammar::State& state) const;
};

} // namespace grammar_worker
} // namespace slr_parser