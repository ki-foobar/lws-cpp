#pragma once

#include <cassert>
#include <string>



namespace absolute_gentleman
{

struct Enchantment
{
    int type;
    int power;



    Enchantment(int type, int power)
        : type(type)
        , power(power)
    {
        assert(type != 0);
        assert(power > 0);
    }



    Enchantment()
        : type(0)
        , power(0)
    {
    }



    std::string to_string() const;
};

}
