#ifndef __STDARGS_H__
#define __STDARGS_H__

/* 编译器负责提供保存区域的空间大小 */
typedef __builtin_va_list va_list;

#define va_start(ap, last)  (__builtin_va_start(ap, last))
#define va_arg(ap, type)    (__builtin_va_arg(ap, type))
#define va_end(ap)          /* nothing */

#endif
