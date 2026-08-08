/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "setup.h"

#include <stdint.h>

#include "hw_sha256.h"
#include "registers.h"
#include "regtable.h"

static uint32_t hash_region0[SHA256_DIGEST_WORDS];
static uint32_t hash_region1[SHA256_DIGEST_WORDS];
static uint32_t hash_region2[SHA256_DIGEST_WORDS];

void setup_hash_regions(void)
{
  GREG32(GLOBALSEC, HASH_REGION0_BASE) = (uint32_t)(uintptr_t)hash_region0;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION0_BASE_OFFSET,
                (uint32_t)(uintptr_t)hash_region0);
  GREG32(GLOBALSEC, HASH_REGION0_SIZE) = 0x20;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION0_SIZE_OFFSET, 0x20);
  GREG32(GLOBALSEC, HASH_REGION0_CTRL) = 1;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION0_CTRL_OFFSET, 1);

  GREG32(GLOBALSEC, HASH_REGION1_BASE) = (uint32_t)(uintptr_t)hash_region1;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION1_BASE_OFFSET,
                (uint32_t)(uintptr_t)hash_region1);
  GREG32(GLOBALSEC, HASH_REGION1_SIZE) = 0x20;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION1_SIZE_OFFSET, 0x20);
  GREG32(GLOBALSEC, HASH_REGION1_CTRL) = 1;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION1_CTRL_OFFSET, 1);

  GREG32(GLOBALSEC, HASH_REGION2_BASE) = (uint32_t)(uintptr_t)hash_region2;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION2_BASE_OFFSET,
                (uint32_t)(uintptr_t)hash_region2);
  GREG32(GLOBALSEC, HASH_REGION2_SIZE) = 0x20;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION2_SIZE_OFFSET, 0x20);
  GREG32(GLOBALSEC, HASH_REGION2_CTRL) = 1;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION2_CTRL_OFFSET, 1);
}

void disable_hash_regions(void)
{
  GREG32(GLOBALSEC, HASH_REGION0_CTRL) = 7;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION0_CTRL_OFFSET, 7);
  GREG32(GLOBALSEC, HASH_REGION1_CTRL) = 7;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION1_CTRL_OFFSET, 7);
  GREG32(GLOBALSEC, HASH_REGION2_CTRL) = 7;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_HASH_REGION2_CTRL_OFFSET, 7);
}

void protect_ro_a_flash(void)
{
  GREG32(GLOBALSEC, LAUNCH_REGION_BASE) =
    CONFIG_PROGRAM_MEMORY_BASE + CONFIG_RO_MEM_OFF;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_LAUNCH_REGION_BASE_OFFSET,
                CONFIG_PROGRAM_MEMORY_BASE + CONFIG_RO_MEM_OFF);
  GREG32(GLOBALSEC, LAUNCH_REGION_SIZE) = 0x80000;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_LAUNCH_REGION_SIZE_OFFSET, 0x80000);
  GREG32(GLOBALSEC, FLASH_REGION0_CTRL) = 7;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_FLASH_REGION0_CTRL_OFFSET, 7);
}
