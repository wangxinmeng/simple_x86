/******************************************************************************

  Copyright (C), 2020, shinetimes.

 ******************************************************************************
  File Name     : serial.c
  Version       : Initial Draft
  Author        : shinetimes
  Created       : 2020/5/5
  Last Modified :
  Description   : 8250/16550 driver
  Function List :
              early_serial_hw_init
              early_serial_init
  History       :
  1.Date        : 2020/5/5
    Author      : shinetimes
    Modification: Created file

******************************************************************************/

#include <x86_64_asm.h>
#include <serial.h>

int early_serial_base;

void serial_putchar(int ch)
{
    uint16_t timeout = 0xffff;
    while((((inb(early_serial_base + LSR))& LSR_TXRDY) == 0) && (--timeout))
    {
        cpu_relax();
    }
    outb(early_serial_base + TXR, ch);
}

int serial_getchar(void)
{
    int c = -1;
    if (!(inb(early_serial_base + LSR) & LSR_DATA)) {
        return c;
    }
    c = inb(early_serial_base + RXR);
    if (c == 127) {
        c = '\b';
    }
    return c;
}


void early_serial_hw_init(uint16_t port, uint32_t baud)
{
    uint8_t c;
    uint16_t divisor;

    outb(port + LCR, LCR_WLEN8);  /* 8n1 */
    outb(port + IER, 0);    /* no interrupt */
    outb(port + FCR, 0);    /* no fifo */
    outb(port + MCR, MCR_RTS | MCR_DTR);  /* DTR + RTS */

    divisor = 115200/baud;
    c = inb(port + LCR);
    outb(port + LCR, c | LCR_DLAB);
    outb(port + DLL, divisor & 0xff);
    outb(port + DLM, (divisor >> 8) & 0xff);
    outb(port + LCR, c & ~LCR_DLAB);

    early_serial_base = port;
}

void early_serial_init(void)
{
    early_serial_hw_init(COM1, DEFAULT_BAUD);
}

