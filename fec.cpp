#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "fec.hpp"

consteval fec::trellis_table
get_default_trellis()
{
  fec::trellis_table ret;

  for(int j=0; j<TABLE_SIZE; j++){
    const uint8_t temp = j >> 1;

    ret.row[j].cur_state          = temp;
    ret.row[j].prev_state         = (temp >> 1) ^ ((j & 0x01) << 3);
    ret.row[j].new_path_metric    = -1;

    ret.row[j].ex_out           = 
      (((ret.row[j].cur_state & (1U << (0))) ^
        ((ret.row[j].cur_state & (1U << (1))) >> 1) ^
        ((ret.row[j].cur_state & (1U << (2))) >> 2) ^ 
        ((ret.row[j].cur_state & (1U << (3))) >> 3))) +
        (((ret.row[j].cur_state & (1U << (0))) ^
        ((ret.row[j].cur_state & (1U << (2))) >> 2) ^ 
        ((ret.row[j].cur_state & (1U << (3))) >> 3)) << 1U);
  }
  
  return ret;
}

constexpr fec::trellis_table def_table = get_default_trellis();

inline fec::trellis_row
fec::find_min_row(fec::trellis_table tab, uint8_t prev_state)
{
  uint32_t min = -1;
  fec::trellis_row ret;

  if(prev_state > 0x0F){
    for(int i=0; i<TABLE_SIZE;i++){
      if(tab.row[i].new_path_metric < min){
        min = tab.row[i].new_path_metric;
        ret = tab.row[i];
      }
    }

    return ret;
  } 

  for(int i=0; i<TABLE_SIZE;i++){
    if(tab.row[i].new_path_metric < min && tab.row[i].cur_state == prev_state){
      min = tab.row[i].new_path_metric;
      ret = tab.row[i];
    }
  }
  
  return ret;
}

inline void
fec::recalculate_paths(uint8_t val, trellis_table *tab)
{
  for(int i=0; i<TABLE_SIZE; i++){
    tab->row[i].br_metric = 
      (((val & 0x03) ^ (tab->row[i].ex_out)) >> 1)
      + ((((val & 0x03) ^ tab->row[i].ex_out)) & 0x01);

    if(tab->row[i].new_path_metric != -1)
      tab->row[i].new_path_metric += tab->row[i].br_metric;
  }
}

  inline void
fec::move_min_paths(trellis_table *prev, trellis_table *next)
{
  for(int i=0; i<TABLE_SIZE; i++){
    if(prev->row[i].new_path_metric == -1)
      continue;
    
    for(int j=0; j<TABLE_SIZE; j++){
      if(prev->row[i].cur_state != next->row[j].prev_state)
        continue;

      if(next->row[j].new_path_metric > prev->row[i].new_path_metric)
        next->row[j].new_path_metric = prev->row[i].new_path_metric;
    }
  }
}

  fec::trellis_table *
fec::create_trellis_matrix(uint32_t size)
{
  trellis_table *ret = (trellis_table *)calloc(size*4, sizeof(trellis_table));

  for(int i=0; i<size*4; i++){
    memcpy(&(ret[i]), &def_table, sizeof(trellis_table));
  }

  ret[0].row[0].new_path_metric = 0;
  ret[0].row[1].new_path_metric = 0;
  ret[0].row[2].new_path_metric = 0;
  ret[0].row[3].new_path_metric = 0;

  return ret;
}

  int
fec::encode_pack(uint8_t **data, uint32_t *size)
{
  if(*data == NULL)
    return -1;

  (*data) = (uint8_t *)reallocarray(*data, 
      (*size)*2, 
      sizeof(uint8_t));

  uint8_t reg = 0x00;
  uint16_t *temp = (uint16_t *)calloc((*size), sizeof(uint16_t));
  uint8_t dummy;

  for(int i=0; i<(*size); i++){
    for(int j=0; j<8; j++){
      reg <<= 1U;
      reg |= (((*data)[i] & (1U << j)) >> j);

      dummy = 0x00;

      dummy |= (reg & (1U << (0))) ^
          ((reg & (1U << (1))) >> 1) ^
          ((reg & (1U << (2))) >> 2) ^ 
          ((reg & (1U << (3))) >> 3);

      dummy |= ((reg & (1U << (0))) ^
          ((reg & (1U << (2))) >> 2) ^ 
          ((reg & (1U << (3))) >> 3)) << 1;

      temp[i] |= dummy << 2*j;
    }
  }

  memcpy(*data, temp, (*size)*2);
  *size = (*size) * 2;

  free(temp);

  return 0;
}

  int
fec::decode_pack(uint8_t **data, uint32_t *size)
{
  if(*data == NULL)
    return -1;

  trellis_table *tab = create_trellis_matrix((*size)*4 + 1);

  for(int i=0; i<*size; i++){
    uint8_t cur_byte = (*data)[i];
    for(int j=0; j<4; j++){
      recalculate_paths(cur_byte, &(tab[4*i + j]));
      move_min_paths(&(tab[4*i + j]), &(tab[4*i + j + 1]));
      cur_byte >>= 2;
    }
  }

  uint8_t *decoded = (uint8_t *)calloc((*size)/2, sizeof(uint8_t));
  uint8_t cur_state = 0xFF;

  for(int i=((*size)/2)-1; i>=0; i--){
    for(int j=7; j>= 0; j--){
      trellis_row min_row = find_min_row(tab[i*8 + j], cur_state);
      decoded[i] ^= ((min_row.cur_state & 0x01) << j);
      cur_state = min_row.prev_state;
    }
  }

  (*data) = (uint8_t *)reallocarray(*data, (*size/2), sizeof(uint8_t));

  memcpy((*data), decoded, (*size)/2);
  *size = (*size)/2;

  int ret = find_min_row(tab[(*size)*4 - 1], 0xFF).new_path_metric;

  free(tab);
  free(decoded);

  return ret;
}
