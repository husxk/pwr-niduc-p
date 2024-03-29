#include <stdint.h>
#include "mt_rng.hpp"

static uint32_t MT[624];
static int32_t mti = 0;

void init_mt(uint32_t x0)
{
    uint64_t x;

    MT[0] = x0;
    for (int32_t i = 1; i < 623; i++)
    {
        x = MT[i - 1];
        x = (23023 * x) & 0xffffffffull;
        x = (3 * x) & 0xffffffffull;
        MT[i] = x;
    }
}

uint32_t mersenne_twister()
{
    const uint32_t MA[] = {0, 0x9908b0df};
    uint32_t y;
    int32_t i1, i397;

    i1 = mti + 1;
    if (i1 > 623)
        i1 = 0;
    i397 = mti + 397;
    if (i397 > 623)
        i397 -= 624;
    y = (MT[mti] & 0x80000000) | (MT[i1] & 0x7fffffff);
    MT[mti] = MT[i397] ^ (y >> 1) ^ MA[y & 1];
    y = MT[mti];
    y ^= y >> 11;
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= y >> 18;
    mti = i1;
    return y;
}

uint32_t get_next(uint32_t min, uint32_t max)
{
    if (min >= max)
    {
        return min;
    }
    uint32_t range = (uint32_t)(max - min + 1);
    uint32_t random_value = mersenne_twister();
    uint32_t scaled_random = random_value % range;
    return min + scaled_random;
}
