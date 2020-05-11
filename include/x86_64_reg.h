#ifndef __X86_64_REG_H__
#define __X86_64_REG_H__
#ifndef BIT
#define BIT(x) (1<<(x))
#endif
#define CR0_PE_ON   BIT(0)
#define CR0_PG_ON   BIT(31)
#define CR4_PAE_ON  BIT(5)

#define IA32_EFER   0xC0000080
#define EFER_LME    BIT(8)

#endif
