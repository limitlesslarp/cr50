/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "hw_sha256.h"

#include "rom_api.h"

void rom_sha256_hash(const void *src, uint32_t len, void *dst)
{
  ROM_SHA256_INIT();
  ROM_SHA256_UPDATE(src, len);
  ROM_SHA256_FINAL(dst);
}
