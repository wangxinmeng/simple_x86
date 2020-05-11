
#include <string.h>
#include <serial.h>
#include <console.h>
#include <printk.h>

void putchar(int ch)
{
    serial_putchar(ch);
}

void puts(const char *str)
{
    while (*str)
    {
        putchar(*str++);
    }
}

int getchar(void)
{
    return serial_getchar();
}

static char sbuf[GETS_BUFF_MAX];
const char *gets(void)
{
    int c, count = 0;
    memset(sbuf, 0, GETS_BUFF_MAX);
    while(count < GETS_BUFF_MAX)
    {
        c = getchar();
        if (c > 0)
        {
            if('\n' == c)
            {
                count++;
                break;
            }
            sbuf[count++] = c;
        }
    }
    sbuf[count] = '\0';
    return sbuf;
}

void console_init(void)
{
    early_serial_init();
    puts("\r\nconsole_init ok\r\n");
#if 0
    printk("test s %s\r\n", __FUNCTION__);
    printk("test d %d\r\n", 12345);
    printk("test ld %ld\r\n", 0xFEDCBA9876543210);
    printk("test ld %ld\r\n", 0x0123456789ABCDEF);
    printk("test x 0x%x\r\n", 0x76543210);
    printk("test x x 0x%x 0x%x\r\n", 0xFEDCBA9876543210, 0xFEDCBA9876543210);
    printk("test lx 0x%lx\r\n", 0xFEDCBA9876543210);
    printk("test X 0x%X\r\n", 0x76543210);
    printk("test X X 0x%X 0x%X\r\n", 0xFEDCBA9876543210, 0xFEDCBA9876543210);
    printk("test lX 0x%lX\r\n", (uint64_t)-1);
#endif
}


