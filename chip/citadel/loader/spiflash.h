/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_SPIFLASH_H
#define __CROS_EC_CHIP_CITADEL_LOADER_SPIFLASH_H

#include <stdint.h>

int spi_rx_available(void);
void spi_fifo_write(const uint32_t *src, uint32_t len);
void spi_fifo_read(uint32_t *dst, uint32_t len);
int spi_tx_idle(void);
int spi_fifo_init(void);
int spi_fifo_advance(uint32_t rx_bytes, uint32_t tx_bytes);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_SPIFLASH_H */
