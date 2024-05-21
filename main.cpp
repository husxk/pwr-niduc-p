#include <fstream>
#include <iostream>

#include <cstdint>
#include <cstring>

#include "mt_rng.hpp"
#include "tea.hpp"
#include "ble.hpp"
#include "data_process.hpp"

#define TEST_SIZE 0xFFFF

class
simulation
{
  ble::packet  *pack;
  tea_ctx_t    *enc;
  data_process *process;

  inline void
  do_test()
  {
    read_n_bytes(TEST_SIZE);

    ble::packet *test_packet = new ble::packet(pack);

    printf("\nEncryption test\n");

    uint8_t**  data = test_packet->get_data_ptr();
    uint32_t* size = test_packet->get_size_ptr();

    enc = new tea_ctx_t(data, size);
    enc->encrypt_data();
    enc->decrypt_data();

    auto res = std::memcmp(test_packet->get_cdata(),
                            pack->get_cdata(),
                            pack->get_size());

    res == 0 ?
            printf("Encryption: packets are equal\n") :
            printf("Encryption: packets differs\n");

    free(test_packet);

    printf("\nCRC test\n");
    printf("\nCalculate CRC 2 times test\n");

    uint32_t crc = process->generate_crc(pack->get_cdata(), pack->get_size());
    uint32_t crc2 = process->generate_crc(pack->get_cdata(), pack->get_size());

    bool result = crc == crc2;

    result == true ?
            printf("CRC: CRC equals\n") :
            printf("CRC: CRC differs\n");

    printf("\nChange data value and check if CRC catched that\n");
  
    test_packet = new ble::packet(pack);
    uint8_t* t_data = test_packet->get_data();
    t_data[0] ^= 0x1;

    crc2 = process->generate_crc(test_packet->get_cdata(), test_packet->get_size());

    result = crc == crc2;

    result == true ?
            printf("CRC: CRC equals\n") :
            printf("CRC: CRC differs\n");

    free(test_packet);


    printf("\nWhitening test\n");

    test_packet = new ble::packet(pack);

    process->whitening(test_packet->get_data(), test_packet->get_size());
    process->whitening(test_packet->get_data(), test_packet->get_size());

    res = std::memcmp(test_packet->get_cdata(),
                       pack->get_cdata(),
                       pack->get_size());

    res == 0 ?
            printf("Whitening: packets are equal\n") :
            printf("Whitening: packets differs\n");

    free(test_packet);
  }

  inline void
  read_n_bytes(uint32_t dsize)
  {
    uint8_t msg[dsize];

    for(size_t i = 0; i < dsize; i++)
      msg[i] = (uint8_t) get_next(0, 0xFFFFFFFF);

    if(this->pack != NULL)
      free(this->pack);

    this->pack = new ble::packet(dsize, msg);
  }

  public:

  simulation()
  {
    process = new data_process();
  }

  void
  run(int repetition_count)
  {
    for(int i = 0; i < repetition_count; i++)
      do_test();
  }
};

int 
main(void)
{
  init_mt(time(NULL));

  simulation *sim = new simulation();
  sim->run(1);

  return 0;
}
