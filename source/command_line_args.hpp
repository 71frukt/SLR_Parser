#include <iostream>
#include "cxxopts.hpp"

namespace slr_parser {

struct AppSettings
{
    std::string filepath;
};

inline bool ParseCommandLineArgs(AppSettings& settings, const int argc, const char *argv[])
{
    cxxopts::Options options("SLR_Parser", "Парсер грамматик и лексер");

    options.add_options()
        ("f,source_file", "Путь к входному файлу", cxxopts::value<std::string>())
        ("h,help"       , "ПОМОГИ!")
        ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return false;
    }

    if (result.count("source_file"))
    {
        settings.filepath = result["source_file"].as<std::string>();
    }

    return true;
}

}
