#include "common.h"
#include "system.h"

void system_pre_init(void){}
void system_reset(int flags){

  while (1)
    asm("wfi");
}

const char *system_get_chip_vendor(void){
  return "Google";
}

const char *system_get_chip_name(void){
  return "Citadel";
}

const char *system_get_chip_revision(void){
  return "C2-PROTO"; // technically supposed to be C2-DVT, C2-PVT, or C2-PROTO..
}

int system_get_bbram(enum system_bbram_idx idx, uint8_t *value)
{
	return 0;
}

int system_set_bbram(enum system_bbram_idx idx, uint8_t value)
{
	return 0;
}

enum system_image_copy_t system_get_ro_image_copy(void){
  return SYSTEM_IMAGE_UNKNOWN;
}

// data_62930