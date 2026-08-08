/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "spiflash.h"

#include <stdint.h>

#include "registers.h"

int spi_rx_available(void)
{
  uint32_t rd = GREG32(SPI, RX_READ_PTR);
  uint32_t wr = GREG32(SPI, RX_WRITE_PTR);

  if (((wr ^ rd) & 0x1000) != 0)
    return 0x1000;

  rd &= 0xfff;
  wr &= 0xfff;
  if (rd == wr)
    return 0;
  if (rd > wr)
    wr += 0x1000;

  return wr - rd;
}

void spi_fifo_write(const uint32_t *src, uint32_t len)
{
  uint32_t base;
  uint32_t slot;

  if ((len & 3) || ((uintptr_t)src & 3) || len == 0)
    return;

  base = GREG32(SPI, TX_WRITE_PTR) - (uintptr_t)src;

  while (len) {
    slot = (((uintptr_t)src + base) >> 2) & 0x1ff;

    REG32(GC_SPI_TX_FIFO_BASE_ADDR + slot * 4) = *src++;
    len -= 4;
  }
}

void spi_fifo_read(uint32_t *dst, uint32_t len)
{
  uint32_t base;
  uint32_t slot;

  if ((len & 3) || ((uintptr_t)dst & 3) || len == 0)
    return;

  base = GREG32(SPI, RX_READ_PTR) - (uintptr_t)dst;

  while (len) {
    slot = (((uintptr_t)dst + base) >> 2) & 0x1ff;

    *dst++ = REG32(GC_SPI_RX_FIFO_BASE_ADDR + slot * 4);
    len -= 4;
  }
}

int spi_tx_idle(void)
{
  return ((GREG32(SPI, FIFO_STATE) ^ 4) >> 2) & 1;
}

int spi_fifo_init(void)
{
  GREG32(SPI, FIFO_RESET) = 0;
  GREG32(PINMUX, SPI_DIO3_CTL) = 0x17;
  while (!spi_tx_idle())
    ;
  GREG32(PINMUX, SPI_DIO1_CTL) = 7;
  GREG32(PINMUX, SPI_DIO0_CTL) = 7;
  GREG32(PINMUX, SPI_DIO2_CTL) = 3;
  GREG32(SPI, CTRL) = 0x70;
  GREG32(SPI, BOOT_STATUS) = 0;
  GREG32(SPI, TRIGGER) = 9;
  GREG32(SPI, TRIGGER) = 0x12;

  return 0;
}

int spi_fifo_advance(uint32_t rx_bytes, uint32_t tx_bytes)
{
  GREG32(SPI, RX_READ_PTR) += rx_bytes;
  GREG32(SPI, TX_WRITE_PTR) += tx_bytes;

  return GREG32(SPI, RX_READ_PTR);
}
