#ifndef MT_RNG_H
#define MT_RNG_H

#include <cstdint>

void init_mt(uint32_t x0);
uint32_t get_next(uint32_t min, uint32_t max);
uint32_t get_normal(double mean, double standard_deviation);

#endif // MT_RNG_H
