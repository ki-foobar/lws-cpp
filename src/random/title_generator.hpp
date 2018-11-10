#pragma once

#include <string>
#include <vector>
#include "engine.hpp"



namespace absolute_gentleman
{
namespace random
{

class TitleGenerator
{
public:
    static void initialize();

    std::string generate(int seed);

private:
    static std::vector<std::vector<std::string>> word_table_cp932;
    static std::vector<std::vector<std::string>> word_table_utf8;

    Engine engine;
};

}
}
