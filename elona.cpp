#include "elona.hpp"
#include <array>
#include <iostream>
#include <vector>
#include "random.hpp"


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





std::string skillname(int id)
{
    switch (id)
    {
    case 10: return u8"筋力";
    case 11: return u8"耐久";
    case 12: return u8"器用";
    case 13: return u8"感覚";
    case 14: return u8"意思";
    case 15: return u8"習得";
    case 16: return u8"魔力";
    case 17: return u8"魅力";
    case 18: return u8"速度";
    case 19: return u8"運勢";
    case 50: return u8"火炎";
    case 51: return u8"冷気";
    case 52: return u8"電撃";
    case 53: return u8"暗黒";
    case 54: return u8"幻惑";
    case 55: return u8"毒";
    case 56: return u8"地獄";
    case 57: return u8"音";
    case 58: return u8"神経";
    case 59: return u8"混沌";
    case 60: return u8"魔法";
    case 150: return u8"読書";
    case 151: return u8"遺伝子学";
    case 152: return u8"戦術";
    case 153: return u8"重量挙げ";
    case 154: return u8"治癒";
    case 155: return u8"瞑想";
    case 156: return u8"交渉";
    case 157: return u8"隠密";
    case 158: return u8"鍵開け";
    case 159: return u8"探知";
    case 160: return u8"投資";
    case 161: return u8"解剖学";
    case 162: return u8"自然鑑定";
    case 163: return u8"採掘";
    case 164: return u8"魔力の限界";
    case 165: return u8"暗記";
    case 166: return u8"二刀流";
    case 167: return u8"両手持ち";
    case 168: return u8"盾";
    case 169: return u8"重装備";
    case 170: return u8"中装備";
    case 171: return u8"軽装備";
    case 172: return u8"詠唱";
    case 173: return u8"回避";
    case 174: return u8"魔道具";
    case 175: return u8"罠解体";
    case 176: return u8"大工";
    case 177: return u8"裁縫";
    case 178: return u8"錬金術";
    case 179: return u8"宝石細工";
    case 180: return u8"栽培";
    case 181: return u8"信仰";
    case 182: return u8"旅歩き";
    case 183: return u8"演奏";
    case 184: return u8"料理";
    case 185: return u8"釣り";
    case 186: return u8"心眼";
    case 187: return u8"見切り";
    case 188: return u8"魔力制御";
    case 189: return u8"射撃";
    default: return "";
    }
}


}




namespace hsprnd
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




int randomenc(gentleman::random::Generator& gen, int e_level, WeaponType weapon_type)
{
    const auto& enclist = enclist_table[static_cast<size_t>(weapon_type)][e_level];
    const auto sum = enclist_sum[static_cast<size_t>(weapon_type)][e_level];

    const auto p = gen.rndex(sum);
    for (const auto& e : enclist)
    {
        if (p < e.second)
        {
            return e.first;
        }
    }
    return 0;
}



int randomencp(gentleman::random::Generator& gen, bool has_ehekatl_feat, int hammer_enhancement)
{
    if (hammer_enhancement)
    {
        return 0;
        // return rndex2(rndex2((500 + has_ehekatl_feat * 50) * std::min(1 + hammer_enhancement * hammer_enhancement, 50)) + 1) + 1;
    }
    else
    {
        return gen.rnd(gen.rnd(500 + has_ehekatl_feat * 50) + 1) + 1;
    }
}



std::string get_e_desc(int e_type, int e_power)
{
    switch (e_type / 10000)
    {
    case 1:
        return skillname(e_type % 10000) + u8"を" + std::to_string(e_power / 50 + 1) + "上げる";
    case 2:
        return skillname(e_type % 10000) + u8"への耐性を授ける";
    case 3:
        return skillname(e_type % 10000) + u8"の技能を" + std::to_string(e_power / 50 + 1) + "上げる";
    case 6:
        return skillname(e_type % 10000) + u8"を維持する";
    case 7:
        return skillname(e_type % 10000) + u8"属性の追加ダメージを与える";
    default:
        break;
    }
    switch (e_type)
    {
    case 21: return u8"ランダムなテレポートを引き起こす";
    case 22: return u8"テレポートを妨害する";
    case 23: return u8"盲目を無効にする";
    case 24: return u8"麻痺を無効にする";
    case 25: return u8"混乱を無効にする";
    case 26: return u8"恐怖を無効にする";
    case 27: return u8"睡眠を無効にする";
    case 28: return u8"毒を無効にする";
    case 29: return u8"速度を上げ、ワールドマップでの移動時間を短くする";
    case 30: return u8"エーテルの風からあなたを保護する";
    case 31: return u8"雷雨と雪による足止めを無効にする";
    case 32: return u8"あなたを浮遊させる";
    case 33: return u8"あなたを変異から保護する";
    case 34: return u8"魔法の威力を高める";
    case 35: return u8"透明な存在を見ることを可能にする";
    case 36: return u8"攻撃対象からスタミナを吸収する";
    case 37: return u8"全てを終結させる";
    case 38: return u8"攻撃対象からマナを吸収する";
    case 39: return u8"完全貫通攻撃発動の機会を増やす";
    case 40: return u8"稀に時を止める";
    case 41: return u8"アイテムを盗まれなくする";
    case 42: return u8"腐ったものを難なく消化させる";
    case 43: return u8"呪いの言葉から保護する";
    case 44: return u8"クリティカルヒットの機会を増やす";
    case 45: return u8"使用者の生き血を吸う";
    case 46: return u8"あなたの成長を妨げる";
    case 47: return u8"魔物を呼び寄せる";
    case 48: return u8"異物の体内への侵入を防ぐ";
    case 49: return u8"演奏報酬の品質を上げる";
    case 50: return u8"追加打撃の機会を増やす";
    case 51: return u8"追加射撃の機会を増やす";
    case 52: return u8"被る物理ダメージを軽減する";
    case 53: return u8"被るダメージを稀に無効にする";
    case 54: return u8"攻撃された時、相手に切り傷のダメージを与える";
    case 55: return u8"出血を抑える";
    case 56: return u8"神が発する電波をキャッチする";
    case 57: return u8"竜族に対して強力な威力を発揮する";
    case 58: return u8"不死者に対して強力な威力を発揮する";
    case 59: return u8"他者の信仰を明らかにする";
    case 60: return u8"深い音色で聴衆を酔わす";
    case 61: return u8"神に対して強力な威力を発揮する";
    default: return "Unknown: " + std::to_string(e_type);
    }
}



int randomele(gentleman::random::Generator& gen)
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

    auto e = gen.rnd(11);
    const auto r = rarity[e];

    if (r != 1)
    {
        static_cast<void>(gen.rnd(1)); // Advance RNG's state by 1 step.
        for (int i = 0; i < r - 1; ++i)
        {
            const auto e2 = gen.rnd(11);
            const auto r2 = rarity[e2];
            if (r2 < r)
            {
                if (gen.rnd(2) == 0)
                {
                    e = e2;
                }
            }
        }
    }

    return e + 50;
}



int encadd(gentleman::random::Generator& gen, int e_type)
{
    if (e_type >= 20 || e_type == 0)
    {
        return e_type;
    }

    switch (e_type)
    {
    case 1: return 10000 + gen.rnd(10) + 10;
    case 2: return 20000 + randomele(gen);
    case 3: return 30000 + gen.rnd(40) + 150;
    case 6: return 60000 + gen.rnd(10) + 10;
    case 7: return 70000 + randomele(gen);
    default: return 0;
    }
}


}
