#ifndef __KDEBUG_H__
#define __KDEBUG_H__

#include <x86_64_reg.h>
#include <x86_64_asm.h>
#include <basetype.h>
#include <printk.h>

#define STACK_MAX_DEPTH 20

typedef enum {
    RIP = 0,
    RAX,
    RBX,
    RCX,
    RDX,
    RSI,
    RDI,
    RBP,
    RSP,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    MAX_REGS
}REGS_E;

#define REG_ARG1  RDI
#define REG_ARG2  RSI 
#define REG_ARG3  RDX 
#define REG_ARG4  RCX 
#define REG_ARG5  R8 
#define REG_ARG6  R9 
#define REG_LR    RAX

typedef struct {
    const char *func_name;
    const char *func_file;
    int func_name_len;
    int func_line;
    uint64_t func_addr;
    uint64_t func_off;
}stab_func_info_s;

extern const char *regs_string[MAX_REGS];
extern uint64_t temp_regs[MAX_REGS];
extern uint64_t root_bp;

void print_kernel_info(void);
void backtrace_init(void);
int get_kfunc_loc(uint64_t addr, stab_func_info_s *info);
void dump_regs(void);

static inline void dump_stack(void)
{
    //save_regs(&temp_regs[0]);
    asm volatile(
                "movq %%rax, 8(%%rbx)\n\r"
                "leaq (%%rip), %%rax\n\r"
                "movq %%rax, (%%rbx)\n\r"
                "movq %%rbx, 16(%%rbx)\n\r"
                "movq %%rcx, 24(%%rbx)\n\r"
                "movq %%rdx, 32(%%rbx)\n\r"
                "movq %%rsi, 40(%%rbx)\n\r"
                "movq %%rdi, 48(%%rbx)\n\r"
                "movq %%rbp, 56(%%rbx)\n\r"
                "movq %%rsp, 64(%%rbx)\n\r"
                "movq %%r8,  72(%%rbx)\n\r"
                "movq %%r9,  80(%%rbx)\n\r"
                "movq %%r10, 88(%%rbx)\n\r"
                "movq %%r11, 96(%%rbx)\n\r"
                "movq %%r12, 104(%%rbx)\n\r"
                "movq %%r13, 112(%%rbx)\n\r"
                "movq %%r14, 120(%%rbx)\n\r"
                "movq %%r15, 128(%%rbx)\n\r"
                : : "b" (temp_regs)
                : "memory");
    uint64_t rbp = temp_regs[RBP];
    uint64_t rip = temp_regs[RIP];
    uint64_t *args = (uint64_t *)(*(uint64_t *)rbp);
    temp_regs[RDI] = args[-1];
    temp_regs[RSI] = args[-2];
    temp_regs[RDX] = args[-3];
    temp_regs[RCX] = args[-4];
    temp_regs[R8]  = args[-5];
    temp_regs[R9]  = args[-6];
    dump_regs();
    int i;
    printk("Calltrace:\n");
    for (i = 0; (rbp != root_bp && rip != 0) && i < STACK_MAX_DEPTH; i++)
    {
        printk("  <0x%016lx>\n", rip);
        #if 0 //不支持-gstabs,编译有问题,目前无法解决
        stab_func_info_s info = {0};
        if (0 == get_kfunc_loc(rip - 1, &info))
        {
            printk("\t0x%016lx, %s\n", info.func_addr, info.func_name);
        }
        #endif
        rip = ((uint64_t *)rbp)[1];
        rbp = ((uint64_t *)rbp)[0];
    }
}


#endif
