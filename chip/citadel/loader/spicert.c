/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "spicert.h"

#include <stdint.h>

#include "debug_printf.h"
#include "hw_sha256.h"
#include "registers.h"
#include "rescue.h"
#include "rom_flash.h"
#include "ro_uart.h"
#include "spiflash.h"
#include "system.h"

void check_engage_spirescue(void)
{
  if ((GREG32(PMU, BOOT_FLAGS) & 1) == 0)
    return;

  GREG32(PINMUX, SPI_CS_SEL) = 0xc;
  GREG32(PINMUX, SPI_RX_SEL) = 0x11;
  GREG32(PINMUX, OUTPUT_ENABLE) &= 0xfffeffff;
  GREG32(PINMUX, OUTPUT_ENABLE) &= 0x7fffffff;

  if ((GREG32(SPI_FLAG, STATUS) & 2) == 0)
    return;

  debug_printf("SPI rescue!\n");
  rescue((const void *)1);
  debug_printf("retval = %d\n", 0);
  while (!uart_tx_done())
    ;
  while ((GREG32(SPI_FLAG, STATUS) & 2) != 0)
    ;
  loader_panic_reboot(2);
}

int spicert(void)
{
  uint32_t blank = 0xffffffff;
  uint8_t cert[0x1c00];
  uint8_t digest[0x20];
  uint32_t *p;
  uint32_t offset;
  uint32_t diff;
  int i;

  for (p = (uint32_t *)(uintptr_t)RO_A_CERT_BASE;
       p != (uint32_t *)(uintptr_t)RO_A_CERT_END; p++)
    blank &= *p;
  for (p = (uint32_t *)(uintptr_t)RO_B_CERT_BASE;
       p != (uint32_t *)(uintptr_t)RO_B_CERT_END; p++)
    blank &= *p;

  if (blank != 0xffffffff)
    return 1;
  if ((GREG32(SPI_FLAG, STATUS) & 1) == 0)
    return 2;

  for (offset = 0; offset < sizeof(cert); offset += 0x400) {
    while (spi_rx_available() != 0x400)
      ;
    spi_fifo_read((uint32_t *)(cert + offset), 0x400);
    spi_fifo_write((const uint32_t *)(cert + offset), 0x400);
    spi_fifo_advance(0x400, 0x400);
  }

  rom_sha256_hash(cert, 0x1be0, digest);

  diff = 0;
  for (i = 0; i < 0x20; i++)
    diff |= cert[0x1be0 + i] ^ digest[i];
  if (diff != 0)
    return 3;

  if (rom_flash_write(0, 0xa00, cert, 0x400) != 0)
    return 4;
  if (rom_flash_write(1, 0xa00, cert + 0x1000, 0x300) != 0)
    return 5;

  return 0;
}

void check_engage_spicert(void)
{
  int result;

  if ((GREG32(FUSE, FW_DEFINED_BROM_CONFIG1) & 0x10) != 0 ||
      (GREG32(SPI_FLAG, STATUS) & 1) == 0)
    return;

  spi_fifo_init();
  debug_printf("boot :");
  result = spicert();

  debug_printf("%d\n", result);
  while (!spi_tx_idle())
    ;
  GREG32(SPI, BOOT_STATUS) = result | 0x80;
  while (spi_tx_idle())
    ;
  while (!spi_tx_idle())
    ;
}
