#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "tea.hpp"
#include "ble.hpp"

#define BIT_COUNT 8

ble::packet::packet(uint32_t data_size, uint8_t *data)
{
  this->data_size = data_size;
  this->data = (uint8_t *)std::malloc(data_size);
  std::memcpy(this->data, data, data_size);
}

ble::packet::packet(const ble::packet *copy)
{
  this->data_size = copy->data_size;
  this->data = (uint8_t *)std::malloc(copy->data_size);
  std::memcpy(this->data, copy->data, copy->data_size);
}

ble::packet::~packet()
{
  std::free(this->data);
}

  void
ble::packet::print_packet()
{
  for(int i=0; i<this->data_size; i++)
  {
    printf("%02X", data[i]);
    if(i % 0x10 == 0x0F)
      printf("\n");
    else
      printf(":");
  }
  printf("\n");
}

  uint32_t
ble::packet::check_packet(ble::packet pack)
{
  uint32_t err_count = 0;

  for(int i=0; i<this->data_size; i++){
    for(int j=0; j<BIT_COUNT; j++){
      if(((this->data[i] & 1U << j) ^ (pack.data[i] & 1U << j))){
        err_count++;
      }
    }
  }

  return err_count;
}

  int
ble::packet::distort_data(uint8_t percent)
{
  if(!percent)
    return 0;

  if(percent > 100)
    return -1;

  uint32_t dummy = 0;

  for(uint32_t i = 0; i < this->data_size*8; i++){
    dummy += percent;

    if( dummy >= 100 ){
      this->data[i/8] ^= 1u << i%8;
      dummy %= 100;
    }
  }

  return 0;
}
