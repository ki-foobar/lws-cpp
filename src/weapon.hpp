#pragma once

#include <cassert>
#include "weapon_type.hpp"



namespace absolute_gentleman
{

struct Weapon
{
    WeaponType type;
    int level;
    int seed;



    Weapon(WeaponType type, int level, int seed)
        : type(type)
        , level(level)
        , seed(seed)
    {
        assert(1 <= level);
        assert(50500 <= seed);
    }



    bool is_selectable() const
    {
        return (seed - 50500) % 17 != 0;
    }
};

}

