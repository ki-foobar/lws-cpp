#pragma once

#include <cassert>
#include "weapon_type.hpp"



namespace absolutegentleman
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
        assert(50501 <= seed);
        assert((seed - 50500) % 17 != 0);
    }
};

}
