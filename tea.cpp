#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include <cstring>

#include "mt_rng.hpp"
#include "tea.hpp"

tea_ctx_t::tea_ctx_t(uint8_t **data, uint32_t *u8_datasize)
{
  this->key[0] = get_next(0,-1);
  this->key[1] = get_next(0,-1);
  this->key[2] = get_next(0,-1);
  this->key[3] = get_next(0,-1);

  const uint32_t u32_datasize =
    (*u8_datasize) % TEA_DATABLOCK_SIZE == 0 ?
    ((*u8_datasize)/TEA_DATABLOCK_SIZE) * 2 :
    ((*u8_datasize)/TEA_DATABLOCK_SIZE + 1)*2;

  (*data) = (uint8_t *)reallocarray(*data, u32_datasize*8, sizeof(uint32_t));

  this->data = (uint32_t *) (*data);
  this->data_size = u32_datasize;
  *u8_datasize = u32_datasize*4;
}

tea_ctx_t::~tea_ctx_t()
{
}

void
tea_ctx_t::print_data(void)
{
  for(int i=0; i<this->data_size; i++){
    printf("%08X", this->data[i]);

    if(i % 8 == 7)
      printf("\n");
    else
      printf(":");
  }

  printf("\n\n");
}

void
tea_ctx_t::encrypt_data(void)
{
  for(int i=0; i<this->data_size + 1; i++){
    encrypt_8B(2*i);
  }
}

void
tea_ctx_t::decrypt_data(void)
{
  for(int i=0; i<this->data_size + 1; i++){
    decrypt_8B(2*i);
  }
}

inline void
tea_ctx_t::encrypt_8B(int index)
{
  uint32_t val_0 = data[index], val_1 = data[index+1];
  uint32_t sum = 0;

  for(int i=0; i<TEA_ITER_COUNT; i++){
    sum += TEA_DELTA;
    val_0 += ((val_1<<4) + key[0]) ^ (val_1 + sum) ^ ((val_1>>5) + key[3]);
    val_1 += ((val_0<<4) + key[2]) ^ (val_0 + sum) ^ ((val_0>>5) + key[1]);
  }

  data[index] = val_0;
  data[index+1] = val_1;
}

inline void
tea_ctx_t::decrypt_8B(int index)
{
  uint32_t val_0 = data[index], val_1 = data[index+1];
  uint32_t sum = TEA_SUM_DECRYPT;

  for(int i=0; i<TEA_ITER_COUNT; i++){
    val_1 -= ((val_0<<4) + key[2]) ^ (val_0 + sum) ^ ((val_0>>5) + key[1]);
    val_0 -= ((val_1<<4) + key[0]) ^ (val_1 + sum) ^ ((val_1>>5) + key[3]);
    sum -= TEA_DELTA;
  }

  data[index] = val_0;
  data[index+1] = val_1;
}
