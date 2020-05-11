/******************************************************************************

                  版权所有 (C), 2020, shinetimes

 ******************************************************************************
  文 件 名   : loader_asm.h
  版 本 号   : 初稿
  作    者   : shinetimes
  生成日期   : 2020年5月4日
  最近修改   :
  功能描述   : loader需要创建段描述符和页表的默认参数
  函数列表   :
  修改历史   :
  1.日    期   : 2020年5月4日
    作    者   : shinetimes
    修改内容   : 创建文件

******************************************************************************/

#ifndef __LOADER_ASM_H__
#define __LOADER_ASM_H__
/*  **************************************************************** */

/* 32位代码段 默认配置为可读，非一致性，0特权级 */
#define P_CODE_SEG_DESC_DEF(base, limit) \
    P_CODE_SEG_DESC(base, limit, DPL(0), CODE_NO_ACCESSED, CODE_READABLE, CODE_NO_CONFORMING)
/* 32位默认配置为可读写，向上扩展， 0特权级 */
#define P_DATA_SEG_DESC_DEF(base, limit) \
    P_DATA_SEG_DESC(base, limit, DPL(0), DATA_NO_ACCESSED, DATA_WRITEABLE, DATA_EXP_DIRECT_UP)

/* 保护模式代码段和数据段索引/段选择子 */
#define P_CODE_SELECTOR  P_SEG_DEF(1)
#define P_DATA_SELECTOR  P_SEG_DEF(2)

/*  **************************************************************** */

/* 64位代码段，默认非一致性，可读 */
#define E_CODE64_SEG_DESC_DEF(dpl) \
    E_CODE64_SEG_DESC(CODE_NO_ACCESSED, CODE_READABLE, CODE_NO_CONFORMING, dpl)
/* 64位数据段，默认可读写，向上扩展 */
#define E_DATA64_SEG_DESC_DEF(dpl)\
    E_DATA64_SEG_DESC(DATA_NO_ACCESSED, DATA_WRITEABLE, DATA_EXP_DIRECT_UP, dpl)
/* 32位代码段，默认非一致性，可读 */
#define E_CODE32_SEG_DESC_DEF(base, limit, dpl) \
    E_CODE32_SEG_DESC(base, limit, dpl, CODE_NO_ACCESSED, CODE_READABLE, CODE_NO_CONFORMING)
/* 32位数据段，默认可读写，向上扩展 */
#define E_DATA32_SEG_DESC_DEF(base, limit, dpl) \
    E_DATA32_SEG_DESC(base, limit, dpl, DATA_NO_ACCESSED, DATA_WRITEABLE, DATA_EXP_DIRECT_UP)

/* 64位long/ia-32e模式代码段和数据段索引/段选择子 */
#define E_CODE64_SELECTOR  P_SEG_DEF(1)
#define E_DATA64_SELECTOR  P_SEG_DEF(2)

/*  **************************************************************** */
/* 暂时以2MB为页大小，需要三级页表 */
#define E_PML4T_BASE  0x90000
#define E_PDPT_BASE   0x91000
#define E_PDT_BASE    0x92000
#define MAX_PHY_ADDR  36 //先按照最小的支持长度36，其它40/52需要读取CPUID查询

/* CR3默认配置为写回，cache使能 */
#define E_CR3_DEF(phy_addr) \
    E_CR3(((phy_addr)>>12), MAX_PHY_ADDR, P_PAGE_WRITE_BACK, P_PAGE_CACHE_ENABLE)

/* PML4E默认配置，可读写，用户模式，写回，cache使能，未访问，不支持执行禁止功能 */
#define E_PML4E_DEF(phy_addr) \
    E_PML4E(((phy_addr)>>12), MAX_PHY_ADDR, P_PAGE_READ_WRITE, P_PAGE_USER, P_PAGE_WRITE_BACK, \
        P_PAGE_CACHE_ENABLE, P_PAGE_ACCESSED_NO, E_PAGE_XD_DISABLE)

/* PDPTE默认配置，可读写，用户模式，写回，cache使能，未访问，不支持执行禁止功能 */
#define E_PDPTE_DEF(phy_addr) \
    E_PDPTE(((phy_addr)>>12), MAX_PHY_ADDR, P_PAGE_READ_WRITE, P_PAGE_USER, P_PAGE_WRITE_BACK, \
        P_PAGE_CACHE_ENABLE, P_PAGE_ACCESSED_NO, E_PAGE_XD_DISABLE)

/* PDE配置为2MB物理页，可读写，超级模式，写回，cache使能，未访问，干净页，局部，不支持执行禁止功能 */
#define E_PDE_2M_DEF(phy_addr) \
    E_PDE_2MB(((phy_addr)>>21), MAX_PHY_ADDR, P_PAGE_READ_WRITE, P_PAGE_SUPER, P_PAGE_WRITE_BACK, \
        P_PAGE_CACHE_ENABLE, P_PAGE_ACCESSED_NO, P_PAGE_NOT_DIRTYED, P_PAGE_LOCAL, P_PAGE_PAT0, E_PAGE_XD_DISABLE)

/*  **************************************************************** */

#define SMAP 0x534d4150

#endif
