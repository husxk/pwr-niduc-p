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

  static constexpr uint32_t whitening_init  =  0x5A;

  uint32_t crc;

  class whitening_register
  {
    uint8_t as_unsigned;

    public:

    uint8_t get_bit(uint8_t x)
    {
      return as_unsigned & (1 << x);
    }

    void cycle()
    {
      as_unsigned <<= 1;

      const uint8_t msb_bit = as_unsigned >> 7;

      as_unsigned ^= msb_bit << 4;
      as_unsigned |= msb_bit;
    }

    whitening_register()
    {
      this->as_unsigned = whitening_init;
    }
  };

  void crc_calc(uint8_t*, size_t);

  public:
    bool check_crc(uint8_t*, size_t);
    uint32_t generate_crc(uint8_t*, size_t);

    void whitening(uint8_t*, size_t);

};
