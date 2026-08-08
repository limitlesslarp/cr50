/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CONFIG_CHIP_H
#define __CROS_EC_CONFIG_CHIP_H

#if defined(BOARD)
#include "core/cortex-m/config_core.h"
#include "hw_regdefs.h"
#endif

#define CONFIG_ROM_BASE         0x0
#define CONFIG_ROM_SIZE         0x2000

#define CONFIG_RAM_BASE         0x10000
#define CONFIG_RAM_SIZE         0x10000

#define CONFIG_FLASH_BANK_SIZE         0x800
#define CONFIG_FLASH_ERASE_SIZE        0x800
#define CONFIG_FLASH_WRITE_SIZE        4
#define CONFIG_FLASH_WRITE_IDEAL_SIZE  128
#define CONFIG_FLASH_ROW_SIZE          256

#define CONFIG_PROGRAM_MEMORY_BASE     0x40000
#define CONFIG_FLASH_SIZE              (512 * 1024)
#define CONFIG_FLASH_ERASED_VALUE32    (-1U)
#define CONFIG_RO_HEAD_ROOM            1024
#define CONFIG_RW_HEAD_ROOM            CONFIG_RO_HEAD_ROOM

#define CONFIG_INTERNAL_STORAGE
#define CONFIG_MAPPED_STORAGE
#define CONFIG_MAPPED_STORAGE_BASE CONFIG_PROGRAM_MEMORY_BASE

#define HOOK_TICK_INTERVAL_MS 500
#define HOOK_TICK_INTERVAL    (HOOK_TICK_INTERVAL_MS * MSEC)

#define CONFIG_STACK_SIZE 1024
#define IDLE_TASK_STACK_SIZE 512
#define TASK_STACK_SIZE 488
#define LARGER_TASK_STACK_SIZE 640

#define GPIO_PIN(port, index) (port), (1 << (index))
#define GPIO_PIN_MASK(port, mask) (port), (mask)
#define PLACEHOLDER_GPIO_BANK 0

#define PCLK_FREQ  (24 * 1000 * 1000)

#define CHIP_HAS_RO_B

#define CFG_FLASH_HALF (CONFIG_FLASH_SIZE >> 1)
#define CFG_TOP_SIZE  0x3000
#define CFG_TOP_A_OFF (CFG_FLASH_HALF - CFG_TOP_SIZE)
#define CFG_TOP_B_OFF (CONFIG_FLASH_SIZE - CFG_TOP_SIZE)

#define CONFIG_RO_MEM_OFF 0
#define CHIP_RO_B_MEM_OFF CFG_FLASH_HALF
#define CONFIG_RO_SIZE 0x4000

#define CONFIG_RW_MEM_OFF CONFIG_RO_SIZE
#define CONFIG_RW_B_MEM_OFF (CFG_FLASH_HALF + CONFIG_RW_MEM_OFF)
#define CONFIG_RW_SIZE (CFG_FLASH_HALF - CONFIG_RW_MEM_OFF - CFG_TOP_SIZE)

#define CONFIG_EC_PROTECTED_STORAGE_OFF    0
#define CONFIG_EC_PROTECTED_STORAGE_SIZE   CONFIG_FLASH_SIZE
#define CONFIG_EC_WRITABLE_STORAGE_OFF     0
#define CONFIG_EC_WRITABLE_STORAGE_SIZE    CONFIG_FLASH_SIZE
#define CONFIG_RO_STORAGE_OFF              0
#define CONFIG_RW_STORAGE_OFF              0
#define CONFIG_WP_STORAGE_OFF              0
#define CONFIG_WP_STORAGE_SIZE             CONFIG_EC_PROTECTED_STORAGE_SIZE

#define CONFIG_CUSTOMIZED_RO

#define CONFIG_FLASH_LOG_SPACE CONFIG_FLASH_BANK_SIZE
#define CONFIG_FLASH_LOG_BASE                                                  \
  (CONFIG_PROGRAM_MEMORY_BASE + CHIP_RO_B_MEM_OFF + CONFIG_RO_SIZE -          \
   CONFIG_FLASH_LOG_SPACE)

#define AP_RO_DATA_SPACE_SIZE CONFIG_FLASH_BANK_SIZE
#define AP_RO_DATA_SPACE_ADDR (CONFIG_FLASH_LOG_BASE - AP_RO_DATA_SPACE_SIZE)
#define MAX_RO_CODE_SIZE (CONFIG_RO_SIZE - CONFIG_FLASH_LOG_SPACE -           \
                          AP_RO_DATA_SPACE_SIZE)

#define CONFIG_LIBCRYPTOC

/* Not sure if this is different on citadel or not. I don't think it matters for loader only though.. will change in the future if needed. */
#define CONFIG_EXTENSION_COMMAND 0xbaccd00a

#define CONFIG_IRQ_COUNT (GC_INTERRUPTS_COUNT - 15)

#endif /* __CROS_EC_CONFIG_CHIP_H */
