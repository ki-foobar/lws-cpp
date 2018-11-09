#pragma once

#include <cstdint>


/*
This class is based on a HSP's DLL, EXRand.


0〜32kじゃ何もできん!! 拡張乱数 EXRand
(c)2002 D.N.A. Softwares
このDLLはフリーソフトウェアです。


乱数生成部は松本眞氏と西村拓士氏による
Mersenne Twisterを使用しています。a


Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. The names of its contributors may not be used to endorse or promote
products derived from this software without specific prior written
permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



namespace gentleman
{
namespace random
{

class Generator
{
public:
    Generator(int32_t seed = 0)
        : linear_seed(static_cast<int64_t>(seed))
    {
        init_genrand(static_cast<uint64_t>(seed));
    }


    int32_t rnd(int32_t max)
    {
        linear_seed = linear_seed * 214013 + 2531011;
        return static_cast<int32_t>((linear_seed >> 16) & 32767) % max;
    }


    int32_t rndex(int32_t max)
    {
        return static_cast<int32_t>(genrand_real2() * max);
    }


    void randomize(int32_t new_seed)
    {
        linear_seed = new_seed;
        init_genrand(new_seed);
    }



    Generator(const Generator&) = default;
    Generator(Generator&&) = default;
    Generator& operator=(const Generator&) = default;
    Generator& operator=(Generator&&) = default;



private:
    /* Period parameters */
    static constexpr int N = 624;
    static constexpr int M = 397;
    static constexpr unsigned long MATRIX_A = 0x9908b0dfUL;   /* constant vector a */
    static constexpr unsigned long UMASK = 0x80000000UL; /* most significant w-r bits */
    static constexpr unsigned long LMASK = 0x7fffffffUL; /* least significant r bits */


    int64_t linear_seed;

    unsigned long state[N]; /* the array for the state vector  */
    int left = 1;
    int initf = 0;
    unsigned long *next;


    void init_genrand(unsigned long s);
    void next_state();
    double genrand_real2();
};

}
}
