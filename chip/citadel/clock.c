#include "common.h"

void clock_init(){}

uint32_t __hw_clock_source_read(){
  return 0x00000000;
}

int __hw_clock_source_init(uint32_t start_t){
  return 0x00000000;
}

uint32_t get_seconds_since_cold_boot(){
  return 0x00000000;
}