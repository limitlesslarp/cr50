/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_REGTABLE_H
#define __CROS_EC_CHIP_CITADEL_LOADER_REGTABLE_H

#include <stdint.h>

void regtable_note(uint32_t reg, uint32_t val);
void regtable_dirty_increment(void);
uint32_t glitch_reg32_val(uint32_t reg, uint32_t val);
int verify_reg_counter(uint32_t expected, uint32_t violation);
int verify_regtable(uint32_t violation);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_REGTABLE_H */
