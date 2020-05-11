#ifndef __STRING_H__
#define __STRING_H__

#include <basetype.h>

size_t strlen(const char *s);

char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t len);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t len);

void *memset(void *s, char c, size_t len);
void *memcpy(void *dst, const void *src, size_t len);
int memcmp(const void *v1, const void *v2, size_t n);

#endif
