/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_DEBUG_PRINTF_H
#define __CROS_EC_CHIP_CITADEL_LOADER_DEBUG_PRINTF_H

__attribute__((__format__(__printf__, 1, 2)))
int debug_printf(const char *format, ...);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_DEBUG_PRINTF_H */
