/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "system.h"

#include "registers.h"
#include "vectors.h"

struct citadel_rev {
  uint32_t register_values;
  const char *revision_str;
};

struct citadel_c2_rev {
  uint32_t addr;
  uint32_t mask;
  uint32_t val;
  const char *revision_str;
};

static const struct citadel_rev b2_rev_map[] = {
  {0xc00100, "B2-C"},
  {0x800000, "B2-P"},
  {0xc00059, "B2-D"},
};

static const struct citadel_c2_rev c2_rev_map[] = {
  {GC_FUSE_C2_PVT_MARKER_ADDR, 0xffff, 1, "C2-PVT"},
  {GBASE(FUSE) + GC_FUSE_FW_DEFINED_BROM_CONFIG1_OFFSET, 0xff, 0x86,
   "C2-DVT"},
};

static uint32_t get_b2_fuse_set_id(void)
{
  return ((REG32(GC_FUSE_RBOX_KEY_COMBO0_VAL_ADDR) >> 8) & 0xff) |
         ((REG32(GC_FUSE_RBOX_KEY_COMBO0_VAL_ADDR) & 0xff) << 16) |
         (((REG32(GC_FUSE_RBOX_POL_KEY1_IN_ADDR) >> 5) & 1) << 8);
}

static const char *get_b2_revision_str(void)
{
  uint32_t register_values = get_b2_fuse_set_id();
  int i;

  for (i = 0; i < sizeof(b2_rev_map) / sizeof(b2_rev_map[0]); i++) {
    if (b2_rev_map[i].register_values == register_values)
      return b2_rev_map[i].revision_str;
  }

  return "B2";
}

static const char *get_c2_revision_str(void)
{
  int i;

  for (i = 0; i < sizeof(c2_rev_map) / sizeof(c2_rev_map[0]); i++) {
    if ((REG32(c2_rev_map[i].addr) & c2_rev_map[i].mask) ==
        c2_rev_map[i].val)
      return c2_rev_map[i].revision_str;
  }

  return "C2-PROTO";
}

const char *citadel_get_revision_str(void)
{
  switch ((GREG32(PMU, STRAP_FUSE) >> 28) - 3) {
  case 0:
    return "B1";
  case 1:
    return get_b2_revision_str();
  case 2:
    return "C1";
  case 3:
    return get_c2_revision_str();
  default:
    return "UNKNOWN";
  }
}

void loader_panic_reboot(uint32_t delay_units)
{
  GREG32(ALERT, REBOOT_LEVEL) = 0;
  GREG32(PMU, PERICLKSET) = 0x400000;
  GREG32(TIMER, REBOOT_DELAY) = 0x3e800 * delay_units;
  GREG32(TIMER, CTRL) = 1;
  GREG32(PINMUX, OUTPUT_ENABLE) |= 0x20;
  GREG32(PMU, REBOOT_PARAM) = 4;
  GREG32(PMU, RESET_REQUEST) = 0x1e;
  GREG32(PMU, RESET_REQUEST) |= 1;
  _purgatory(3);
}
