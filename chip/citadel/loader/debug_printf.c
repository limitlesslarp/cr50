/* Copyright 2026 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "debug_printf.h"

#include <stdarg.h>
#include <stdint.h>

#include "ro_uart.h"
#include "loader_util.h"

static void print_uint(void (*addchar)(uint32_t c), uint32_t pad_width,
                       uint32_t val, uint32_t base)
{
  static const char hex[] = "0123456789ABCDEF";

  if (pad_width > 1 || val >= base)
    print_uint(addchar, pad_width - 1, val / base, base);

  addchar(hex[val % base]);
}

static int vdebug_printf(void (*addchar)(uint32_t c), const char *format,
                         va_list args)
{
  while (*format) {
    uint32_t c = *format++;
    uint32_t width = 0;

    if (c != '%') {
      if (c == '\n')
        addchar('\r');
      addchar(c);
      continue;
    }

    c = *format++;
    while (c >= '0' && c <= '9') {
      width = width * 10 + c - '0';
      c = *format++;
    }

    if (c == '%') {
      addchar('%');
    } else if (c == 'u') {
      print_uint(addchar, width, va_arg(args, unsigned int), 10);
    } else if (c == 'd') {
      int val = va_arg(args, int);
      if (val < 0) {
        addchar('-');
        val = -val;
      }
      print_uint(addchar, width, (uint32_t)val, 10);
    } else if (c == 'x' || c == 'X') {
      uint32_t val = va_arg(args, unsigned int);
      if (c == 'X') {
        val = citadel_bswap32(val);
        width = 8;
      }
      print_uint(addchar, width, val, 16);
    } else {
      addchar('%');
      if (c)
        addchar(c);
    }
  }

  return 0;
}

int debug_printf(const char *format, ...)
{
  va_list args;
  int result;

  va_start(args, format);
  result = vdebug_printf(uart_write_char, format, args);
  va_end(args);

  return result;
}
