#include <iomanip>
#include <iostream>
#include <mutex>

#include "elona.hpp"
#include "parallel.hpp"
#include "random/title_generator.hpp"
#include "weapon.hpp"

using namespace absolute_gentleman;



constexpr auto has_ehekatl_feat = true;
constexpr auto hammer_enhancement = 0;




random::TitleGenerator title_generator;



std::mutex cout_mutex;
std::mutex title_generator_mutex;



class WeaponEnchantmentSearcher
{
public:
    WeaponEnchantmentSearcher() = default;



    template <typename F>
    void search(int page, F match, WeaponType weapon_type)
    {
        constexpr auto weapon_level = 1;

        for (int i = 0; i < 17; ++i)
        {
            const auto weapon_seed = 50500 + page * 17 + i;
            const Weapon weapon{weapon_type, weapon_level, weapon_seed};
            const auto matched = _match_enchantment(weapon, match);
            if (matched)
            {
                _dump_added_enchantment_infomation(weapon);
            }
        }
    }



private:
    random::Engine engine;



    template <typename F>
    bool _match_enchantment(const Weapon& weapon, F match)
    {
        for (int i = 0; i < 50; ++i)
        {
            const auto seed = weapon.seed + weapon.level * 10 + i;
            engine.randomize(seed);
            const auto e_level = engine.rnd(5);
            const auto e_type = randomenc(engine, e_level, weapon.type);
            const auto e_power = randomencp(engine, has_ehekatl_feat, hammer_enhancement);
            const auto e_type2 = encadd(engine, e_type);
            if (e_type2 != 0)
            {
                if (e_type2 == 34)
                {
                    if (engine.rnd(3))
                    {
                        continue;
                    }
                }
                return match(e_type2, e_power);
            }
        }

        return false;
    }



    void _dump_added_enchantment_infomation(const Weapon& weapon)
    {
        std::string weapon_title;
        {
            std::lock_guard<std::mutex> guard{title_generator_mutex};
            weapon_title = title_generator.generate(weapon.seed - 40000);
            if (!weapon.is_selectable())
            {
                weapon_title += "(選択不可)";
            }
        }

        engine.randomize(weapon.seed);
        const auto blood = 4 + engine.rnd(12);

        int type{};
        int power{};
        for (int i = 0; i < 50; ++i)
        {
            const auto seed = weapon.seed + weapon.level * 10 + i;
            engine.randomize(seed);
            const auto e_level = engine.rnd(5);
            const auto e_type = randomenc(engine, e_level, weapon.type);
            const auto e_power = randomencp(engine, has_ehekatl_feat, hammer_enhancement);
            const auto e_type2 = encadd(engine, e_type);
            if (e_type2 != 0)
            {
                if (e_type2 == 34)
                {
                    if (engine.rnd(3))
                    {
                        continue;
                    }
                }
                type = e_type2;
                power = e_power;
                break;
            }
        }

        std::lock_guard<std::mutex> guard{cout_mutex};
        std::cout
            << weapon.level << "->" << (weapon.level + 1) << ","
            << weapon.seed << ","
            << ((weapon.seed - 50500) / 17 + 1) << ","
            << weapon_title << ","
            << get_e_desc(type, power) << ","
            << power << ","
            << blood
            << std::endl;
    }
};



int main(int argc, char** argv)
{
    constexpr auto begin = 0;
    constexpr auto end = 1 * 10000 * 10000;
    auto w = WeaponType::melee;

    if (argc == 2 && argv[1] == std::string{"--ranged"})
    {
        w = WeaponType::ranged;
    }

    auto match = [](int, int power)
    {
        return power == 550;
    };


    title_generator.initialize();

    init_enclist_table();

    std::cout << "Id,Page,Name,Enc,Power,Blood" << std::endl;

    const auto page_begin = begin / 17;
    const auto page_end = end / 17;

    repeat_parallel([=](size_t i)
    {
        thread_local WeaponEnchantmentSearcher searcher;
        const auto page = i + page_begin;
        searcher.search(page, match, w);
    }, page_end - page_begin);

    return 0;
}
