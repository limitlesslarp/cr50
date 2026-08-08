/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_LOADER_UTIL_H
#define __CROS_EC_CHIP_CITADEL_LOADER_LOADER_UTIL_H

#include <stdint.h>

static inline uint32_t citadel_bswap32(uint32_t value)
{
  return (value >> 24) | ((value >> 8) & 0xff00) |
         ((value << 8) & 0xff0000) | (value << 24);
}

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_LOADER_UTIL_H */
