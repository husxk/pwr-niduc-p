#pragma once

#include <cstdint>

namespace ble
{
  class packet
  {
    uint8_t  *data;
    uint32_t data_size;

    public:
      packet(uint32_t data_size, uint8_t *data);
      packet(const ble::packet *pack);
      ~packet();

      void print_packet();

      uint32_t check_packet(ble::packet pack);

      int distort_data(uint8_t percent);

      uint32_t get_size()  const
      {
        return this->data_size;
      }

      uint8_t* get_cdata() const
      {
        return this->data;
      }

      uint8_t* get_data()
      {
        return this->data;
      }

      uint32_t* get_size_ptr()
      {
        return &data_size;
      }

      uint8_t** get_data_ptr()
      {
        return &data;
      }
  };

} // namespace ble
