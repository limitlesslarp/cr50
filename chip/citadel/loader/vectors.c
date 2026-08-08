/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "vectors.h"

#include <stdint.h>

#include "registers.h"
#include "regtable.h"

static void wait_forever(void)
{
  while (1) {
#if defined(__arm__) || defined(__thumb__)
    __asm__ volatile("wfe");
#endif
  }
}

void common_handler(void)
{
  wait_forever();
}

void _purgatory(uint32_t level)
{
  uint32_t original = level;

  if (level == 3) {
    level = glitch_reg32_val(GBASE(FUSE) +
                             GC_FUSE_FW_DEFINED_BROM_ERR_RESPONSE_OFFSET,
                             GREG32(FUSE,
                                             FW_DEFINED_BROM_ERR_RESPONSE));
    GREG32(CRYPTO, WIPE_SECRETS) = 0xffffffff;
    GREG32(KEYMGR, WIPE_SECRETS) = 0xffffffff;
    GREG32(KEYMGR, WIPE_LAUNCH3) = 0xffffffff;

    if (level & 0x2000) {
      GREG32(GLOBALSEC, ROM_LOCK) = 0;
      GREG32(GLOBALSEC, FLASH_REGION0_CTRL) = 0;
      GREG32(GLOBALSEC, FLASH_REGION7_CTRL) = 0;
    }

    if (level & 0x4000) {
      GREG32(ALERT, AES_WIPE_SECRETS) = 0x8000000;
      GREG32(ALERT, AES_WIPE_COMMIT) = 1;
      GREG32(ALERT, FW_TRIGGER) = 0xa9;
    }

    if (level & 0x8000) {
      GREG32(ALERT, FLASH_RCV_WIPE) = 1;
      GREG32(ALERT, AES_WIPE_SECRETS) = 0x8000000;
      GREG32(ALERT, AES_WIPE_COMMIT) = 1;
      GREG32(ALERT, FW_TRIGGER) = 0xa9;
    }

    if (level & 0x1000) {
      do {
        level = GREG32(GLOBALSEC, CPU0_S_PERMISSION);
        GREG32(GLOBALSEC, CPU1_S_PERMISSION) = 0;
        GREG32(GLOBALSEC, CPU0_S_PERMISSION) = 0;
      } while (level != 0x33);
    }
  }

  if (original >= 2 && original <= 3)
    wait_forever();
}
