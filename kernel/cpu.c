#include <cpu.h>
#include <head_asm.h>
#include <printk.h>

#define BIOS_MEM_ADDR (0x8000 + KERNEL_VIR_START)

BIOS_MEM_S bios_mem;
void bios_mem_init(void)
{
    int i;
    bios_mem = *(BIOS_MEM_S *)BIOS_MEM_ADDR;
    printk("sizeof(BIOS_MEM_S) is %d\n", sizeof(BIOS_MEM_S));
    printk("sizeof(BIOS_MEM_ZONE_S) is %d\n", sizeof(BIOS_MEM_ZONE_S));
    printk("bios mem zone num is %d\n", bios_mem.zone_num);
    for (i = 0; i < bios_mem.zone_num; i++)
    {
        uint64_t addr, len;
        uint32_t type;
        addr = bios_mem.zone[i].base_addr.u64;
        len  = bios_mem.zone[i].length.u64;
        type = bios_mem.zone[i].type;
        printk("zone %02d: ", i);
        printk("base addr: 0x%016lx  ", addr);
        printk("length: 0x%016lx  ", len);
        printk("type: %d\n", type);
        printk("    (%ldG-%ldM-%ldK-%ldB) -> (%ldG-%ldM-%ldK-%ldB), "
            "len (%ldG-%ldM-%ldK-%ldB), type %s\n", 
            GET_GB(addr), GET_MB(addr), GET_KB(addr), GET_B(addr),
            GET_GB(addr+len), GET_MB(addr+len), GET_KB(addr+len), GET_B(addr+len),
            GET_GB(len), GET_MB(len), GET_KB(len), GET_B(len),
            (type == 1)?"Available":"Reserved");
    }
}


CPUID_INFO_S cpuid_info;

void cpuid_get_max_func(void)
{
    uint32_t a, b, c, d, *p;
    get_cpuid(0, 0, &a, &b, &c, &d);
    cpuid_info.basic_max_func = a;
    p =  (uint32_t *)&(cpuid_info.vendor_name[0]);
    *p++ = b;
    *p++ = d;
    *p = c;
    get_cpuid(0x80000000, 0, &a, &b, &c, &d);
    cpuid_info.extended_max_func = a;
    printk("basic_max_func: 0x%x, extended_max_func: 0x%x\n", 
        cpuid_info.basic_max_func, cpuid_info.extended_max_func);
    printk("vendorname: %s\n", cpuid_info.vendor_name);
}

void cpuid_get_famliy_model(void)
{
    uint32_t a, b, c, d;
    get_cpuid(1, 0, &a, &b, &c, &d);
    cpuid_info.display_family = B_SEL(a, 8, 4, 0);
    cpuid_info.display_model  = B_SEL(a, 4, 4, 0);
    printk("display_family: 0x%x, display_model: 0x%x\n", 
        cpuid_info.display_family, cpuid_info.display_model);
#if 0
    cpuid_info.display_family += (a >> 20) & 0xff;
    cpuid_info.display_model  += (a >> 16) & 0xf;
    printk("display_family: 0x%x, display_model: 0x%x\n", 
        cpuid_info.display_family, cpuid_info.display_model);
#endif
}

void cpuid_get_max_phy_addr(void)
{
    uint32_t a, b, c, d;
    get_cpuid(0x80000008, 0, &a, &b, &c, &d);
    cpuid_info.max_phy_addr    = B_SEL(a, 0, 8, 0);
    cpuid_info.max_linear_addr = B_SEL(a, 8, 8, 0);
    printk("max_phy_addr: %d, max_linear_addr: %d\n", 
        cpuid_info.max_phy_addr, cpuid_info.max_linear_addr);
}

void cpuid_get_cache_info(void)
{
    uint32_t a, b, c, d, sub, total;
    CACHE_INFO_S info;
    for(sub = 0;;sub++)
    {
        get_cpuid(4, sub, &a, &b, &c, &d);
        info.type = a & 0x1f;
        if (!info.type)
        {
            break;
        }
        info.level          = B_SEL(a,  5,  3, 0);
        cpuid_info.core_num = B_SEL(a, 26, 10, 0) + 1;
        info.line_size      = B_SEL(b,  0, 12, 0) + 1;
        info.line_partition = B_SEL(b, 12, 10, 0) + 1;
        info.way            = B_SEL(b, 22, 10, 0) + 1;
        info.set            = c + 1;
        total = info.line_size * info.line_partition * info.way * info.set / KB(1);
        printk("cache level %d, type %d, line size %d, partition %d, "
            "way %d, set %d, total %dKB\n",
            info.level, info.type, info.line_size, info.line_partition, 
            info.way, info.set, total);
        switch(info.level)
        {
            case 1:
                if (info.type == 1)
                {
                    cpuid_info.cache_info[CACHE_L1D] = info;
                }
                else
                {
                    cpuid_info.cache_info[CACHE_L1I] = info;
                }
                break;
            case 2:
                cpuid_info.cache_info[CACHE_L2] = info;
                break;
            case 3:
                cpuid_info.cache_info[CACHE_L3] = info;
                break;
            default:
                break;
        }
    }
    printk("core num %d\n", cpuid_info.core_num);
}

void cpuid_info_init(void)
{
    cpuid_get_max_func();
    cpuid_get_famliy_model();
    cpuid_get_max_phy_addr();
    cpuid_get_cache_info();
}

void cpu_init(void)
{
    bios_mem_init();
    cpuid_info_init();
}

