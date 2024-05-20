#include <fstream>
#include <iostream>

#include <cstdint>
#include <cstring>

#include "mt_rng.hpp"
#include "tea.hpp"
#include "ble.hpp"

#define TEST_SIZE 0xFFFF

class
simulation
{
  ble::packet *pack;
  tea_ctx_t   *enc;

  inline void
  do_test()
  {
    read_n_bytes(TEST_SIZE);

    ble::packet *test_packet = new ble::packet(pack);

    printf("\nEncryption test\n");

    auto* data = test_packet->get_cdata();
    auto  size = test_packet->get_size();

    enc = new tea_ctx_t(&data, &size);
    enc->encrypt_data();
    enc->decrypt_data();

    auto res = std::memcmp(test_packet->get_cdata(),
                            pack->get_cdata(),
                            pack->get_size());
    res == 0 ?
            printf("\nEncryption: packets are equal\n") :
            printf("\nEncryption: packets differs\n");
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
