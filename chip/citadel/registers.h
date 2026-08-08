/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_REGISTERS_H
#define __CROS_EC_CHIP_CITADEL_REGISTERS_H

#include "common.h"
#include "config_chip.h"
#include "hw_regdefs.h"
#include "util.h"

#define GC_MODULE_OFFSET 0x10000

#define GBASE(module) \
  GC_##module##_BASE_ADDR
#define GOFFSET(module, reg) \
  GC_##module##_##reg##_OFFSET
#define GREG8(module, reg) \
  REG8(GBASE(module) + GOFFSET(module, reg))
#define GREG16(module, reg) \
  REG16(GBASE(module) + GOFFSET(module, reg))
#define GREG32(module, reg) \
  REG32(GBASE(module) + GOFFSET(module, reg))
#define GREG32_ADDR(module, reg) \
  REG32_ADDR(GBASE(module) + GOFFSET(module, reg))

#define GWRITE(module, reg, value) (GREG32(module, reg) = (value))
#define GREAD(module, reg) GREG32(module, reg)

#define GBASE_I(module, i) (GBASE(module) + (i) * GC_MODULE_OFFSET)
#define GREG32_I(module, i, reg) \
  REG32(GBASE_I(module, i) + GOFFSET(module, reg))
#define GREG32_ADDR_I(module, i, reg) \
  REG32_ADDR(GBASE_I(module, i) + GOFFSET(module, reg))

#define RO_A_HEADER \
  ((const struct SignedHeader *)(CONFIG_PROGRAM_MEMORY_BASE + CONFIG_RO_MEM_OFF))
#define RW_A_HEADER \
  ((const struct SignedHeader *)(CONFIG_PROGRAM_MEMORY_BASE + CONFIG_RW_MEM_OFF))
#define RW_B_HEADER \
  ((const struct SignedHeader *)(CONFIG_PROGRAM_MEMORY_BASE + CONFIG_RW_B_MEM_OFF))

#define RW_A_MAX_SIZE (CONFIG_FLASH_SIZE - CONFIG_RW_MEM_OFF)
#define RW_B_MAX_SIZE (CFG_FLASH_HALF - CONFIG_RW_MEM_OFF)
#define RESCUE_LIMIT (CONFIG_PROGRAM_MEMORY_BASE + CFG_FLASH_HALF)
#define RESCUE_RW_B_LIMIT \
  (CONFIG_PROGRAM_MEMORY_BASE + CONFIG_RW_B_MEM_OFF + \
   (0x60 - 8) * CONFIG_FLASH_BANK_SIZE)
#define RO_A_CERT_BASE (CONFIG_PROGRAM_MEMORY_BASE + 0x2800)
#define RO_A_CERT_END  (CONFIG_PROGRAM_MEMORY_BASE + 0x37fc)
#define RO_B_CERT_BASE (CONFIG_PROGRAM_MEMORY_BASE + CFG_FLASH_HALF + 0x2800)
#define RO_B_CERT_END  (CONFIG_PROGRAM_MEMORY_BASE + CFG_FLASH_HALF + 0x33fc)

#endif /* __CROS_EC_CHIP_CITADEL_REGISTERS_H */
