#include <iomanip>
#include <iostream>
#include <mutex>

#include "elona.hpp"
#include "parallel.hpp"
#include "randomtitlegenerator.hpp"
#include "weapon.hpp"

using namespace absolutegentleman;



// MODIFY HERE.

// [begin, end)
constexpr auto begin = 0;
constexpr auto end = 2 * 10000 * 10000;

/*
 * 20050: fire
 * 20051: cold
 * 20052: lightning
 * 20053: darkness
 * 20054: mind
 * 20055: poison
 * 20056: nether
 * 20057: sound
 * 20058: nerve
 * 20059: chaos
 * 20060: magic
 */

constexpr auto has_ehekatl_feat = true;
constexpr auto hammer_enhancement = 0;



using namespace hsprnd;


gentleman::elona::RandomTitleGenerator title_generator;



std::mutex cout_mutex;
std::mutex title_generator_mutex;



class WeaponTitleSearcher
{
public:
    WeaponTitleSearcher(gentleman::random::Generator& gen)
        : gen(gen)
    {
    }



    void search(int page, int searching_type, int power_threshold, WeaponType weapon_type)
    {
        constexpr auto weapon_level = 1;

        for (int i = 1; i < 17; ++i)
        {
            const auto weapon_seed = 50500 + page * 17 + i;
            const Weapon weapon{weapon_type, weapon_level, weapon_seed};
            const auto matched = _match_enchantment(weapon, searching_type, power_threshold);
            if (matched)
            {
                _dump_added_enchantment_infomation(weapon);
            }
        }
    }



private:
    gentleman::random::Generator gen;



    bool _match_enchantment(const Weapon& weapon, int type, int threshold)
    {
        for (int i = 0; i < 50; ++i)
        {
            const auto seed = weapon.seed + weapon.level * 10 + i;
            gen.randomize(seed);
            const auto e_level = gen.rnd(5);
            const auto e_type = randomenc(gen, e_level, weapon.type);
            const auto e_power = randomencp(gen, has_ehekatl_feat, hammer_enhancement);
            const auto e_type2 = encadd(gen, e_type);
            if (e_type2 != 0)
            {
                if (e_type2 == 34)
                {
                    if (gen.rnd(3))
                    {
                        continue;
                    }
                }
                return e_type2 == type && e_power >= threshold;
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
        }
        gen.randomize(weapon.seed);
        const auto blood = 4 + gen.rnd(12);

        int type{};
        int power{};
        for (int i = 0; i < 50; ++i)
        {
            const auto seed = weapon.seed + weapon.level * 10 + i;
            gen.randomize(seed);
            const auto e_level = gen.rnd(5);
            const auto e_type = randomenc(gen, e_level, weapon.type);
            const auto e_power = randomencp(gen, has_ehekatl_feat, hammer_enhancement);
            const auto e_type2 = encadd(gen, e_type);
            if (e_type2 != 0)
            {
                if (e_type2 == 34)
                {
                    if (gen.rnd(3))
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



int main()
{
    title_generator.initialize();

    init_enclist_table();

    std::cout << "Id,Page,Name,Enc,Power,Blood" << std::endl;

    const auto page_begin = begin / 17;
    const auto page_end = end / 17;

    gentleman::parallel::repeat_internal([=](size_t begin, size_t end)
    {
        gentleman::random::Generator gen;
        WeaponTitleSearcher searcher{gen};
        for (size_t i = begin; i < end; ++i)
        {
            const auto page = i + page_begin;
            searcher.search(page, 34, 400, WeaponType::melee);
        }
    }, page_end - page_begin);

    return 0;
}
