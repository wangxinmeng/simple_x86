#ifndef __ASM_BASETYPE_H__
#define __ASM_BASETYPE_H__

#define ASM_BIT(x)  (1 << (x))
#ifdef ASM_C_DEBUG
/* 默认扩展到8字节，64bit */
#define B_SEL(value, bit_start, bit_space, bit_offset) \
        (((((unsigned long long)(value))>>(bit_start))&((ASM_BIT(bit_space))-1))<<(bit_offset))
/* 操作数为4字节宽度，32bit */
#define B_SEL4(value, bit_start, bit_space, bit_offset) \
        ((((value)>>(bit_start))&((ASM_BIT(bit_space))-1))<<(bit_offset))
#else
#define B_SEL(value, bit_start, bit_space, bit_offset) \
        ((((value)>>(bit_start))&((ASM_BIT(bit_space))-1))<<(bit_offset))
#define B_SEL4(value, bit_start, bit_space, bit_offset) B_SEL(value, bit_start, bit_space, bit_offset)
#endif
#define ASM_MB(x) ((x) << 20)

#define SYMBOL_NAME(X)       X
#define SYMBOL_NAME_LABEL(X) X##:

#define ENTRY(name) \
.global SYMBOL_NAME(name);  \
SYMBOL_NAME_LABEL(name)

#endif
