/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_HW_SHA256_H
#define __CROS_EC_CHIP_CITADEL_LOADER_HW_SHA256_H

#include <stdint.h>

#define SHA256_DIGEST_LENGTH 32
#define SHA256_DIGEST_WORDS (SHA256_DIGEST_LENGTH / sizeof(uint32_t))

void rom_sha256_hash(const void *src, uint32_t len, void *dst);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_HW_SHA256_H */
