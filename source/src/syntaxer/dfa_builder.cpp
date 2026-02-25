#include <iostream>

#include "syntaxer/dfa_builder.hpp"
#include "syntaxer/grammar.hpp"


namespace slr_parser {
namespace syntaxer   {

int DfaBuilder::FindStateIndex_(const Grammar::State& state) const
{
    for (size_t i = 0; i < states_.size(); ++i)
    {
        if (states_[i] == state)
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

void DfaBuilder::Build()
{
    Grammar::Item  start_item {0, 0};
    Grammar::State start_state = parser_.GetClosure({start_item});

    states_.push_back(start_state);

    for (size_t i = 0; i < states_.size(); ++i)
    {
        const Grammar::State current_state = states_[i];

        std::set<Grammar::Symbol> transition_symbols;

        for (const Grammar::Item& item : current_state)
        {
            const Grammar::Production& rule = parser_.GetGrammar().GetRules()[item.rule_num];
            
            if (item.dot_pos < rule.rhs.size())
                transition_symbols.insert(rule.rhs[item.dot_pos]);
        }

        for (const Grammar::Symbol& symbol : transition_symbols)
        {
            Grammar::State next_state = parser_.GetGoto(current_state, symbol);

            if (next_state.empty())
                continue;

            int target_index = FindStateIndex_(next_state);

            if (target_index == -1)
            {
                target_index = static_cast<int>(states_.size());
                states_.push_back(next_state);
            }

            transitions_[{static_cast<int>(i), symbol}] = target_index;
        }
    }
}

} // namespace slr_parser
} // namespace syntaxer