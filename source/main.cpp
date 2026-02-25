#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

#include "command_line_args.hpp"

#include "lexer/crazy_lexer.hpp"
#include "lexer/token.hpp"
#include "syntaxer/grammar.hpp"
#include "syntaxer/grammar_worker.hpp"
#include "syntaxer/dfa_builder.hpp"
#include "syntaxer/parser.hpp"


int main(const int argc, const char *argv[]) try {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    slr_parser::AppSettings settings = {};
    if (slr_parser::ParseCommandLineArgs(settings, argc, argv) == false)
    {
        return 0;
    }

    std::istream* input_stream = nullptr;
    std::unique_ptr<std::ifstream> file_storage;

    if (!settings.filepath.empty())
    {
        file_storage = std::make_unique<std::ifstream>(settings.filepath);

        if (!file_storage->is_open())
        {
            throw std::runtime_error("Could not open file: " + settings.filepath);
        }
        
        input_stream = file_storage.get();
    }
    else
    {
        input_stream = &std::cin;
    }
    
    
    std::cout << "Enter the code:" << std::endl;

    slr_parser::lexer::CrazyLexer lexer(input_stream);

    std::vector<std::unique_ptr<slr_parser::lexer::Token>> tokens;

    while (true)
    {
        int type = lexer.yylex();
        
        if (type == slr_parser::lexer::Token::EOF_) break;

        if (lexer.cur_token)
            tokens.push_back(std::move(lexer.cur_token));
    }

    // std::cout << "Lexing done. Tokens count: " << tokens.size() << std::endl;

    slr_parser::syntaxer::Grammar grammar;
    
    slr_parser::syntaxer::GrammarWorker lr_builder(grammar);
    slr_parser::syntaxer::DfaBuilder canonical_collection(lr_builder);
    
    canonical_collection.Build();
    slr_parser::syntaxer::GrammarWorker::FirstFollow first_follow(grammar);
    
    slr_parser::syntaxer::Parser parser(grammar, canonical_collection, first_follow);

    parser.Run(std::move(tokens));

    return 0;
}

catch (std::exception& e) {
    std::cerr << "EXEPTION!!\n what():" << e.what() << std::endl;
}