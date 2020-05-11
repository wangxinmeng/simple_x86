#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <basetype.h>

#define GETS_BUFF_MAX  512

void puts(const char *str);
const char *gets(void);

void console_init(void);

#endif
