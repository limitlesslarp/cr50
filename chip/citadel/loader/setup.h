/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_SETUP_H
#define __CROS_EC_CHIP_CITADEL_LOADER_SETUP_H

void setup_hash_regions(void);
void disable_hash_regions(void);
void protect_ro_a_flash(void);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_SETUP_H */
