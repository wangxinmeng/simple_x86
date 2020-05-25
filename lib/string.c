#include <string.h>

size_t strlen(const char *s)
{
    size_t cnt = 0;
    while (*s++ != '\0')
    {
        cnt ++;
    }
    return cnt;
}

char *strcpy(char *dst, const char *src)
{
    char *p = dst;
    while ((*p++ = *src++) != '\0');
    return dst;
}

char *strncpy(char *dst, const char *src, size_t len)
{
    char *p = dst;
    while ((len-- > 0)&&((*p++ = *src++) != '\0'));
    return dst;
}

int strcmp(const char *s1, const char *s2)
{
    while((*s1 != '\0')&&(*s1 == *s2))
    {
        s1 ++;
        s2 ++;
    }
    return (int)((uint8_t)*s1 - (uint8_t)*s2);
}

int strncmp(const char *s1, const char *s2, size_t len)
{
    while((len-- > 0)&&(*s1 != '\0')&&(*s1 == *s2))
    {
        s1 ++;
        s2 ++;
    }
    return (int)((uint8_t)*s1 - (uint8_t)*s2);
}
char * strfind(const char *s, char c) 
{
    while (*s != '\0') {
        if (*s == c) {
            break;
        }
        s ++;
    }
    return (char *)s;
}

void *memset(void *s, char c, size_t len)
{
    char *p = (char *)s;
    while(len-- > 0)
    {
        *p++ = c;
    }
    return s;
}

void *memcpy(void *dst, const void *src, size_t len)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    while (len -- >0)
    {
        *d++ = *s++;
    }
    return dst;
}

int memcmp(const void *v1, const void *v2, size_t n)
{
    const uint8_t *s1 = (const uint8_t *)v1;
    const uint8_t *s2 = (const uint8_t *)v2;
    while((n-- > 0)&&(*s1 ++ == *s2 ++));
    return (int)(*s1 - *s2);
}

