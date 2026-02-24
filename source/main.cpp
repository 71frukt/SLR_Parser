#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include "lexer/token.hpp"
#include "lexer/crazy_lexer.hpp"
#include "command_line_args.hpp"

int main(const int argc, const char *argv[]) try {

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
    
    slr_parser::lexer::CrazyLexer lexer(input_stream);
    
    int token_type;

    while ((token_type = lexer.yylex()) != slr_parser::lexer::Token::EOF_)
    {
        std::cout << "Got token ID: " << token_type << std::endl;

        auto& token = *lexer.cur_token; // ссылка на объект внутри unique_ptr

        std::cout << "Token ID: " << token_type 
                  << " [L:" << token.code_place.line 
                  << ", C:" << token.code_place.column << "]";

        if (token_type == slr_parser::lexer::Token::ID)
        {
            std::cout << "attr: " << std::get<slr_parser::lexer::Token::VarT>(token.attr) << std::endl;
        }

        else if (token_type == slr_parser::lexer::Token::NUM)
        {
            std::cout << "attr: " << std::get<slr_parser::lexer::Token::NumT>(token.attr) << std::endl;
        }
    }
    return 0;
}

catch (std::exception& e) {
    std::cerr << "EXEPTION!!\n what():" << e.what() << std::endl;
}