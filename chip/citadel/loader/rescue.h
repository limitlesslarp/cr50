/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_RESCUE_H
#define __CROS_EC_CHIP_CITADEL_LOADER_RESCUE_H

int rescue_sync(void);
void rescue(const void *hashes);
int check_engage_rescue(void);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_RESCUE_H */
