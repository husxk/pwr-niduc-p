#include <cstdint>
#include <cmath>
#include "mt_rng.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static uint32_t MT[624];
static int32_t mti = 0;

constexpr double TWO_PI = 2.0 * M_PI;

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

static uint32_t mersenne_twister()
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
    uint64_t range = (uint64_t)(max - min) + 1;
    uint32_t random_value = mersenne_twister();
    uint32_t scaled_random = random_value % range;
    return min + scaled_random;
}

uint32_t get_normal(double mean, double standard_deviation)
{
    //the method generates two values. In order not to drop the second value we store it in z1 variable.

    static double z1;
    static bool generate;
    generate = !generate; //flag to alter between generating new pair or returning the second value from pair

    if (!generate)
        return static_cast<uint32_t>(z1 * standard_deviation + mean);

    double u1, u2, z0;
    do
    {
        u1 = mersenne_twister() * (1.0 / static_cast<double>(UINT32_MAX));
        u2 = mersenne_twister() * (1.0 / static_cast<double>(UINT32_MAX));
        z0 = sqrt(-2.0 * log(u1)) * cos(TWO_PI * u2);
        z1 = sqrt(-2.0 * log(u1)) * sin(TWO_PI * u2);
    } while (std::isnan(z0) || std::isnan(z1));

    return static_cast<uint32_t>(z0 * standard_deviation + mean);
}

