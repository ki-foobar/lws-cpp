#pragma once

#include <string>
#include <vector>
#include "weapon_type.hpp"
#include "random/engine.hpp"



namespace absolute_gentleman
{

int randomenc(random::Engine& engine, int e_level, WeaponType weapon_type);
int randomencp(random::Engine& engine, bool has_ehekatl_feat, int hammer_enhancement);

int encadd(random::Engine& engine, int e_type);

void init_enclist_table();

}
