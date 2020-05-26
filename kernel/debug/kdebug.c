#include <kdebug.h>
#include <basetype.h>
#include <printk.h>
#include <head_asm.h>
#include <x86_64_asm.h>
#include <elf.h>
#include <string.h>

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
    extern char _text[], _etext[], _rodata[], _erodata[], _stab[], _estab[],
        _data[], _edata[], _bss[], _ebss[], _end[], start_kernel[];
    uint64_t len;
    printk("Special kernel symbols:\n");
    printk("  entry    0x%016x (phys)\n", (uint64_t)start_kernel);
    len = _etext - _text;
    printk("  etext    0x%016x (phys), len (%ldG-%ldM-%ldK-%ldB)\n", 
        (uint64_t)_etext, GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len));
    len = _erodata - _rodata;
    printk("  erodata  0x%016x (phys), len (%ldG-%ldM-%ldK-%ldB)\n", 
        (uint64_t)_erodata, GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len));
    len = _estab - _stab;
    printk("  estab    0x%016x (phys), len (%ldG-%ldM-%ldK-%ldB)\n", 
        (uint64_t)_estab, GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len));
    len = _edata - _data;
    printk("  edata    0x%016x (phys), len (%ldG-%ldM-%ldK-%ldB)\n", 
        (uint64_t)_edata, GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len));
    len = _ebss - _bss;
    printk("  ebss     0x%016x (phys), len (%ldG-%ldM-%ldK-%ldB)\n", 
        (uint64_t)_ebss, GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len));
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

extern const Elf64_Stab __STAB_BEGIN__[];  // beginning of stabs table
extern const Elf64_Stab __STAB_END__[];    // end of stabs table
extern const char __STABSTR_BEGIN__[];      // beginning of string table
extern const char __STABSTR_END__[];        // end of string table

static void stab_binsearch(const Elf64_Stab *stabs, int *region_left, 
        int *region_right, int type, uint64_t addr) 
{
    int l = *region_left, r = *region_right, any_matches = 0;

    while (l <= r) {
        int true_m = (l + r) / 2, m = true_m;

        // search for earliest stab with right type
        while (m >= l && stabs[m].n_type != type) {
            m --;
        }
        if (m < l) {    // no match in [l, m]
            l = true_m + 1;
            continue;
        }

        // actual binary search
        any_matches = 1;
        if (stabs[m].n_value < addr) {
            *region_left = m;
            l = true_m + 1;
        } else if (stabs[m].n_value > addr) {
            *region_right = m - 1;
            r = m - 1;
        } else {
            // exact match for 'addr', but continue loop to find
            // *region_right
            *region_left = m;
            l = m;
            addr ++;
        }
    }

    if (!any_matches) {
        *region_right = *region_left - 1;
    }
    else {
        // find rightmost region containing 'addr'
        l = *region_right;
        for (; l > *region_left && stabs[l].n_type != type; l --)
            /* do nothing */;
        *region_left = l;
    }
}

int get_kfunc_loc(uint64_t addr, stab_func_info_s *info)
{
    const Elf64_Stab *stabs, *stab_end;
    const char *stabstr, *stabstr_end;

    stabs = __STAB_BEGIN__;
    stab_end = __STAB_END__;
    stabstr = __STABSTR_BEGIN__;
    stabstr_end = __STABSTR_END__;

    if (stabstr_end <= stabstr || stabstr_end[-1] != 0)
    {
        printk("[%s][%d]para error!\r\n", __FUNCTION__, __LINE__);
        return -1;
    }
#if 0
    // Now we find the right stabs that define the function containing
    // 'eip'.  First, we find the basic source file containing 'eip'.
    // Then, we look in that source file for the function.  Then we look
    // for the line number.

    // Search the entire set of stabs for the source file (type N_SO).
    int lfile = 0, rfile = (stab_end - stabs) - 1;
    stab_binsearch(stabs, &lfile, &rfile, N_SO, addr);
    if (lfile == 0)
    {
        printk("[%s][%d]N_SO not found!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    // Search within that file's stabs for the function definition
    // (N_FUN).
    int lfun = lfile, rfun = rfile;
    int lline, rline;
    stab_binsearch(stabs, &lfun, &rfun, N_FUN, addr);

    if (lfun <= rfun) {
        // stabs[lfun] points to the function name
        // in the string table, but check bounds just in case.
        if (stabs[lfun].n_strx < stabstr_end - stabstr) {
            info->func_name = stabstr + stabs[lfun].n_strx;
        }
        info->func_addr = stabs[lfun].n_value;
        addr -= info->func_addr;
        // Search within the function definition for the line number.
        lline = lfun;
        rline = rfun;
    } else {
        // Couldn't find function stab!  Maybe we're in an assembly
        // file.  Search the whole file for the line number.
        info->func_addr = addr;
        lline = lfile;
        rline = rfile;
    }
    info->func_name_len = strfind(info->func_name, ':') - info->func_name;

    // Search within [lline, rline] for the line number stab.
    // If found, set info->eip_line to the right line number.
    // If not found, return -1.
    stab_binsearch(stabs, &lline, &rline, N_SLINE, addr);
    if (lline <= rline) {
        info->func_line = stabs[rline].n_desc;
    } else 
    {
        printk("[%s][%d]N_SLINE not found!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    // Search backwards from the line number for the relevant filename stab.
    // We can't just use the "lfile" stab because inlined functions
    // can interpolate code from a different file!
    // Such included source files use the N_SOL stab type.
    while (lline >= lfile
           && stabs[lline].n_type != N_SOL
           && (stabs[lline].n_type != N_SO || !stabs[lline].n_value)) {
        lline --;
    }
    if (lline >= lfile && stabs[lline].n_strx < stabstr_end - stabstr) {
        info->func_file = stabstr + stabs[lline].n_strx;
    }
#else
    int lfun = 0, rfun = (stab_end - stabs) - 1;
    stab_binsearch(stabs, &lfun, &rfun, N_FUN, addr);

    if (lfun <= rfun) {
        // stabs[lfun] points to the function name
        // in the string table, but check bounds just in case.
        if (stabs[lfun].n_strx < stabstr_end - stabstr) {
            info->func_name = stabstr + stabs[lfun].n_strx;
        }
        info->func_addr = stabs[lfun].n_value;
        addr -= info->func_addr;
    } else {
        // Couldn't find function stab!  Maybe we're in an assembly
        // file.  Search the whole file for the line number.
        info->func_addr = addr;
    }
    info->func_name_len = strfind(info->func_name, ':') - info->func_name;
#endif
    return 0;

}


void backtrace_init(void)
{
    set_root_bp(KERNEL_BP_ADDR + KERNEL_VIR_START);
    dump_stack();
}


