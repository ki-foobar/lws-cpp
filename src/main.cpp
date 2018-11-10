#include <iostream>
#include "elona.hpp"
#include "parallel.hpp"
#include "weapon_enchantment_searcher.hpp"

using namespace absolute_gentleman;



int main(int argc, char** argv)
{
    random::TitleGenerator::initialize();

    constexpr auto begin = 0;
    constexpr auto end = 1 * 10000 * 10000;
    constexpr auto lvl = 1;
    auto w = WeaponType::melee;

    if (argc == 2 && argv[1] == std::string{"--ranged"})
    {
        w = WeaponType::ranged;
    }

    auto match = [](const Enchantment& e)
    {
        return e.power == 550;
    };

    init_enclist_table();

    std::cout << "Lvl,Id,Page,Enc,Power,Blood" << std::endl;

    const auto page_begin = begin / 17;
    const auto page_end = end / 17;

    repeat_parallel([=](size_t i)
    {
        thread_local WeaponEnchantmentSearcher searcher;
        const auto page = i + page_begin;
        searcher.search(page, match, lvl, w);
    }, page_end - page_begin);

    return 0;
}
