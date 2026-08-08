/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <stdint.h>

#include "debug_printf.h"
#include "registers.h"
#include "regtable.h"
#include "rescue.h"
#include "ro_uart.h"
#include "setup.h"
#include "signed_header.h"
#include "spicert.h"
#include "system.h"
#include "verify.h"

int reset(void)
{
  const struct SignedHeader *first;
  const struct SignedHeader *second;
  uint32_t scratch;

  uart_init();
  GREG32(KEYMGR, MODE) = 0xfc030;
  debug_printf("\nBldr |%u\n", (unsigned int)get_cycle_count());
  GREG32(FUSE, FW_DEFINED_BROM_CONFIG1) = 0x10000;
  regtable_dirty_increment();
  regtable_note(GBASE(FUSE) +
                GC_FUSE_FW_DEFINED_BROM_CONFIG1_OFFSET, 0x10000);
  verify_regtable(3);
  verify_reg_counter(9, 3);
  setup_hash_regions();
  protect_ro_a_flash();
  GREG32(KEYMGR, MODE) = 0xfc030;
  check_engage_spicert();
  check_engage_spirescue();

  if (image_a_newer(RW_A_HEADER, RW_B_HEADER)) {
    first = RW_A_HEADER;
    second = RW_B_HEADER;
  } else {
    first = RW_B_HEADER;
    second = RW_A_HEADER;
  }

  GREG32(PMU, SCRATCH_WR_EN) = 1;
  scratch = GREG32(PMU, RETRY_COUNTER);
  debug_printf("retry|%u\n", (unsigned int)scratch);
  GREG32(PMU, RETRY_COUNTER) = scratch + 1;
  regtable_dirty_increment();
  regtable_note(GBASE(PMU) +
                GC_PMU_RETRY_COUNTER_OFFSET, scratch + 1);
  GREG32(PMU, SCRATCH_WR_EN) = 0;
  regtable_dirty_increment();
  regtable_note(GBASE(PMU) +
                GC_PMU_SCRATCH_WR_EN_OFFSET, 0);

  if (scratch > 5) {
    const struct SignedHeader *tmp = first;

    first = second;
    second = tmp;
  }

  GREG32(KEYMGR, MODE) = 0xfc030;
  tryLaunch((uint32_t)(uintptr_t)first,
            first == RW_B_HEADER ? RW_B_MAX_SIZE :
            RW_A_MAX_SIZE);
  tryLaunch((uint32_t)(uintptr_t)second,
            second == RW_B_HEADER ? RW_B_MAX_SIZE :
            RW_A_MAX_SIZE);

  debug_printf("No valid RW image found.\n");
  while (!uart_tx_done())
    ;
  rescue_sync();
  while (read_uart_rx_data() != 'r')
    ;
  rescue(0);
  debug_printf("Rebooting...\n");
  while (!uart_tx_done())
    ;
  loader_panic_reboot(0xa);

  return 0;
}
