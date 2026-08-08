/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_ROM_API_H
#define __CROS_EC_CHIP_CITADEL_LOADER_ROM_API_H

#include <stdint.h>

#define ROM_RANDOM32 ((uint32_t (*)(void))0x50)
#define ROM_SHA256_INIT ((void (*)(void))0x54)
#define ROM_SHA256_UPDATE ((void (*)(const void *src, uint32_t len))0x5c)
#define ROM_SHA256_FINAL ((void (*)(void *dst))0x60)
#define ROM_RSA_VERIFY ((int (*)(const void *signature, const void *hash))0x74)
#define ROM_KEYMGR_PREPARE ((int (*)(void *dst))0x78)
#define ROM_KEY_LADDER_STEP ((void (*)(uint32_t cert, const void *hash))0x84)
#define ROM_FLASH_ERASE ((int (*)(uint32_t bank, uint32_t page))0x8c)
#define ROM_FLASH_WRITE \
  ((int (*)(uint32_t bank, uint32_t offset, const void *src, uint32_t words))0x90)
#define ROM_INFO_READ \
  ((uint32_t (*)(uint32_t index, uint32_t *dst, uint32_t mask))0x94)

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_ROM_API_H */
