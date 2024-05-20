#include <fstream>
#include <iostream>

#include <cstdint>

#include "mt_rng.hpp"
#include "tea.hpp"
#include "ble.hpp"

#define TEST_SIZE 0xFFFF

class
simulation{
  private:
    ble::packet         *pack;
    tea_ctx_t           *enc;

  private:
  inline void
    do_test()
  {
    char * fname = "/dev/random";
    read_n_bytes(fname, TEST_SIZE);

    ble::packet *begin_pack = new ble::packet(pack);

    enc = new tea_ctx_t(&(pack->data), &(pack->data_size));
    enc->encrypt_data();
    ble::packet *enc_pack = new ble::packet(pack);

    pack->distort_data(3);

    printf("This much %% of bits not matching : %f\n", ((float)pack->check_packet(enc_pack)/((float)pack->data_size*8.0f))*100.0f);
    enc->decrypt_data();

    printf("This much %% of bits not matching after decryption : %f\n", ((float)pack->check_packet(begin_pack)/((float)pack->data_size*8.0f))*100.0f);
  }

  inline void
    read_n_bytes(char *fname, uint32_t dsize)
  {
    std::ifstream rand;
    rand.open(fname);

    uint8_t msg[dsize];
    rand.read((char *)msg, dsize);
    rand.close();

    this->pack = new ble::packet(dsize, msg);
  }

  public:
    simulation(uint32_t pack_size)
  {
    char* fname = "/dev/random";
    read_n_bytes(fname, pack_size);

    init_mt(time(NULL));

    this->pack->data_size = pack_size;
  }

    simulation(uint32_t pack_size, char *fname)
  {
    read_n_bytes(fname, pack_size);
    this->pack->data_size = pack_size;
  }

  int
    run(int repetition_count)
  {
    for(int i=0; i<repetition_count; i++)
    {
      do_test();
    }

    return 0;
  }
};

int 
main(void)
{
  simulation *sim = new simulation(TEST_SIZE);
  sim->run(1);

  return 0;
}
