/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_VECTORS_H
#define __CROS_EC_CHIP_CITADEL_LOADER_VECTORS_H

#include <stdint.h>

void common_handler(void);
void _purgatory(uint32_t level);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_VECTORS_H */
