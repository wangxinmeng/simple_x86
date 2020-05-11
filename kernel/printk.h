#ifndef __PRINTK_H__
#define __PRINTK_H__

#include <basetype.h>
#include <console.h>
#include <stdarg.h>

#define PRINTK_BUF_MAX  4096
#define is_digit(c)	((c) >= '0' && (c) <= '9')

int printk(const char *fmt, ...);


#endif