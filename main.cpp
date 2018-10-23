#include <iomanip>
#include <iostream>
#include <mutex>

#include "elona.hpp"
#include "parallel.hpp"
#include "randomtitlegenerator.hpp"




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
constexpr auto weapon_type = WeaponType::ranged;





using namespace hsprnd;


gentleman::elona::RandomTitleGenerator title_generator;




std::mutex cout_mutex;

void process_one_title(gentleman::random::Generator& gen, int weapon_seed, int level)
{
    const auto weapon_title = title_generator.generate(weapon_seed - 40000);
    gen.randomize(weapon_seed);
    const auto blood = 4 + gen.rnd(12);

    int type{};
    int power{};
    for (int i = 0; i < 50; ++i)
    {
        const auto seed = weapon_seed + level * 10 + i;
        gen.randomize(seed);
        const auto e_level = gen.rnd(5);
        const auto e_type = randomenc(gen, e_level, weapon_type);
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
        << level << "->" << (level + 1) << ","
        << weapon_seed << ","
        << ((weapon_seed - 50500) / 17 + 1) << ","
        << weapon_title << ","
        << get_e_desc(type, power) << ","
        << power << ","
        << blood
        << std::endl;
}



bool match_enchantment(gentleman::random::Generator& gen, int weapon_seed, int type, int threshold, int level)
{
    for (int i = 0; i < 50; ++i)
    {
        const auto seed = weapon_seed + level * 10 + i;
        gen.randomize(seed);
        const auto e_level = gen.rnd(5);
        const auto e_type = randomenc(gen, e_level, weapon_type);
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
            return e_type2/10000 == 2 && e_power >= threshold;
        }
    }

    return false;
}



void search(gentleman::random::Generator& gen, int page, int searching_type, int power_threshold)
{
    for (int i = 1; i < 17; ++i)
    {
        const auto weapon_seed = 50500 + page * 17 + i;
        const auto match = match_enchantment(gen, weapon_seed, searching_type, power_threshold, 1);
        if (match)
        {
            process_one_title(gen, weapon_seed, 1);
            for (int i = 2; i <= 14; ++i)
            {
                process_one_title(gen, weapon_seed - (i - 1) * 10, i);
            }
        }
    }
}



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
        for (size_t i = begin; i < end; ++i)
        {
            const auto page = i + page_begin;
            search(gen, page, 0, 550);
        }
    }, page_end - page_begin);

    return 0;
}
