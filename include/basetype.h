#ifndef __BASETYPE_H__
#define __BASETYPE_H__

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef BIT
#define BIT(x)  (1 << (x))
#endif

#ifndef B_SEL
#define B_SEL(value, bit_start, bit_space, bit_offset) \
    ((((value)>>(bit_start))&((BIT(bit_space))-1))<<(bit_offset))
#endif

#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)
#define GB(x) ((x) << 30)
#define TB(x) ((x) << 40)
#define PB(x) ((x) << 50)

#define GET_GB(x)  ((x)/GB(1))
#define GET_MB(x)  (((x)/MB(1))% KB(1))
#define GET_KB(x)  (((x)/KB(1))% KB(1))
#define GET_B(x)   ((x)% KB(1))

#define __always_inline inline __attribute__((always_inline))
#define __noinline __attribute__((noinline))
#define __noreturn __attribute__((noreturn))
#define __packed __attribute__((packed))

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short int uint16_t;
typedef signed short int int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;

typedef uint32_t size_t;
typedef int bool;

/* *
 * Rounding operations (efficient when n is a power of 2)
 * Round down to the nearest multiple of n
 * */
#define ROUNDDOWN(a, n) ({                                          \
            size_t __a = (size_t)(a);                               \
            (typeof(a))(__a - __a % (n));                           \
        })

/* Round up to the nearest multiple of n */
#define ROUNDUP(a, n) ({                                            \
            size_t __n = (size_t)(n);                               \
            (typeof(a))(ROUNDDOWN((size_t)(a) + __n - 1, __n));     \
        })

/* Round up the result of dividing of n */
#define ROUNDUP_DIV(a, n) ({                                        \
uint32_t __n = (uint32_t)(n);                           \
(typeof(a))(((a) + __n - 1) / __n);                     \
})

/* Return the offset of 'member' relative to the beginning of a struct type */
#define offsetof(type, member)                                      \
    ((size_t)(&((type *)0)->member))

/* *
 * to_struct - get the struct from a ptr
 * @ptr:    a struct pointer of member
 * @type:   the type of the struct this is embedded in
 * @member: the name of the member within the struct
 * */
#define to_struct(ptr, type, member)                               \
    ((type *)((char *)(ptr) - offsetof(type, member)))


#endif
