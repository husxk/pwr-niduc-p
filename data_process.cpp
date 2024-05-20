#include "data_process.hpp"

void
data_process::crc_calc(uint8_t* data, size_t size)
{
  crc = crc_init_value;

  for(size_t i = 0; i < size; i++)
  {
    crc ^= (*data) << crc_bitshift;
    data++;

    for (int j = 0; j < 8; j++)
    {
      if (crc & crc_mask_msb)
      {
         crc <<= 1;
        crc ^= crc_key;
      }
      else
         crc <<= 1;
    }
  }

  crc &= crc_mask_result;
}

uint32_t
data_process::generate_crc(uint8_t* data, size_t size)
{
  crc_calc(data, size);
  return crc;
}

bool
data_process::check_crc(uint8_t* data, size_t size)
{
  crc_calc(data, size);
  return crc == 0;
}
