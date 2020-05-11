#ifndef __SERIAL_H__
#define __SERIAL_H__

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define CMO4 0x2E8

#define DEFAULT_SERIAL_PORT 0x3f8 /* ttyS0 */
#define DEFAULT_BAUD 9600

#define TXR             0       /*  Transmit register (WRITE) */
#define RXR             0       /*  Receive register  (READ)  */
#define IER             1       /*  Interrupt Enable          */
#define IIR             2       /*  Interrupt ID              */
#define FCR             2       /*  FIFO control              */
#define LCR             3       /*  Line control              */
#define MCR             4       /*  Modem control             */
#define LSR             5       /*  Line Status               */
#define MSR             6       /*  Modem Status              */
#define DLL             0       /*  Divisor Latch Low         */
#define DLM             1       /*  Divisor latch High        */

#define LSR_DATA        0x01    // Data available
#define LSR_TXRDY       0x20    // Transmit buffer avail
#define LSR_TSRE        0x40    // Transmitter off

#define LCR_DLAB        0x80    // Divisor latch access bit
#define LCR_WLEN8       0x03    // Wordlength: 8 bits

#define MCR_RTS         0x02    // RTS complement
#define MCR_DTR         0x01    // DTR complement
#define MCR_OUT2        0x08    // Out2 complement

void serial_putchar(int ch);
int serial_getchar(void);

void early_serial_init(void);

#endif
