/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_KEY_LADDER_H
#define __CROS_EC_CHIP_CITADEL_LOADER_KEY_LADDER_H

#include <stdint.h>

int mix_key_material(uint32_t applysec, void *rsa_key, const void *hash);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_KEY_LADDER_H */
