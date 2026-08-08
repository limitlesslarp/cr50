/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "regtable.h"

#include "debug_printf.h"
#include "registers.h"
#include "rom_api.h"
#include "vectors.h"

#define REG_DIRTY reg_dirty
#define REG_COUNTER reg_counter
#define REG_TABLE_COUNT reg_table_count
#define REG_TABLE reg_table
#define REG_EXPECTED reg_expected
#define REG_PADDING 0x34687195u

static volatile uint32_t reg_dirty;
static volatile uint32_t reg_counter;
static volatile uint32_t reg_table_count;
static volatile uint32_t reg_table[0xc8];
static volatile uint32_t reg_expected[0xc8];

static void reg_mismatch(uint32_t step, uint32_t val, uint32_t violation);

void regtable_note(uint32_t reg, uint32_t val)
{
  uint32_t count = REG_TABLE_COUNT;
  uint32_t i;

  for (i = 0; i < count; i++) {
    if (REG_TABLE[i] == reg) {
      REG_EXPECTED[i] = REG_PADDING ^ val;
      REG_DIRTY++;
      return;
    }
  }

  if (count <= 0xc7)
    REG_TABLE_COUNT = count + 1;

  REG_TABLE[count] = reg;
  REG_EXPECTED[count] = REG_PADDING ^ val;
  REG_DIRTY++;
}

void regtable_dirty_increment(void)
{
  REG_DIRTY++;
}

uint32_t glitch_reg32_val(uint32_t reg, uint32_t val)
{
  uint32_t count = REG_TABLE_COUNT;
  uint32_t step;
  uint32_t actual;

  for (step = 0; step < count; step++) {
    if (REG_TABLE[step] != reg)
      continue;

    actual = REG32(REG_TABLE[step]);

    if ((actual ^ REG_PADDING) != REG_EXPECTED[step])
      reg_mismatch(step, actual, 2);
    if (actual != val)
      reg_mismatch(step, val, 2);

    return val;
  }

  if (count <= 0xc7)
    REG_TABLE_COUNT = count + 1;

  REG_TABLE[count] = reg;
  REG_EXPECTED[count] = REG_PADDING ^ val;
  REG32(reg) = val;

  return val;
}

static uint32_t reg_counter_next(void)
{
  REG_COUNTER++;
  return REG_COUNTER;
}

int verify_reg_counter(uint32_t expected, uint32_t violation)
{
  uint32_t err_resp = (glitch_reg32_val(
                         GBASE(FUSE) +
                         GC_FUSE_FW_DEFINED_BROM_ERR_RESPONSE_OFFSET,
                         GREG32(FUSE,
                                         FW_DEFINED_BROM_ERR_RESPONSE)) >> 2) & 3;

  if (REG_COUNTER != expected) {
    GREG32(ALERT, EXPECTATION) =
      (REG_COUNTER << 16) | 1 | (expected << 4);
    _purgatory(err_resp | violation);
  }

  if (REG_DIRTY & 1) {
    GREG32(ALERT, EXPECTATION) = (REG_DIRTY << 16) | 2;
    _purgatory(err_resp | violation);
  }

  return reg_counter_next();
}

int verify_regtable(uint32_t violation)
{
  uint32_t count;
  uint32_t step;
  uint32_t i;
  uint32_t val;

  GREG32(RNG, GLITCH_REFRESH) = 0;
  REG_DIRTY++;
  regtable_note(GBASE(RNG) +
                GC_RNG_GLITCH_REFRESH_OFFSET, 0);

  count = REG_TABLE_COUNT;
  step = ROM_RANDOM32();

  for (i = 0; i < count; i++) {
    step = (step + 0xd3) % REG_TABLE_COUNT;
    val = REG32(REG_TABLE[step]);

    if ((val ^ REG_PADDING) != REG_EXPECTED[step])
      reg_mismatch(step, val, violation);
  }

  return reg_counter_next();
}

static void reg_mismatch(uint32_t step, uint32_t val, uint32_t violation)
{
  uint32_t err_resp;

  debug_printf("!exp @%8x: %x vs. %x\n", (unsigned int)REG_TABLE[step],
               (unsigned int)(REG_EXPECTED[step] ^ REG_PADDING),
               (unsigned int)val);

  err_resp =
    (glitch_reg32_val(GBASE(FUSE) +
                      GC_FUSE_FW_DEFINED_BROM_ERR_RESPONSE_OFFSET,
                      GREG32(FUSE,
                                      FW_DEFINED_BROM_ERR_RESPONSE)) >> 2) & 3;

  GREG32(ALERT, EXPECTATION) = REG_TABLE[step];
  _purgatory(err_resp | violation);
}
