#ifndef __CPU_H__
#define __CPU_H__

#include <basetype.h>

#define BIOS_MEM_ZONE_MAX  16

typedef struct __packed tag_bios_mem_zone
{
    union {
        struct {
            uint32_t low;
            uint32_t high;
        }u32;
        uint64_t u64;
    }base_addr;
    union {
        struct {
            uint32_t low;
            uint32_t high;
        }u32;
        uint64_t u64;
    }length;
    uint32_t type;
}BIOS_MEM_ZONE_S;

typedef struct __packed tag_bios_mem
{
    uint32_t zone_num;
    BIOS_MEM_ZONE_S zone[BIOS_MEM_ZONE_MAX];
}BIOS_MEM_S ;

typedef struct tag_cache_info
{
    uint8_t  level, type;
    uint16_t line_size, line_partition, way;
    uint32_t set;
}CACHE_INFO_S;

typedef enum tag_cache_type
{
    CACHE_L1D = 0,
    CACHE_L1I,
    CACHE_L2,
    CACHE_L3,
    CACHE_MAX
}CACHE_TYPE_E;

#define VENDOR_NAME_MAX_LEN   16
typedef struct tag_cpuid_info
{
    uint32_t basic_max_func;
    uint32_t extended_max_func;
    char vendor_name[VENDOR_NAME_MAX_LEN];
    uint16_t display_family;
    uint16_t display_model;
    uint16_t cache_line;
    uint16_t core_num;
    uint8_t max_phy_addr;
    uint8_t max_linear_addr;
    CACHE_INFO_S cache_info[CACHE_MAX];
}CPUID_INFO_S;

static inline void get_cpuid(uint32_t Main,uint32_t Sub,
                            uint32_t * a, uint32_t * b,
                            uint32_t * c, uint32_t * d)
{
    __asm__ __volatile__    (   "cpuid  \n\t"
                    :"=a"(*a),"=b"(*b),"=c"(*c),"=d"(*d)
                    :"0"(Main),"2"(Sub)
                );
}


void cpu_init(void);

#endif
