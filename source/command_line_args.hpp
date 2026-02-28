#include <iostream>
#include <fstream>
#include <string>
#include "cxxopts.hpp"

namespace slr_parser {

struct AppSettings
{
    std::unique_ptr<std::ifstream> file_input_storage;
    std::unique_ptr<std::ofstream> file_output_storage;

    std::istream* istream = &std::cin;
    std::ostream* ostream = &std::cout;
};

inline bool ParseCommandLineArgs(AppSettings& settings, const int argc, const char *argv[])
{
    cxxopts::Options options("SLR_Parser", "Парсер грамматик и лексер");

    options.add_options()
        ("i,input_file" , "Путь к входному файлу" , cxxopts::value<std::string>())
        ("o,output_file", "Путь к выходному файлу", cxxopts::value<std::string>())
        ("h,help"       , "ПОМОГИ!")
        ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return false;
    }


    if (result.count("input_file"))
    {
        std::string ifilename = result["input_file"].as<std::string>();
        
        settings.file_input_storage = std::make_unique<std::ifstream>(ifilename);

        if (!settings.file_input_storage->is_open())
        {
            throw std::runtime_error("Could not open file: " + ifilename);
        }
        
        settings.istream = settings.file_input_storage.get();
    }
    
    else
    {
        settings.istream = &std::cin;
    }


    if (result.count("output_file"))
    {
        std::string ofilename = result["output_file"].as<std::string>();
        
        settings.file_output_storage = std::make_unique<std::ofstream>(ofilename);

        if (!settings.file_output_storage->is_open())
        {
            throw std::runtime_error("Could not open file: " + ofilename);
        }
        
        settings.ostream = settings.file_output_storage.get();
    }
    
    else
    {
        settings.ostream = &std::cout;
    }

    return true;
}

}
