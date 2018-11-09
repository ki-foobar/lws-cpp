#pragma once

#include <string>
#include <vector>



namespace gentleman
{
namespace elona
{

class RandomTitleGenerator
{
public:
    void initialize();

    std::string generate(int seed) const;

private:
    std::vector<std::vector<std::string>> word_table_cp932;
    std::vector<std::vector<std::string>> word_table_utf8;
};

}
}
