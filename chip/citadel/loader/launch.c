/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "verify.h"

#include <stdint.h>
#include <string.h>

#include "debug_printf.h"
#include "hw_sha256.h"
#include "key_ladder.h"
#include "registers.h"
#include "regtable.h"
#include "rescue.h"
#include "rom_api.h"
#include "rom_flash.h"
#include "ro_uart.h"
#include "setup.h"

struct header_hashes {
  uint32_t img_hash[SHA256_DIGEST_WORDS];
  uint32_t fuses_hash[SHA256_DIGEST_WORDS];
  uint32_t info_hash[SHA256_DIGEST_WORDS];
};

static const uint32_t fuse_addr_mask_table[] = {
  0x00010001, 0x4001fff8, 0x0ffff000, 0x4001fff8,
  0xf0000000, 0x401040b4, 0x55555555, 0x401040bc,
  0x55555555, 0x401040c4, 0x55555555, 0x401040cc,
  0x55555555, 0x401040d4, 0x55555555, 0x401040dc,
  0x55555555,
};

static struct header_hashes hashes;
static uint32_t hash[SHA256_DIGEST_WORDS];
static uint32_t fuses[CITADEL_FUSE_MAX];
static uint32_t info[CITADEL_INFO_MAX];
static uint32_t rsa_key[0x62];

int image_a_newer(const struct SignedHeader *a, const struct SignedHeader *b)
{
  if (a->epoch_ != b->epoch_)
    return a->epoch_ > b->epoch_;
  if (a->major_ != b->major_)
    return a->major_ > b->major_;
  if (a->minor_ != b->minor_)
    return a->minor_ > b->minor_;
  if (a->timestamp_ != b->timestamp_)
    return a->timestamp_ > b->timestamp_;

  return 1;
}

void tryLaunch(uint32_t addr, uint32_t max_size)
{
  const struct SignedHeader *hdr = (const struct SignedHeader *)(uintptr_t)addr;
  const uint32_t *key_blob;
  uint32_t end = addr + max_size;
  uint32_t bit;
  uint32_t val;
  uint32_t fuse_addr;
  uint32_t fuse_mask;
  uint32_t vector_addr;
  int i;
  int pair;

  memset(hash, 0, sizeof(hash));

  if (hdr->magic != CITADEL_SIGNED_HEADER_MAGIC || hdr->image_size > max_size)
    return;

  key_blob = is_good_key(&hdr->keyid);
  if (!key_blob)
    return;

  memcpy(rsa_key, key_blob, 0x184);
  rsa_key[0x61] = key_blob[0x61];

  if (addr > hdr->ro_base)
    return;

  if (hdr->ro_max > end || addr > hdr->rx_base || end < hdr->rx_max)
    return;

  rescue_sync();
  rom_rsa_verify_wrapper(hdr->config1_ & 0x100,
                         rsa_key, (const void *)(uintptr_t)(addr + 4));

  GREG32(GLOBALSEC, RX_REGION_BASE) = hdr->rx_base & 0xffffffe0;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_RX_REGION_BASE_OFFSET,
                hdr->rx_base & 0xffffffe0);
  GREG32(GLOBALSEC, RX_REGION_SIZE) =
    ((hdr->rx_max - 1) & 0xffffffe0) - (hdr->rx_base & 0xffffffe0) + 0x20;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_RX_REGION_SIZE_OFFSET,
                GREG32(GLOBALSEC, RX_REGION_SIZE));
  GREG32(GLOBALSEC, FLASH_REGION0_CTRL) = 3;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_FLASH_REGION0_CTRL_OFFSET, 3);
  GREG32(GLOBALSEC, FLASH_REGION7_CTRL) = 3;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_FLASH_REGION7_CTRL_OFFSET, 3);

  ROM_SHA256_INIT();
  ROM_SHA256_UPDATE(&hdr->tag[0], 0x1c);
  ROM_SHA256_UPDATE(rsa_key, 0x184);
  ROM_SHA256_UPDATE(&hdr->image_size, hdr->image_size - 0x328);
  ROM_SHA256_FINAL(hashes.img_hash);

  for (i = 0; i < CITADEL_FUSE_MAX; i++)
    fuses[i] = CITADEL_FUSE_IGNORE;

  for (i = 0; i < 0x35; i++) {
    if (hdr->fusemap[i >> 5] & (1u << (i & 0x1f)))
      fuses[i] = REG32(GBASE(FUSE) + GC_FUSE_WORD0_OFFSET + i * 4);
  }

  for (i = 0x7f, pair = 2; i != 0x77; i--, pair += 2) {
    if (hdr->fusemap[i >> 5] & (1u << (i & 0x1f))) {
      fuse_addr = fuse_addr_mask_table[pair - 1];
      fuse_mask = fuse_addr_mask_table[pair];

      fuses[i] = REG32(fuse_addr) & fuse_mask;
    }
  }

  rom_sha256_hash(fuses, sizeof(fuses), hashes.fuses_hash);

  for (i = 0; i < CITADEL_INFO_MAX; i++)
    info[i] = CITADEL_INFO_IGNORE;

  for (i = 0; i < 0x80; i++) {
    bit = 1u << (i & 0x1f);

    if (hdr->infomap[i >> 5] & bit) {
      val = 0;

      info[i] = rom_info_read(i + 0x80, &val, bit) ^ val ^ info[i];
    }
  }

  rom_sha256_hash(info, sizeof(info), hashes.info_hash);

  check_engage_rescue();

  if (hdr->img_chk_ != hashes.img_hash[0] ||
      hdr->fuses_chk_ != hashes.fuses_hash[0] ||
      hdr->info_chk_ != hashes.info_hash[0]) {
    debug_printf("Himg =%X..%X : %x\n", (unsigned int)hdr->img_chk_,
                 (unsigned int)hashes.img_hash[0],
                 (unsigned int)hashes.img_hash[SHA256_DIGEST_WORDS - 1]);
    debug_printf("Hfss =%X..%X : %x\n", (unsigned int)hdr->fuses_chk_,
                 (unsigned int)hashes.fuses_hash[0],
                 (unsigned int)hashes.fuses_hash[SHA256_DIGEST_WORDS - 1]);
    debug_printf("Hinf =%X..%X : %x\n", (unsigned int)hdr->info_chk_,
                 (unsigned int)hashes.info_hash[0],
                 (unsigned int)hashes.info_hash[SHA256_DIGEST_WORDS - 1]);
    return;
  }

  rom_sha256_hash(&hashes, sizeof(hashes), hash);
  verify_regtable(hdr->expect_response_);
  mix_key_material(hdr->config1_ & 0x100,
                   rsa_key, hash);
  if ((GREG32(RSA, STATUS) & 1) == 0)
    return;

  verify_regtable(hdr->expect_response_);

  for (i = 0; i < 7; i++) {
    GREG32_ADDR(KEYMGR, LAUNCH_KEY0)[i] =
      hdr->signature[i] ^ hdr->epoch_;
    regtable_note((uintptr_t)&GREG32_ADDR(KEYMGR, LAUNCH_KEY0)[i],
                  hdr->signature[i] ^ hdr->epoch_);
  }

  GREG32(KEYMGR, LAUNCH_IV) = hdr->major_;
  regtable_note(GBASE(KEYMGR) +
                GC_KEYMGR_LAUNCH_IV_OFFSET, hdr->major_);
  GREG32(KEYMGR, LAUNCH_DST) = 0;
  regtable_note(GBASE(KEYMGR) +
                GC_KEYMGR_LAUNCH_DST_OFFSET, 0);
  GREG32(KEYMGR, LAUNCH_WORDS) = 2;
  regtable_note(GBASE(KEYMGR) +
                GC_KEYMGR_LAUNCH_WORDS_OFFSET, 2);
  GREG32(KEYMGR, LAUNCH_FLAGS) = 0;
  regtable_note(GBASE(KEYMGR) +
                GC_KEYMGR_LAUNCH_FLAGS_OFFSET, 0);
  ROM_KEY_LADDER_STEP(0x22, hash);
  GREG32(KEYMGR, LAUNCH_CTRL) = 0x30;
  regtable_note(GBASE(KEYMGR) +
                GC_KEYMGR_LAUNCH_CTRL_OFFSET, 2);

  GREG32(GLOBALSEC, LAUNCH_REGION_BASE) = addr & 0xffffffe0;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_LAUNCH_REGION_BASE_OFFSET,
                addr & 0xffffffe0);
  GREG32(GLOBALSEC, LAUNCH_REGION_SIZE) =
    ((hdr->image_size + addr - 1) & 0xffffffe0) -
    (addr & 0xffffffe0) + 0x20;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_LAUNCH_REGION_SIZE_OFFSET,
                GREG32(GLOBALSEC, LAUNCH_REGION_SIZE));
  GREG32(GLOBALSEC, FLASH_REGION0_CTRL) = 3;
  regtable_note(GBASE(GLOBALSEC) +
                GC_GLOBALSEC_FLASH_REGION0_CTRL_OFFSET, 3);

  if (hdr->config1_ & 2) {
    GREG32(GLOBALSEC, ROM_LOCK) = 0;
    regtable_note(GBASE(GLOBALSEC) +
                  GC_GLOBALSEC_ROM_LOCK_OFFSET, 0);
  }

  if (hdr->config1_ & 1) {
    GREG32(GLOBALSEC, CPU0_S_PERMISSION) = 0xc9;
    regtable_note(GBASE(GLOBALSEC) +
                  GC_GLOBALSEC_CPU0_S_PERMISSION_OFFSET, 0xc9);
    GREG32(GLOBALSEC, CPU1_S_PERMISSION) = 0xc9;
    regtable_note(GBASE(GLOBALSEC) +
                  GC_GLOBALSEC_CPU1_S_PERMISSION_OFFSET, 0xc9);
  }

  disable_hash_regions();
  verify_regtable(hdr->expect_response_);
  verify_reg_counter(0xd, hdr->expect_response_);

  GREG32(LADDER, WIPE_LAUNCH1) = 0xffffffff;
  GREG32(CRYPTO, WIPE_LAUNCH0) = 0xffffffff;
  GREG32(KEYMGR, WIPE_LAUNCH2) = 0xffffffff;
  vector_addr = addr + sizeof(struct SignedHeader);
  GREG32(M3, VTOR) = vector_addr;

  debug_printf("jump @%8x\n", (unsigned int)(vector_addr + 4));
  while (!uart_tx_done())
    ;

#if defined(__arm__) || defined(__thumb__)
  __asm__ volatile("msr msp, %0" : : "r"(*(uint32_t *)(uintptr_t)vector_addr));
#endif
  ((void (*)(void))(uintptr_t)*(uint32_t *)(uintptr_t)(vector_addr + 4))();
}
