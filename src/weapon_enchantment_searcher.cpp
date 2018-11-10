#include "weapon_enchantment_searcher.hpp"
#include <iomanip>
#include <iostream>
#include <mutex>
#include "elona.hpp"



namespace
{

constexpr auto has_ehekatl_feat = true;
constexpr auto hammer_enhancement = 0;

std::mutex cout_mutex;
std::mutex title_generator_mutex;

}



namespace absolute_gentleman
{

Enchantment WeaponEnchantmentSearcher::_get_enchantment(const Weapon& weapon)
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
            return {e_type2, e_power};
        }
    }

    return {};
}



void WeaponEnchantmentSearcher::_dump(const Weapon& weapon, const Enchantment& enchantment)
{
    std::lock_guard<std::mutex> title_generator_guard{title_generator_mutex};
    std::lock_guard<std::mutex> cout_guard{cout_mutex};

    auto weapon_title = title_generator.generate(weapon.seed - 40000);
    if (!weapon.is_selectable())
    {
        weapon_title += "(選択不可)";
    }

    engine.randomize(weapon.seed);
    const auto blood = 4 + engine.rnd(12);

    std::cout
        << weapon.level << "->" << (weapon.level + 1) << ","
        << weapon.seed << ","
        << ((weapon.seed - 50500) / 17 + 1) << ","
        << weapon_title << ","
        << enchantment.to_string() << ","
        << enchantment.power << ","
        << blood
        << std::endl;
}

}

