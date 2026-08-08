/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_ROM_FLASH_H
#define __CROS_EC_CHIP_CITADEL_LOADER_ROM_FLASH_H

#include <stdint.h>

int rom_flash_erase(uint32_t bank, uint32_t page);
int rom_flash_write(uint32_t bank, uint32_t offset, const void *src,
                    uint32_t words);
uint32_t rom_info_read(uint32_t index, uint32_t *dst, uint32_t mask);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_ROM_FLASH_H */
