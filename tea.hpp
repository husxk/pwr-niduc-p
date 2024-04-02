#pragma once

#define  TEA_DELTA 0x9e3779b9        // Key schedule constant
#define  TEA_SUM_DECRYPT 0xc6ef3720
#define  TEA_KEY_SIZE 4
#define  TEA_ITER_COUNT 32

#define  TEA_DATABLOCK_SIZE 8

class tea_ctx_t {
  public:
    uint32_t    key[TEA_KEY_SIZE];
    uint32_t    *data;
    uint32_t    data_size;

    tea_ctx_t(uint8_t *data, uint32_t u8_datasize);
    ~tea_ctx_t();
    
    void print_data(void);
    void encrypt_data(void);
    void decrypt_data(void);

  private:
    inline void encrypt_8B(int index);
    inline void decrypt_8B(int index);
};
