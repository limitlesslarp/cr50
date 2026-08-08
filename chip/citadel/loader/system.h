/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_SYSTEM_H
#define __CROS_EC_CHIP_CITADEL_LOADER_SYSTEM_H

#include <stdint.h>

const char *citadel_get_revision_str(void);
void loader_panic_reboot(uint32_t delay_units);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_SYSTEM_H */
