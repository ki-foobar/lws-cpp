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
    void initialize();

    std::string generate(int seed);

private:
    std::vector<std::vector<std::string>> word_table_cp932;
    std::vector<std::vector<std::string>> word_table_utf8;
    Engine engine;
};

}
}
