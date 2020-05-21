#include <kdebug.h>
#include <basetype.h>
#include <printk.h>
#include <head_asm.h>
#include <x86_64_asm.h>

uint64_t root_bp = 0;
uint64_t temp_regs[MAX_REGS] = {0};
const char *regs_string[MAX_REGS] = {
    "RIP",
    "RAX",
    "RBX",
    "RCX",
    "RDX",
    "RSI",
    "RDI",
    "RBP",
    "RSP",
    "R8 ",
    "R9 ",
    "R10",
    "R11",
    "R12",
    "R13",
    "R14",
    "R15"
};

void dump_regs(void)
{
    int i = 0;
    printk("dump_stack, regs:\n");
    printk("    %s: 0x%016lx\n", regs_string[0],   temp_regs[0]);
    for (i = 1; i < MAX_REGS; i += 2)
    {
        printk("    %s: 0x%016lx,  %s: 0x%016lx\n",
            regs_string[i],   temp_regs[i], 
            regs_string[i+1], temp_regs[i+1]);
    }
    printk("  Arg1 -> %s, Arg2 -> %s, Arg3 -> %s, Arg4 -> %s\n",
        regs_string[REG_ARG1], regs_string[REG_ARG2], 
        regs_string[REG_ARG3], regs_string[REG_ARG4]);
    printk("  Arg5 -> %s, Arg6 -> %s, LR   -> %s\n",
        regs_string[REG_ARG5], regs_string[REG_ARG6], 
        regs_string[REG_LR]);
    return;
}


void print_kernel_info(void)
{
    extern char __etext[], __edata[], __erodata[], __ebss[], _end[], start_kernel[];
    uint64_t len;
    printk("Special kernel symbols:\n");
    printk("  entry    0x%016x (phys)\n", (uint64_t)start_kernel);
    len = __etext - start_kernel;
    printk("  etext    0x%016x (phys), len (%ldG-%ldM-%ldK-%ldB)\n", 
        (uint64_t)__etext, GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len));
    len = __edata - __etext;
    printk("  edata    0x%016x (phys), len (%ldG-%ldM-%ldK-%ldB)\n", 
        (uint64_t)__edata, GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len));
    len = __erodata - __edata;
    printk("  erodata  0x%016x (phys), len (%ldG-%ldM-%ldK-%ldB)\n", 
        (uint64_t)__erodata, GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len));
    len = __ebss - __erodata;
    printk("  ebss     0x%016x (phys), len (%ldG-%ldM-%ldK-%ldB)\n", 
        (uint64_t)__ebss, GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len));
    printk("  end      0x%016x (phys)\n", (uint64_t)_end);
    len = _end - start_kernel;
    printk("Kernel executable memory footprint: (%ldG-%ldM-%ldK-%ldB)\n", 
        GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len));
    printk("Kernel virual memory offset : 0x%lx\n", (uint64_t)KERNEL_VIR_START);
}


void set_root_bp(uint64_t bp)
{
    root_bp = bp;
}

void print_debuginfo(uint64_t rip)
{
    
}


void backtrace_init(void)
{
    set_root_bp(KERNEL_BP_ADDR + KERNEL_VIR_START);
    dump_stack();
}


