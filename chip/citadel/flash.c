#include "common.h"

#define STUB(ret_type, name, ...) \
    ret_type name(__VA_ARGS__) { return EC_SUCCESS; }

STUB(int, flash_pre_init);
STUB(int, flash_physical_erase, int byte_offset, int num_bytes);
STUB(int, flash_physical_write, int byte_offset, int num_bytes, const char *data);