#include <string.h>
#include <printk.h>

typedef union tagwidth
{
    int32_t width;
    struct {
        int8_t selfwidth;
        int8_t reqwidth;
        int8_t pad;
        int8_t datawidth;
    }un;
}width_t;

static char tempbuf[PRINTK_BUF_MAX] = {0};

static uint8_t get_num_width(long long int num, int base)
{
    uint8_t i = 0;
    do{
        i ++;
    }while((num /= base) != 0);
    
    return i;
}
static char *ntostring(long long int num, int issigned, int base, char *str)
{
    char stack[21] = {0};
    int i = 0, val = 0, offset = 0;
    uint64_t tempnum = *(uint64_t *)(int64_t *)&num;
    if (issigned && num < 0)
    {
        tempnum = (uint64_t)(0 - num);
        *str ++ = '-';
    }
    do{
        val = tempnum % base;
        if ((base > 10)&&(val >= 10))
        {
            offset = 'A' - 10;
        }
        else
        {
            offset = '0';
        }
        stack[i++] = val + offset;
    }while(tempnum /= base);
    i--;
    do {
        *str++ = stack[i--];
    }while(i >= 0);
    return str;
}

int vprintk(char *buf, const char *fmt, va_list args)
{
    int len, base, issigned;
    width_t width;
    char *str, *s;
    long long int num; 
    
    for(str = buf; 
        *fmt && ((uint64_t)str < (uint64_t)(buf + PRINTK_BUF_MAX - 1)); 
        fmt++)
    {
        if (*fmt != '%')
        {
            *str++ = *fmt;
            continue;
        }
        width.width = 0;
        //目前只支持%s/%x/%X/%lx/%lX/%d/%ld/%c/%num/%0
        fmt++;
        /* 解析前缀 */
    repeat:
        switch(*fmt)
        {
            case '0':
                width.un.pad = '0';
                fmt++;
                goto repeat;
            case '1' ... '9':
                do {
                    width.un.reqwidth = width.un.reqwidth *10 + (*fmt) - '0';
                    fmt++;
                    if (!is_digit(*fmt))
                    {
                        break;
                    }
                } while(1);
                if (!width.un.pad)
                {
                    width.un.pad = ' ';
                }
                break;
            default:
                break;
        }
        width.un.datawidth = 4; //int
        if (*fmt == 'l')
        {
            width.un.datawidth = 8; //long long
            fmt++;
        }
        switch(*fmt)
        {
            case 'c':
                while(--width.un.reqwidth > 0)
                {
                    *str++ = width.un.pad;
                }
                *str++ = va_arg(args, int);
                break;
            case 's':
                s = va_arg(args, char*);
                if (!s)
                {
                    s = '\0';
                }
                len = strlen(s);
                while(len -- > 0)
                {
                    *str++ = *s++;
                }
                break;
            case 'd':
            case 'x':
            case 'X':
                base = 16;
                issigned = 0;
                if (width.un.datawidth == 8)
                {
                    num = va_arg(args, uint64_t);
                }
                else
                {
                    num = va_arg(args, uint32_t);
                }
                if (*fmt == 'd')
                {
                    base = 10;
                    issigned = 1;
                }
                width.un.selfwidth = get_num_width(num, base);
                while (width.un.reqwidth-- > width.un.selfwidth)
                {
                    *str ++ = width.un.pad;
                }
                str = ntostring(num, issigned, base, str);
                break;
            default:
                *str++ = '%';
                if (*fmt)
                {
                    *str++ = *fmt;
                }
                break;
        }
        
    }
    *str = '\0';
    return str - buf;
}

int printk(const char *fmt, ...)
{
    va_list args;
    int len;
    va_start(args, fmt);
    len = vprintk(tempbuf, fmt, args);
    va_end(args);
    puts(tempbuf);
    return len;
}


