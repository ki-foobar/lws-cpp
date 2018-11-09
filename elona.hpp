#pragma once

#include <string>
#include <vector>
#include "weapon_type.hpp"
#include "random.hpp"



namespace hsprnd
{

int randomenc(gentleman::random::Generator& gen, int e_level, WeaponType weapon_type);
int randomencp(gentleman::random::Generator& gen, bool has_ehekatl_feat, int hammer_enhancement);

std::string get_e_desc(int e_type, int e_power);

int encadd(gentleman::random::Generator& gen, int e_type);

void init_enclist_table();

}
