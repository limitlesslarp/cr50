/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_VERIFY_H
#define __CROS_EC_CHIP_CITADEL_LOADER_VERIFY_H

#include <stdint.h>

#include "signed_header.h"

const uint32_t *is_good_key(const uint32_t *key);
int rom_rsa_verify_wrapper(int applysec, const void *key, const void *hash);
int image_a_newer(const struct SignedHeader *a, const struct SignedHeader *b);
void tryLaunch(uint32_t addr, uint32_t max_size);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_VERIFY_H */
