#ifndef __X86_64_SEGMENT_H__
#define __X86_64_SEGMENT_H__

/*  **************************************************************** */

/* 段选择子 
    index   段选择子索引，最大2^13=8096
    ti      描述符表类型，0=gdt,1=ldt
    rpl     请求特权级0~3
*/
#define RPL(x)      (x)
#define TI_GDT      0
#define TI_LDT      1
#define P_SEG(index, ti, rpl) \
    ((B_SEL4(rpl, 0, 2, 0)) | (B_SEL4(ti, 0, 1, 2)) | (B_SEL4(index, 0, 13, 3)))

/* 默认使用GDT，RPL权限为0 */
#define P_SEG_DEF(index)  P_SEG(index, TI_GDT, RPL(0))


/*  **************************************************************** */

/* 保护模式段描述符构造 */
/*
    type    包含S位，区分系统段、代码段、数据段
    base    段基地址
    limit   段限长，g=1时，颗粒度为4K~4G，g=0时，颗粒度为1B~1M
    dpl     特权级0~3
    p       1代表段已在内存中，设置段描述符时，必须置1
    avl     被系统软件使用，设置为0即可
    l       保护模式下，保留，设置为0即可
    db      用于标识代码段的位宽，或者栈段的操作数位宽及上边界
            （32位代码/数据段应为1,16位代码/数据段应为0）
    g       段限长的颗粒度，g=1代表以4KB为颗粒度，g=0代表以字节为颗粒度
*/

//保护模式段描述符定义，总共占8B
#define P_SEG_DESC_NULL 0

#define P_SEG_DESC(base, limit, type, s, dpl, p, avl, l, db, g) \
    ((B_SEL(limit,0,16,0)) | (B_SEL(base,0,24,16))   \
    |((B_SEL(type,0,4,40))|(B_SEL(s,0,1,44))|(B_SEL(dpl,0,2,45))|(B_SEL(p,0,1,47))) \
    |((B_SEL(limit,16,4,48))|(B_SEL(avl,0,1,52))|(B_SEL(l,0,1,53))|(B_SEL(db,0,1,54))|(B_SEL(g,0,1,55))) \
    |(B_SEL(base,24,8,56)))

/*  以下默认以dpl=0, 颗粒度为4K即g=1，l=0, avl=0, p=0为基本配置
    只需配置type, base, limit即可
*/
#define DPL(x)          ((x)&0x3)
#define G_4K            1
#define AVL_DEF         0
#define PRE_DEF         1 //默认段描述符在内存中，否则异常
#define L_DEF           0
#define DB_32           1
#define DB_16           0

#define S_TYPE_CODE  1
#define S_TYPE_DATA  1
#define S_TYPE_SYS   0
//默认配置为32位，4KB内存颗粒度
#define P_SEG_DESC_DEF(base, limit, s, type, dpl) \
    P_SEG_DESC(base, limit, type, s, dpl, PRE_DEF, AVL_DEF, L_DEF, DB_32, G_4K)

#define TYPE_H_CODE  1
#define TYPE_H_DATA  0

#define CODE_CONFORMING         1
#define CODE_NO_CONFORMING      0
#define CODE_READABLE           1
#define CODE_UNREADABLE         0
#define CODE_ACCESSED           1
#define CODE_NO_ACCESSED        0
/* 32位代码段 */
#define P_CODE_SEG_DESC(base, limit, dpl, accessed, readable, conforming) \
    P_SEG_DESC_DEF(base, limit, S_TYPE_CODE, \
        (B_SEL(accessed, 0, 1, 0)|B_SEL(readable, 0, 1, 1) \
        |B_SEL(conforming, 0, 1, 2)|B_SEL(TYPE_H_CODE, 0, 1, 3)), dpl)

#define DATA_EXP_DIRECT_UP      0
#define DATA_EXP_DIRECT_DOWN    1
#define DATA_WRITEABLE          1
#define DATA_UNWRITEABLE        0
#define DATA_ACCESSED           1
#define DATA_NO_ACCESSED        0
/* 32位数据段 */
#define P_DATA_SEG_DESC(base, limit, dpl, accessed, writeable, exp_direct) \
    P_SEG_DESC_DEF(base,limit,S_TYPE_DATA, \
        (B_SEL(accessed,0,1,0)|B_SEL(writeable,0,1,1) \
        |B_SEL(exp_direct,0,1,2)|B_SEL(TYPE_H_DATA,0,1,3)),dpl)

/*  **************************************************************** */

/* IA-32e模式，64位时，忽略base/limit */
/* 代码/数据段仍为8B，但系统段为16B */
#define E_IGNORED  0
#define E_CODE_DATA_SEG_DESC(type, s, dpl, p, l, db) \
    P_SEG_DESC(E_IGNORED, E_IGNORED, type, s, dpl, p, E_IGNORED, l, db, E_IGNORED)

#define E_SEG_DESC_NULL \
    P_SEG_DESC_NULL

/* 64位代码段 */
#define E_CODE64_SEG_DESC(accessed, readable, conforming, dpl) \
    E_CODE_DATA_SEG_DESC( \
        (B_SEL(accessed,0,1,0)|B_SEL(readable,0,1,1) \
        |B_SEL(conforming,0,1,2)|B_SEL(TYPE_H_CODE,0,1,3)), \
        S_TYPE_CODE, dpl, PRE_DEF, 1, 0)
/* 64位数据段 */
#define E_DATA64_SEG_DESC(accessed, writeable, exp_direct, dpl) \
    E_CODE_DATA_SEG_DESC( \
        (B_SEL(accessed,0,1,0)|B_SEL(writeable,0,1,1) \
        |B_SEL(exp_direct,0,1,2)|B_SEL(TYPE_H_DATA,0,1,3)), \
        S_TYPE_DATA, dpl, PRE_DEF, E_IGNORED, E_IGNORED)

/* 32位代码段沿用保护模式配置 */
#define E_CODE32_SEG_DESC(base, limit, dpl, accessed, readable, conforming) \
    P_CODE_SEG_DESC(base, limit, dpl, accessed, readable, conforming)

/* 32位数据段沿用保护模式配置 */
#define E_DATA32_SEG_DESC(base, limit, dpl, accessed, writeable, exp_direct) \
    P_DATA_SEG_DESC(base, limit, dpl, accessed, writeable, exp_direct)

/* 系统段描述符为16B */
#define E_SYS_TYPE_LDT      0X2
#define E_SYS_TYPE_TSS      0x9
#define E_SYS_TYPE_TSS_USED 0xB
#define E_SYS_TYPE_CALL     0xC
#define E_SYS_TYPE_INT      0xE
#define E_SYS_TYPE_TRAP     0xF

/* 低8B */
#define E_SYS_SEG_DESC(type, base, limit, dpl, p, avl) \
    P_SEG_DESC(base, limit, type, S_TYPE_SYS, dpl, p, avl, E_IGNORED, E_IGNORED, E_IGNORED) 

/* 高8B */
#define E_SYS_SEG_DESC_HIGH(base) \
    (B_SEL(base, 32, 32, 0))


#define E_LDT_SEG_DESC(base, limit, dpl, p, avl) \
    E_SYS_SEG_DESC(E_SYS_TYPE_LDT, base, limit, dpl, p, avl)

#define E_TSS_SEG_DESC(base, limit, dpl, p, avl) \
    E_SYS_SEG_DESC(E_SYS_TYPE_TSS, base, limit, dpl, p, avl)
#define E_TSS_USED_SEG_DESC(base, limit, dpl, p, avl) \
    E_SYS_SEG_DESC(E_SYS_TYPE_TSS_USED, base, limit, dpl, p, avl)

/* 调用门、中断、异常段描述符格式基本一致 */
#define E_SELECTOR_SEG_DESC(type, selector, offset, dpl, p, byte4) \
    ((B_SEL(offset,0,16,0))|(B_SEL(selector,0,16,16)) \
    |(B_SEL(byte4,0,8,32))|(B_SEL(type,0,4,40))|(B_SEL(S_TYPE_SYS,0,1,44))|(B_SEL(dpl,0,2,45))|(B_SEL(p,0,1,47)) \
    |(B_SEL(offset,16,16,48)))

#define E_CALL_SEG_DESC(selector, offset, dpl, p) \
    E_SELECTOR_SEG_DESC(E_SYS_TYPE_CALL, selector, offset, dpl, p, E_IGNORED)

#define E_INT_SEG_DESC(selector, offset, dpl, p, ist) \
    E_SELECTOR_SEG_DESC(E_SYS_TYPE_INT, selector, offset, dpl, p, ist)

#define E_TRAP_SEG_DESC(selector, offset, dpl, p, ist) \
    E_SELECTOR_SEG_DESC(E_SYS_TYPE_TRAP, selector, offset, dpl, p, ist)

#endif
