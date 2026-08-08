/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "rescue.h"

#include <stdint.h>

#include "debug_printf.h"
#include "hw_sha256.h"
#include "registers.h"
#include "rom_flash.h"
#include "ro_uart.h"
#include "signed_header.h"
#include "spiflash.h"

struct rescue_pkt {
  uint32_t hash[SHA256_DIGEST_WORDS];
  uint32_t frame_num;
  uint32_t flash_offset;
  uint32_t data[];
};

static struct rescue_pkt *rescue_pkt;
static uint32_t rescue_digest[SHA256_DIGEST_WORDS];
static uint32_t rescue_tx_digest[SHA256_DIGEST_WORDS];
static uint32_t rescue_write_buf[0x20];

int rescue_sync(void)
{
  debug_printf("oops?|");
  return uart_init();
}

void rescue(const void *hashes)
{
  uint32_t next_frame = 0;
  uint32_t write_base = 0;
  uint32_t buffered_words = 0;
  uint32_t buffered_and = 0xffffffff;
  const struct SignedHeader *hdr;
  uint32_t *data;
  uint32_t *p;
  uint32_t i;
  uint32_t mismatch;
  uint32_t frame;
  uint32_t erased;
  uint32_t page;
  uint32_t blank;
  uint32_t addr;
  uint32_t frame_offset;
  uint32_t word;
  uint8_t b;

  if (hashes == 0) {
    uart_write_char('e');
    uart_write_char('s');
    uart_write_char('c');
    uart_write_char('u');
    uart_write_char('e');
  } else {
    spi_fifo_init();
  }

  while (1) {
    if (hashes != 0) {
      if (next_frame == 0 && (GREG32(SPI_FLAG, STATUS) & 2) == 0)
        break;
      while (spi_rx_available() < 0x400)
        ;
      spi_fifo_read((uint32_t *)&rescue_pkt, sizeof(rescue_pkt));
    } else {
      for (i = 0; i < 0x400; i++)
        ((uint8_t *)&rescue_pkt)[i] = read_uart_rx_data();
    }

    rom_sha256_hash(&rescue_pkt->frame_num,
                    sizeof(rescue_pkt) - sizeof(rescue_pkt->hash),
                    rescue_digest);

    mismatch = 0;
    for (i = 0; i < 8; i++) {
      mismatch |= rescue_pkt->hash[i] ^ rescue_digest[i];
    }

    if (mismatch == 0) {
      frame = rescue_pkt->frame_num & 0xffffff;

      if (frame == next_frame) {
        rom_sha256_hash(&rescue_pkt, sizeof(rescue_pkt), rescue_digest);

        for (i = 0; i < 0x20; i++) {
          b = ((uint8_t *)rescue_digest)[i];

          ((uint8_t *)rescue_tx_digest)[i] = b ? b : 1;
        }

        if (frame == 0) {
          hdr = (const struct SignedHeader *)rescue_pkt->data;

          if (hashes != 0 && (GREG32(SPI_FLAG, STATUS) & 2) == 0)
            break;
          if (hdr->magic != CITADEL_SIGNED_HEADER_MAGIC)
            break;
          if (hdr->image_size > RW_B_MAX_SIZE)
            break;
          if (rescue_pkt->flash_offset != 0x4000)
            break;
          if (hdr->image_size & 0x7ff)
            break;

          erased = 0;
          rom_flash_write(0, 0x1000, &erased, 1);
          rom_flash_write(1, 0x1000, &erased, 1);

          for (page = 8; page <= 0x7f; page++) {
            if (hashes != 0)
              GREG32(SPI, BOOT_STATUS) = page;
            if (rom_flash_erase(0, page) != 0)
              return;
          }

          for (page = 8; page <= 0x5f; page++) {
            if (hashes != 0)
              GREG32(SPI, BOOT_STATUS) = page;
            if (rom_flash_erase(1, page) != 0)
              return;
          }

          GREG32(SPI, BOOT_STATUS) = 0;
          blank = 0xffffffff;

          for (addr = CONFIG_PROGRAM_MEMORY_BASE + CONFIG_RW_MEM_OFF + 0x20;
               addr != RESCUE_LIMIT + 0x20; addr += 0x20) {
            p = (uint32_t *)(uintptr_t)(addr - 0x20);

            blank &= p[0] & p[1] & p[2] & p[3] & p[4] & p[5] & p[6] & p[7];
          }

          for (addr = CONFIG_PROGRAM_MEMORY_BASE + CONFIG_RW_B_MEM_OFF + 0x20;
               addr != RESCUE_RW_B_LIMIT + 0x20; addr += 0x20) {
            p = (uint32_t *)(uintptr_t)(addr - 0x20);

            blank &= p[0] & p[1] & p[2] & p[3] & p[4] & p[5] & p[6] & p[7];
          }

          if (blank != 0xffffffff)
            break;

          write_base = rescue_pkt->flash_offset;
        }

        frame_offset = rescue_pkt->flash_offset;

        if (frame_offset > write_base + buffered_words * 4) {
          if (frame_offset & 0x7f)
            break;

          if (buffered_words != 0 && buffered_and != 0xffffffff) {
            if (rom_flash_write(write_base >> 18,
                                (write_base >> 2) & 0xffff,
                                rescue_write_buf,
                                buffered_words) != 0)
              break;
            buffered_and = 0xffffffff;
          }

          write_base = frame_offset;
          buffered_words = 0;
        }

        data = rescue_pkt->data;
        word = data[0];

        rescue_write_buf[buffered_words] = word;
        buffered_and &= word;
        buffered_words++;

        if (write_base + buffered_words * 4 > RESCUE_LIMIT)
          return;

        for (i = 1; i < 0xf6; i++) {
          if (buffered_words == 0x20) {
            if (buffered_and != 0xffffffff &&
                rom_flash_write(write_base >> 18,
                                (write_base >> 2) & 0xffff,
                                rescue_write_buf, 0x20) != 0)
              return;

            write_base += 0x80;
            buffered_and = 0xffffffff;
            buffered_words = 0;
          }

          if (write_base > 0x7ffff &&
              (int32_t)rescue_pkt->frame_num < 0)
            break;

          rescue_write_buf[buffered_words] = data[i];
          buffered_and &= data[i];
          buffered_words++;

          if (write_base + buffered_words * 4 > RESCUE_LIMIT)
            return;
        }

        next_frame = frame + 1;
      }
    }

    if (hashes != 0) {
      spi_fifo_write(rescue_tx_digest, SHA256_DIGEST_LENGTH);
      while (!spi_tx_idle())
        ;
      spi_fifo_advance(0x400, 0x20);
    } else {
      for (i = 0; i < 0x20; i++)
        uart_write_char(((uint8_t *)rescue_tx_digest)[i]);
    }

    if (mismatch == 0 &&
        (int32_t)rescue_pkt->frame_num < 0)
      break;
  }

  if (buffered_words != 0 && buffered_and != 0xffffffff)
    rom_flash_write(write_base >> 18, (write_base >> 2) & 0xffff,
                    rescue_write_buf, buffered_words);
}

int check_engage_rescue(void)
{
  if (uart_rx_ready() != 0 && read_uart_rx_data() == 'r')
    rescue(0);
  else
    debug_printf("no\n");

  return 0;
}
