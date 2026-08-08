/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "ro_uart.h"

#include "registers.h"

static int suppress_uart(void)
{
  if ((GREG32(FUSE, FW_DEFINED_BROM_CONFIG1) & 1) == 0)
    return GREG32(PMU, LONG_LIFE_SCRATCH1) & 1;

  return 1;
}

int uart_init(void)
{
  int result = suppress_uart();

  if (result == 0) {
    GREG32(PMU, PERICLKSET) |= 0x1000000;
    GREG32(PINMUX, UART_TX_SEL) = 0x51;
    GREG32(PINMUX, UART_RX_SEL) = 0x1b;
    GREG32(PINMUX, UART_CTL) = 7;
    GREG32(PINMUX, OUTPUT_ENABLE) &= 0xffffffdf;
    GREG32(UART, NCO) = 0x13a9;
    GREG32(UART, CTRL) = 3;
  }

  return result;
}

static int uart_tx_ready(void)
{
  if (suppress_uart() != 0)
    return 1;

  return (GREG32(UART, STATE) ^ 1) & 1;
}

int uart_tx_done(void)
{
  if (suppress_uart() != 0)
    return 1;

  return (GREG32(UART, STATE) & 0x30) == 0x30;
}

void uart_write_char(uint32_t tx)
{
  if (suppress_uart() != 0)
    return;

  while (!uart_tx_ready())
    ;

  GREG32(UART, WDATA) = tx;
}

int uart_rx_ready(void)
{
  return ((GREG32(UART, STATE) ^ 0x80) >> 7) & 1;
}

uint32_t read_uart_rx_data(void)
{
  while (!uart_rx_ready())
    ;

  return GREG32(UART, RDATA);
}

uint32_t get_cycle_count(void)
{
  return GREG32(M3, DWT_CYCCNT);
}
