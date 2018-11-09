#pragma once

#include <string>
#include <vector>
#include "weapon_type.hpp"
#include "random/engine.hpp"



namespace hsprnd
{

int randomenc(absolute_gentleman::random::Engine& engine, int e_level, WeaponType weapon_type);
int randomencp(absolute_gentleman::random::Engine& engine, bool has_ehekatl_feat, int hammer_enhancement);

std::string get_e_desc(int e_type, int e_power);

int encadd(absolute_gentleman::random::Engine& engine, int e_type);

void init_enclist_table();

}
