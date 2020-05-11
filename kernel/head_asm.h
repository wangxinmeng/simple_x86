#ifndef __HEAD_ASM_H__
#define __HEAD_ASM_H__

/*  **************************************************************** */

/* 64位代码段，默认非一致性，可读 */
#define CODE64_SEG_DESC_DEF(dpl) \
    E_CODE64_SEG_DESC(CODE_NO_ACCESSED, CODE_READABLE, CODE_NO_CONFORMING, dpl)
/* 64位数据段，默认可读写，向上扩展 */
#define DATA64_SEG_DESC_DEF(dpl)\
    E_DATA64_SEG_DESC(DATA_NO_ACCESSED, DATA_WRITEABLE, DATA_EXP_DIRECT_UP, dpl)
/* 32位代码段，默认非一致性，可读，基址0，limit 0xffffffff */
#define CODE32_SEG_DESC_DEF(dpl) \
    E_CODE32_SEG_DESC(0, 0xffffffff, dpl, CODE_NO_ACCESSED, CODE_READABLE, CODE_NO_CONFORMING)
/* 32位数据段，默认可读写，向上扩展，基址0，limit 0xffffffff */
#define DATA32_SEG_DESC_DEF(dpl) \
    E_DATA32_SEG_DESC(0, 0xffffffff, dpl, DATA_NO_ACCESSED, DATA_WRITEABLE, DATA_EXP_DIRECT_UP)

/* 64位long/ia-32e模式代码段和数据段索引/段选择子 */
#define K_CODE64_SELECTOR  P_SEG_DEF(1)
#define K_DATA64_SELECTOR  P_SEG_DEF(2)
#define U_CODE32_SELECTOR  P_SEG_DEF(3)
#define U_DATA32_SELECTOR  P_SEG_DEF(4)
#define U_CODE64_SELECTOR  P_SEG_DEF(5)
#define U_DATA64_SELECTOR  P_SEG_DEF(6)
#define K_CODE32_SELECTOR  P_SEG_DEF(7)
#define K_DATA32_SELECTOR  P_SEG_DEF(8)

/*  **************************************************************** */
/* 暂时以2MB为页大小，需要三级页表 */
#define MAX_PHY_ADDR  36 //先按照最小的支持长度36，其它40/52需要读取CPUID查询

/* CR3默认配置为写回，cache使能 */
#define CR3_DEF(phy_addr) \
    E_CR3(((phy_addr)>>12), MAX_PHY_ADDR, P_PAGE_WRITE_BACK, P_PAGE_CACHE_ENABLE)

/* PML4E默认配置，可读写，用户模式，写回，cache使能，未访问，不支持执行禁止功能 */
#define PML4E_DEF(phy_addr) \
    E_PML4E(((phy_addr)>>12), MAX_PHY_ADDR, P_PAGE_READ_WRITE, P_PAGE_USER, P_PAGE_WRITE_BACK, \
        P_PAGE_CACHE_ENABLE, P_PAGE_ACCESSED_NO, E_PAGE_XD_DISABLE)

/* PDPTE默认配置，可读写，用户模式，写回，cache使能，未访问，不支持执行禁止功能 */
#define PDPTE_DEF(phy_addr) \
    E_PDPTE(((phy_addr)>>12), MAX_PHY_ADDR, P_PAGE_READ_WRITE, P_PAGE_USER, P_PAGE_WRITE_BACK, \
        P_PAGE_CACHE_ENABLE, P_PAGE_ACCESSED_NO, E_PAGE_XD_DISABLE)

/* PDE配置为2MB物理页，可读写，超级模式，写回，cache使能，未访问，干净页，局部，不支持执行禁止功能 */
#define PDE_2M_DEF(phy_addr) \
    E_PDE_2MB(((phy_addr)>>21), MAX_PHY_ADDR, P_PAGE_READ_WRITE, P_PAGE_SUPER, P_PAGE_WRITE_BACK, \
        P_PAGE_CACHE_ENABLE, P_PAGE_ACCESSED_NO, P_PAGE_NOT_DIRTYED, P_PAGE_LOCAL, P_PAGE_PAT0, E_PAGE_XD_DISABLE)

/*  **************************************************************** */

#define SMAP 0x534d4150

#ifndef MB
#define MB(x)  ((x)<<20)
#endif

#define KERNEL_REAL_ADDR 0x100000
#define KERNEL_VIR_START 0xffff800000000000
#define KERNEL_STACK_ADDR (0x8000 - 8)
#define KERNEL_BP_ADDR    (uint64_t)0

#define PML4T_BASE_ORI_OFFSET  0x1000
#define PDPT_BASE_ORI_OFFSET   0x2000
#define PDT_BASE_ORI_OFFSET    0x3000


#endif
