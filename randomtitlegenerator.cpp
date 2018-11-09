#include "randomtitlegenerator.hpp"
#include <fstream>
#include "random.hpp"



namespace
{

std::vector<std::string> _split_record(const std::string& line)
{
    std::vector<std::string> fields{15};

    size_t pos{};
    for (size_t i = 0; i < fields.size(); ++i)
    {
        const auto comma_pos = line.find(',', pos);
        if (comma_pos == std::string::npos)
        {
            fields[i] = line.substr(pos);
            break;
        }
        fields[i] = line.substr(pos, comma_pos - pos);
        pos = comma_pos + 1;
    }

    return fields;
}



std::vector<std::vector<std::string>> _load_word_table(const std::string& filename)
{
    std::vector<std::vector<std::string>> word_table;

    std::ifstream in{filename, std::ios::binary};
    if (!in)
    {
        throw std::runtime_error{"Failed to load ndata.csv."};
    }
    std::string buf;
    while (std::getline(in, buf))
    {
        word_table.push_back(_split_record(buf));
    }

    return word_table;
}

}



namespace gentleman
{
namespace elona
{

void RandomTitleGenerator::initialize()
{
    word_table_cp932 = _load_word_table("data/ndata-cp932.csv");
    word_table_utf8 = _load_word_table("data/ndata-utf8.csv");
}



std::string RandomTitleGenerator::generate(int seed) const
{
    gentleman::random::Generator gen{seed};

    // "具"
    const auto category_concrete = std::string{
        static_cast<char>(0x8B),
        static_cast<char>(0xEF),
    };

    // "万能"
    const auto category_general = std::string{
        static_cast<char>(0x96),
        static_cast<char>(0x9C),
        static_cast<char>(0x94),
        static_cast<char>(0x5C),
    };

    // "の"
    const auto no = std::string{
        static_cast<char>(0x82),
        static_cast<char>(0xCC),
    };

    // "・オブ・"
    const auto of = std::string{
        static_cast<char>(0x81),
        static_cast<char>(0x45),
        static_cast<char>(0x83),
        static_cast<char>(0x49),
        static_cast<char>(0x83),
        static_cast<char>(0x75),
        static_cast<char>(0x81),
        static_cast<char>(0x45),
    };

    // "・"
    const auto sep = std::string{
        static_cast<char>(0x81),
        static_cast<char>(0x45),
    };

    int p_1;
    int p_2;
    int p_4;
    std::string randn2_;
    std::string randn2_u;
    std::string randn2_1;

retry:
    while (true)
    {
        p_2 = gen.rnd(static_cast<int>(word_table_cp932.size()));
        p_1 = gen.rnd(14);
        if (!word_table_cp932[p_2][p_1].empty())
        {
            break;
        }
    }
    if (word_table_cp932[p_2][14] == category_concrete)
    {
        goto retry;
    }
    randn2_ = word_table_cp932[p_2][p_1];
    randn2_u = word_table_utf8[p_2][p_1];
    randn2_1 = word_table_cp932[p_2][14];
    do
    {
        if (p_1 == 10 || p_1 == 11)
        {
            if (gen.rnd(5) == 0)
            {
                p_1 = 0;
                if (gen.rnd(2) == 0)
                {
                    randn2_ += no;
                    randn2_u += "の";
                }
                break;
            }
            switch (gen.rnd(5))
            {
            case 0:
                randn2_ += of;
                randn2_u += "・オブ・";
                break;
            case 1:
                return "ザ・" + randn2_u;
            case 2:
                randn2_ += sep;
                randn2_u += "・";
                break;
            default:
                break;
            }
        }
        if (p_1 == 0 || p_1 == 1)
        {
            randn2_ += no;
            randn2_u += "の";
            if (gen.rnd(10) == 0)
            {
                p_1 = 10;
            }
        }
    } while (false);

    bool ok{};
    for (int i = 0; i < 100; ++i)
    {
        p_4 = gen.rnd(static_cast<int>(word_table_cp932.size()));
        if (p_4 == p_2)
        {
            continue;
        }
        if (word_table_cp932[p_4][14] == randn2_1)
        {
            if (word_table_cp932[p_4][14] != category_general && randn2_1 != category_general)
            {
                continue;
            }
        }
        if (p_1 < 10)
        {
            p_1 = gen.rnd(2);
        }
        else
        {
            p_1 = 10 + gen.rnd(2);
        }
        if (word_table_cp932[p_4][p_1].empty())
        {
            continue;
        }
        ok = true;
        break;
    }
    if (!ok)
    {
        goto retry;
    }

    randn2_ += word_table_cp932[p_4][p_1];
    randn2_u += word_table_utf8[p_4][p_1];
    if (randn2_.size() >= 28)
    {
        goto retry;
    }

    return randn2_u;
}

}
}
