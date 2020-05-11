#include <kdebug.h>
#include <basetype.h>
#include <printk.h>
#include <head_asm.h>
#include <x86_64_asm.h>


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


uint64_t root_bp;
void set_root_bp(uint64_t bp)
{
    root_bp = bp;
}

void print_debuginfo(uint64_t rip)
{
    
}

static __noinline uint64_t read_rip(void)
{
    uint64_t rip;
    asm volatile("movq 8(%%rbp), %0" : "=r" (rip));
    return rip;
}

void dump_stack(void)
{
    uint64_t rbp = read_rbp();
    uint64_t rip = read_rip();
    int i;
    uint64_t args[6];
    read_args(args);
    printk("args:\n");
    printk("    arg1(rdi): 0x%016lx, arg2(rsi): 0x%016lx\n", args[0], args[1]);
    printk("    arg3(rdx): 0x%016lx, arg4(rcx): 0x%016lx\n", args[2], args[3]);
    printk("    arg5(r8) : 0x%016lx, arg6(r9) : 0x%016lx\n", args[4], args[5]);
    for (i = 0; (rbp != root_bp && rip != 0) && i < STACK_MAX_DEPTH; i++)
    {
        printk("rbp: 0x%016lx, rip:0x%016lx\n", rbp, rip);
        print_debuginfo(rip - 1);
        rip = ((uint64_t *)rbp)[1];
        rbp = ((uint64_t *)rbp)[0];
    }
}

void backtrace_init(void)
{
    set_root_bp(KERNEL_BP_ADDR + KERNEL_VIR_START);
    dump_stack();
}


