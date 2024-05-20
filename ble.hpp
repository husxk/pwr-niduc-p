#include <cstdint>

namespace ble
{
  class packet
  {
    public:
      uint32_t       crc;
      uint8_t       *data;
      uint32_t      data_size;

    public:
      packet(uint32_t data_size, uint8_t *data);
      packet(const ble::packet *pack);
      ~packet();

      void print_packet();

      uint32_t check_packet(ble::packet pack);

      int distort_data(uint8_t percent);

      int add_crc();

      bool check_crc(uint32_t crc);
  };
};
