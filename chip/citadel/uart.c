#include "registers.h"

void uart_init(void)
{
  GREG32(PMU, PERICLKSET) |= 0x1000000;
  GREG32(PINMUX, UART_TX_SEL) = 0x51;
  GREG32(PINMUX, UART_RX_SEL) = 0x1b;
  GREG32(PINMUX, UART_CTL) = 7;
  GREG32(PINMUX, OUTPUT_ENABLE) &= 0xffffffdf;
  GREG32(UART, NCO) = 0x13a9;
  GREG32(UART, CTRL) = 3;
}

int uart_tx_ready(void)
{
  return (GREG32(UART, STATE) ^ 1) & 1;
}

void uart_write_char(uint32_t tx)
{
  while (!uart_tx_ready())
    ;

  GREG32(UART, WDATA) = tx;
}

void uart_tx_flush(){}

void uart_tx_start(){}
void uart_tx_stop(){}

int uart_init_done(){
  return 0;
}