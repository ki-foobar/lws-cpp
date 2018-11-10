#include "elona.hpp"
#include <array>
#include <iostream>
#include <vector>



namespace
{

struct EncRef
{
    int id;
    int level;
    int rarity;
};

const std::vector<EncRef> encref_base{
    {1,  1, 3000},
    {2,  3, 7500},
    {3,  0, 4500},
    {6,  0, 4500},
    {7,  2, 900},
    {22, 1, 150},
    {23, 1, 400},
    {24, 2, 300},
    {25, 1, 400},
    {26, 1, 600},
    {27, 1, 600},
    {28, 2, 500},
    {32, 1, 250},
    {33, 3, 200},
    {34, 3, 250},
    {48, 1, 300},
    {56, 0, 30},
    {57, 2, 200},
    {58, 2, 200},
    {59, 0, 30},
    {61, 2, 150},
};


std::array<std::array<std::vector<std::pair<int, int>>, 5>, 2> enclist_table;
std::array<std::array<int, 5>, 2> enclist_sum;

}



namespace absolute_gentleman
{

void init_enclist_table()
{
    for (size_t i = 0; i < 2; ++i)
    {
        for (size_t j = 0; j < 5; ++j)
        {
            int sum{};
            for (const auto& ref : encref_base)
            {
                if (ref.id == 34 && i == 1)
                {
                    continue;
                }
                if (ref.level > static_cast<int>(j))
                {
                    continue;
                }
                sum += ref.rarity;
                enclist_table[i][j].push_back(std::make_pair(ref.id, sum));
            }
            enclist_sum[i][j] = sum;
        }
    }
}



int randomenc(random::Engine& engine, int e_level, WeaponType weapon_type)
{
    const auto& enclist = enclist_table[static_cast<size_t>(weapon_type)][e_level];
    const auto sum = enclist_sum[static_cast<size_t>(weapon_type)][e_level];

    const auto p = engine.rndex(sum);
    for (const auto& e : enclist)
    {
        if (p < e.second)
        {
            return e.first;
        }
    }
    return 0;
}



int randomencp(random::Engine& engine, bool has_ehekatl_feat, int hammer_enhancement)
{
    if (hammer_enhancement)
    {
        return 0;
        // return rndex2(rndex2((500 + has_ehekatl_feat * 50) * std::min(1 + hammer_enhancement * hammer_enhancement, 50)) + 1) + 1;
    }
    else
    {
        return engine.rnd(engine.rnd(500 + has_ehekatl_feat * 50) + 1) + 1;
    }
}



int randomele(random::Engine& engine)
{
    const std::array<int, 11> rarity{{
        1,
        1,
        1,
        2,
        2,
        3,
        4,
        3,
        3,
        4,
        5,
    }};

    auto e = engine.rnd(11);
    const auto r = rarity[e];

    if (r != 1)
    {
        static_cast<void>(engine.rnd(1)); // Advance RNG's state by 1 step.
        for (int i = 0; i < r - 1; ++i)
        {
            const auto e2 = engine.rnd(11);
            const auto r2 = rarity[e2];
            if (r2 < r)
            {
                if (engine.rnd(2) == 0)
                {
                    e = e2;
                }
            }
        }
    }

    return e + 50;
}



int encadd(random::Engine& engine, int e_type)
{
    if (e_type >= 20 || e_type == 0)
    {
        return e_type;
    }

    switch (e_type)
    {
    case 1: return 10000 + engine.rnd(10) + 10;
    case 2: return 20000 + randomele(engine);
    case 3: return 30000 + engine.rnd(40) + 150;
    case 6: return 60000 + engine.rnd(10) + 10;
    case 7: return 70000 + randomele(engine);
    default: return 0;
    }
}

}
