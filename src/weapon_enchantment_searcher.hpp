#pragma once

#include "enchantment.hpp"
#include "random/title_generator.hpp"
#include "weapon.hpp"



namespace absolute_gentleman
{

class WeaponEnchantmentSearcher
{
public:
    WeaponEnchantmentSearcher() = default;



    template <typename F>
    void search(int page, F match, int weapon_level, WeaponType weapon_type)
    {
        for (int i = 0; i < 17; ++i)
        {
            const auto weapon_seed = 50500 + page * 17 + i;
            const Weapon weapon{weapon_type, weapon_level, weapon_seed};
            const auto enchantment = _get_enchantment(weapon);
            if (match(enchantment))
            {
                _dump(weapon, enchantment);
            }
        }
    }



private:
    random::Engine engine;
    random::TitleGenerator title_generator;



    Enchantment _get_enchantment(const Weapon& weapon);
    void _dump(const Weapon& weapon, const Enchantment& enchantment);
};

}

