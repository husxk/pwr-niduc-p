#pragma once

#include <cstdint>
#include <cstddef>

class data_process
{
  static constexpr uint32_t crc_key         =  0xcba785;
  static constexpr uint8_t  crc_size        =  24;
  static constexpr uint8_t  crc_bitshift    =  crc_size - 8;
  static constexpr uint32_t crc_mask_msb    =  1 << (crc_size - 1);
  static constexpr uint32_t crc_mask_result = (1 << crc_size) - 1;
  static constexpr uint32_t crc_init_value  =  0;

  uint32_t crc;

  void crc_calc(uint8_t*, size_t);

  public:
    bool check_crc(uint8_t*, size_t);
    uint32_t generate_crc(uint8_t*, size_t);

};
