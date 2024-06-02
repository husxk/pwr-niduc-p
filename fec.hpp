#include <cstdint>

#define TABLE_SIZE 16*2

class
fec
{
  public:
    typedef struct trellis_row{
      uint8_t       cur_state:4;
      uint8_t       prev_state:4;
      uint8_t       ex_out:2;
      uint8_t       br_metric:2;
      uint32_t      new_path_metric;
    }trellis_row;

    typedef struct trellis_table{
      trellis_row row[TABLE_SIZE];
    }trellis_table;

  private:
    inline void         recalculate_paths(uint8_t val, trellis_table *tab);
    inline void         move_min_paths(trellis_table *prev, trellis_table *next);
    trellis_table       *create_trellis_matrix(uint32_t size);
    trellis_row         find_min_row(trellis_table tab, uint8_t prev_state);


  public:
    int decode_pack(uint8_t **data, uint32_t *size);
    int encode_pack(uint8_t **data, uint32_t *size);
};
