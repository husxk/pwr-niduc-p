#ifndef MT_RNG_H
#define MT_RNG_H

#include <stdint.h>

void init_mt(uint32_t x0);
uint32_t mersenne_twister();
uint32_t get_next(uint32_t min, uint32_t max);

#endif // MT_RNG_H
