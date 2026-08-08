/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "rom_flash.h"

#include "rom_api.h"

int rom_flash_erase(uint32_t bank, uint32_t page)
{
  return ROM_FLASH_ERASE(bank, page);
}

int rom_flash_write(uint32_t bank, uint32_t offset, const void *src,
                    uint32_t words)
{
  return ROM_FLASH_WRITE(bank, offset, src, words);
}

uint32_t rom_info_read(uint32_t index, uint32_t *dst, uint32_t mask)
{
  return ROM_INFO_READ(index, dst, mask);
}
