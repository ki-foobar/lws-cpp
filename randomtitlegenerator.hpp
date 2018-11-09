#pragma once

#include <string>
#include <vector>
#include "random.hpp"



namespace gentleman
{
namespace elona
{

class RandomTitleGenerator
{
public:
    void initialize();

    std::string generate(int seed);

private:
    std::vector<std::vector<std::string>> word_table_cp932;
    std::vector<std::vector<std::string>> word_table_utf8;
    gentleman::random::Generator gen;
};

}
}
