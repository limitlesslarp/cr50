/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CHIP_CITADEL_LOADER_RO_UART_H
#define __CROS_EC_CHIP_CITADEL_LOADER_RO_UART_H

#include <stdint.h>

int uart_init(void);
int uart_tx_done(void);
void uart_write_char(uint32_t tx);
int uart_rx_ready(void);
uint32_t read_uart_rx_data(void);
uint32_t get_cycle_count(void);

#endif /* __CROS_EC_CHIP_CITADEL_LOADER_RO_UART_H */
