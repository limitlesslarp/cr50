/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "key_ladder.h"

#include "hw_sha256.h"
#include "registers.h"
#include "rom_api.h"
#include "loader_util.h"

enum {
  KEY_SCRATCH_BYTES = 0x180,
  KEY_WORK_WORD = 0xf8 / sizeof(uint32_t),
  KEY_FLIP_WORD = 0xfc / sizeof(uint32_t),
  KEY_HASH_TMP_WORD = 0x17c / sizeof(uint32_t),
  KEY_SCRATCH_WORDS = KEY_SCRATCH_BYTES / sizeof(uint32_t),
  KEY_TOTAL_WORDS = KEY_SCRATCH_WORDS + SHA256_DIGEST_WORDS,
};

static uint32_t key_scratch[KEY_TOTAL_WORDS];

int mix_key_material(uint32_t applysec, void *rsa_key, const void *hash)
{
  volatile uint32_t *work = key_scratch;
  uint32_t r;
  int i;
  int result = ROM_KEYMGR_PREPARE((void *)(uintptr_t)work);

  if (result)
    return result;

  if (((uint8_t *)rsa_key)[0x180] == 0) {
    volatile uint32_t *p;
    uint32_t flip = key_scratch[KEY_FLIP_WORD];

    key_scratch[KEY_HASH_TMP_WORD] ^= flip;
    key_scratch[KEY_FLIP_WORD] = 0x1ffff ^ flip;

    for (p = &key_scratch[KEY_WORK_WORD];
         p != &key_scratch[KEY_HASH_TMP_WORD - 1]; p++)
      *p = ~*p;
  }

  r = ROM_RANDOM32();
  for (i = 0; i < 0x60; i++) {
    r = (r + 0xd3) % 0x60;
    work[r] ^= r + 0x1000;
  }

  r = ROM_RANDOM32();
  for (i = 0; i < 8; i++) {
    r = (r + 0xd3) & 7;
    work[r] ^= (r + 0x10) ^ citadel_bswap32(((const uint32_t *)hash)[7 - r]);
  }

  rom_sha256_hash(key_scratch, KEY_SCRATCH_BYTES,
                  &key_scratch[KEY_SCRATCH_WORDS]);

  for (i = 0; i < 8; i++)
    GREG32_ADDR(RSA, HASH0)[i] =
      key_scratch[KEY_SCRATCH_WORDS + i];
  GREG32(RSA, HASH_END) = 0;

  (void)applysec;
  return 0;
}
