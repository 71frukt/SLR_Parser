#include <queue>

#include "syntaxer/grammar_worker.hpp"
#include "syntaxer/grammar.hpp"

namespace slr_parser {
namespace syntaxer   {

Grammar::State GrammarWorker::GetClosure(const Grammar::State& core_items) const
{
    Grammar::State closure = core_items;

    std::deque<Grammar::Item> worklist(core_items.begin(), core_items.end());

    while (!worklist.empty())
    {
        Grammar::Item current_item = worklist.front();
        worklist.pop_front();

        const Grammar::Production& rule = grammar_.GetRules()[current_item.rule_num];

        if (current_item.dot_pos >= rule.rhs.size())
            continue;

        const Grammar::Symbol& symbol_after_dot = rule.rhs[current_item.dot_pos];

        if (symbol_after_dot.IsTerminal()) {
            continue;
        }

        Grammar::NonTerminalT non_term_to_expand = symbol_after_dot.AsNonTerminal();

        const auto& all_rules = grammar_.GetRules();

        for (size_t i = 0; i < all_rules.size(); ++i)
        {
            if (all_rules[i].lhs == non_term_to_expand)
            {
                Grammar::Item new_item;
                new_item.rule_num = i;
                new_item.dot_pos = 0;

                // set.insert().second == true if inserted successfully (if elem is new)
                if (closure.insert(new_item).second)
                    worklist.push_back(new_item);
            }
        }
    }

    return closure;
}

Grammar::State GrammarWorker::GetGoto(const Grammar::State& state, const Grammar::Symbol& symbol) const
{
    Grammar::State next_state_core;

    for (const Grammar::Item& item : state)
    {
        const Grammar::Production& rule = grammar_.GetRules()[item.rule_num];

        if (item.dot_pos >= rule.rhs.size())
            continue;

        const Grammar::Symbol& symbol_after_dot = rule.rhs[item.dot_pos];

        if (symbol_after_dot == symbol)
        {
            Grammar::Item next_item = item;
            next_item.dot_pos++;
            
            next_state_core.insert(next_item);
        }
    }

    if (next_state_core.empty()) {
        return {};
    }

    // GOTO(I, X) = CLOSURE(сдвинутые пункты)
    return GetClosure(next_state_core);
}


GrammarWorker::FirstFollow::FirstFollow(const Grammar& grammar)
    : grammar_(grammar)
{
    ComputeFirst_ ();
    ComputeFollow_();
}

const std::set<Grammar::Symbol>& GrammarWorker::FirstFollow::GetFirst(const Grammar::Symbol& sym) const
{
    if (sym.IsTerminal())
    {
        // Для терминала FIRST - это он сам.
        static std::map<Grammar::Symbol, std::set<Grammar::Symbol>> cache;

        if (cache.find(sym) == cache.end())
            cache[sym] = {sym};

        return cache[sym];
    }

    return first_.at(sym);
}


void GrammarWorker::FirstFollow::ComputeFirst_()
{
    bool changed = true;

    while (changed)
    {
        changed = false;
        
        for (const Grammar::Production& rule : grammar_.GetRules())
        {
            // rule: A -> RightSide
            Grammar::NonTerminalT A = rule.lhs;
            Grammar::Symbol sym_A(A);

            std::set<Grammar::Symbol> current_first = GetFirstForSequence_(rule.rhs);
            
            size_t size_before = first_[sym_A].size();
            first_[sym_A].insert(current_first.begin(), current_first.end());
            
            if (first_[sym_A].size() > size_before)
                changed = true;
        }
    }
}

std::set<Grammar::Symbol> GrammarWorker::FirstFollow::GetFirstForSequence_(const std::vector<Grammar::Symbol>& seq) const
{
    if (seq.empty())
        return {};
    
    Grammar::Symbol head = seq[0];
    
    if (head.IsTerminal())
        return {head};

    auto it = first_.find(head);
    if (it == first_.end())
        return {};

    return it->second; 
}

const std::set<Grammar::Symbol>& GrammarWorker::FirstFollow::GetFollow(Grammar::NonTerminalT nt) const {
    if (follow_.find(nt) == follow_.end()) {
        static const std::set<Grammar::Symbol> empty;
        return empty;
    }
    return follow_.at(nt);
}


void GrammarWorker::FirstFollow::ComputeFollow_()
{
    follow_[Grammar::NonTerminalT::DODIK].insert(EndMarker());

    bool changed = true;

    while (changed)
    {
        changed = false;

        for (const Grammar::Production& rule : grammar_.GetRules())
        {
            Grammar::NonTerminalT A = rule.lhs;

            const auto& rhs = rule.rhs;

            for (size_t i = 0; i < rhs.size(); ++i)
            {
                Grammar::Symbol B = rhs[i];

                if (B.IsTerminal())
                    continue;
                
                Grammar::NonTerminalT B_nt = B.AsNonTerminal();
                size_t size_before = follow_[B_nt].size();

                std::vector<Grammar::Symbol> beta;

                for (size_t k = i + 1; k < rhs.size(); ++k)
                    beta.push_back(rhs[k]);

                if (!beta.empty())
                {
                    std::set<Grammar::Symbol> first_beta = GetFirstForSequence_(beta);
                    follow_[B_nt].insert(first_beta.begin(), first_beta.end());
                }

                if (beta.empty())
                {
                    const auto& follow_A = follow_[A];
                    follow_[B_nt].insert(follow_A.begin(), follow_A.end());
                }

                if (follow_[B_nt].size() > size_before) {
                    changed = true;
                }
            }
        }
    }
}



}
}