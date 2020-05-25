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

static __noinline uint64_t read_rip(void)
{
    uint64_t rip;
    asm volatile("movq 8(%%rbp), %0" : "=r" (rip));
    return rip;
}

static inline void dump_stack(void)
{
    save_regs(&temp_regs[0]);
    dump_regs();
    uint64_t rbp = read_rbp();
    uint64_t rip = read_rip();
    int i;
    for (i = 0; (rbp != root_bp && rip != 0) && i < STACK_MAX_DEPTH; i++)
    {
        stab_func_info_s info = {0};
        printk("rbp: 0x%016lx, rip:0x%016lx\n", rbp, rip);
        if (0 == get_kfunc_loc(rip - 1, &info))
        {
            printk("\t%s[%s][%d] func 0x%016lx\n", info.func_file, info.func_name, 
                info.func_line, info.func_addr);
        }
        rip = ((uint64_t *)rbp)[1];
        rbp = ((uint64_t *)rbp)[0];
    }
}


#endif
