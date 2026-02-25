#include "syntaxer/parser.hpp"
#include <iostream>
#include <iomanip>
#include <string>

namespace slr_parser {
namespace syntaxer   {

// --- 1. Вспомогательная функция для красивых имен ---
std::string Parser::GetSymbolName_(const Grammar::Symbol& sym) const
{
    if (sym.IsTerminal()) {
        auto type = sym.AsTerminal();
        switch (type) {
            case lexer::Token::ID:     return "id";
            case lexer::Token::NUM:    return "num";
            case lexer::Token::PLUS:   return "+";
            case lexer::Token::MINUS:  return "-";
            case lexer::Token::MUL:    return "*";
            case lexer::Token::DIV:    return "/";
            case lexer::Token::LPAREN: return "(";
            case lexer::Token::RPAREN: return ")";
            case lexer::Token::EOF_:   return "$";
            default: return "term";
        }
    } else {
        auto nt = sym.AsNonTerminal();
        switch (nt) {
            case Grammar::NonTerminalT::EXPR:   return "E";
            case Grammar::NonTerminalT::TERM:   return "T";
            case Grammar::NonTerminalT::FACTOR: return "F";
            case Grammar::NonTerminalT::DODIK:  return "S'";
            default: return "NT";
        }
    }
}

Parser::Parser(const Grammar& grammar, const DfaBuilder& col, const GrammarWorker::FirstFollow& ff) 
    : grammar_(grammar)
{
    BuildTables(col, ff);
}

void Parser::BuildTables(const DfaBuilder& col, const GrammarWorker::FirstFollow& ff) {
    const auto& states = col.GetStates();
    const auto& transitions = col.GetTransitions();

    state_symbol_map_[0] = "$"; 

    for (size_t i = 0; i < states.size(); ++i)
    {
        int state_idx = static_cast<int>(i);
        const Grammar::State& state = states[i];

        for (const auto& [key, next_state] : transitions)
        {
            if (key.first != state_idx) continue;
            
            Grammar::Symbol symbol = key.second;

            state_symbol_map_[next_state] = GetSymbolName_(symbol);
            
            if (symbol.IsTerminal())
                action_table_[{state_idx, symbol}] = {Action::Type::Shift, next_state};
            
            else
                goto_table_[{state_idx, symbol}] = next_state;
        }

        for (const auto& item : state)
        {
            const auto& rule = grammar_.GetRules()[item.rule_num];

            if (item.dot_pos >= rule.rhs.size())
            {
                if (rule.lhs == Grammar::NonTerminalT::DODIK)
                {
                    Grammar::Symbol eof = GrammarWorker::FirstFollow::EndMarker();
                    action_table_[{state_idx, eof}] = {Action::Type::Accept, 0};
                } 
                else
                {
                    const auto& follow_set = ff.GetFollow(rule.lhs);
                    for (const auto& term : follow_set)
                    {
                        action_table_[{state_idx, term}] = {Action::Type::Reduce, static_cast<int>(item.rule_num)};
                    }
                }
            }
        }
    }
}

void Parser::Run(std::vector<std::unique_ptr<lexer::Token>> tokens)
{
    std::vector<int> stack = {0};
    size_t cursor = 0;

    std::cout << std::left 
              << std::setw(30) << "STACK" 
              << std::setw(40) << "INPUT" 
              << "ACTION" << std::endl;
    std::cout << std::string(90, '-') << std::endl;

    while (true) {
        int current_state = stack.back();
        
        Grammar::Symbol current_sym = (cursor < tokens.size()) 
                             ? Grammar::Symbol(tokens[cursor]->type) 
                             : GrammarWorker::FirstFollow::EndMarker();

        Action action = {Action::Type::Error, 0};
        auto it = action_table_.find({current_state, current_sym});

        if (it != action_table_.end())
            action = it->second;

        std::string input_str;
        for (size_t k = cursor; k < tokens.size(); ++k) 
        {
            auto& t = tokens[k];
            if (t->type == lexer::Token::ID)
                try { input_str += std::get<std::string>(t->attr) + " "; } catch(...) { input_str += "id "; }
            
            else if (t->type == lexer::Token::NUM)
                 try { input_str += std::to_string(std::get<double>(t->attr)) + " "; } catch(...) { input_str += "num "; }
            
            else if (t->type == lexer::Token::EOF_)
                input_str += "$";
            
            else
                input_str += GetSymbolName_(Grammar::Symbol(t->type)) + " "; 
        }

        std::cout << std::left 
                  << std::setw(30) << StackToString(stack) 
                  << std::setw(40) << input_str
                  << action.ToString() << std::endl;

        if (action.type == Action::Type::Shift)
        {
            stack.push_back(action.operand);
            cursor++;
        }
        else if (action.type == Action::Type::Reduce)
        {
            const auto& rule = grammar_.GetRules()[action.operand];
            size_t len = rule.rhs.size();
            
            for(size_t k=0; k < len; ++k) stack.pop_back();
            
            int top_state = stack.back();
            
            if (goto_table_.count({top_state, Grammar::Symbol(rule.lhs)}))
            {
                int next_state = goto_table_.at({top_state, Grammar::Symbol(rule.lhs)});
                stack.push_back(next_state);
            }
            else
            {
                std::cerr << "CRITICAL ERROR: No GOTO entry after reduce!" << std::endl;
                return;
            }
        }
        else if (action.type == Action::Type::Accept)
        {
            std::cout << "[[SUCCESS]]" << std::endl;
            return;
        }
        else
        {
            std::cerr << "Syntax Error!" << std::endl;
            return;
        }
    }
}

std::string Parser::StackToString(const std::vector<int>& stack) const {
    std::string s = "";
    for (int st : stack)
    {
        if (state_symbol_map_.count(st))
            s += state_symbol_map_.at(st);
        
        else
            s += std::to_string(st);
        
        s += " ";
    }
    
    return s;
}

} // namespace syntaxer
} // namespace slr_parser