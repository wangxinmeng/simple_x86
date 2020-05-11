
#include <asm_basetype.h>
#include <x86_64_reg.h>
#include <x86_64_segment.h>
#include <x86_64_page.h>
#include "loader_asm.h"

/* BIOS原始内存分布 
0x00000 ~ 0x9FFFF: 基本内存（640KB）
    0x00000 ~ 0x003FF: 中断向量表（1024B）
    0x00400 ~ 0x004FF: bios数据区（256B）
    0x00500 ~ 0x07BFF: 自由内存区
    0x07C00 ~ 0x07DFF: 引导程序加载区（512B）
    0x07E00 ~ 0x9FFFF: 自由内存区
0xA0000 ~ 0xBFFFF: 显存（128KB）
    0xA0000 ~ 0xAFFFF: EGA/VGA/XGA/XVGA图形视频缓冲区（64KB）
    0xB0000 ~ 0xB7FFF: Mono text video buffer（32KB）
    0xB8000 ~ 0xBFFFF: CGA/EGA+ chroma text video buffer（32KB）
0xC0000 ~ 0xFFFFF: bios自己使用（256KB）
    0xC0000 ~ 0xC7FFFF: 显卡bios使用（32KB）
    0xC8000 ~ 0xCBFFFF: ide控制器bios使用（16KB）
    0xCC000 ~ 0xEFFFFF: 
    0xF0000 ~ 0xFFFFFF: 系统bios使用（64KB）
*/

/*  当前规划，主要利用以下2个自由内存区：
    第1部分：0x00500 ~ 0x07BFF: 自由内存区(0x7700即30,464B, 29.75KB)
    第2部分：0x07E00 ~ 0x9FFFF: 自由内存区(0x98200即623,104B, 608.5KB)

    LOADER阶段，可以回收BOOT的MBR区，作为其它用途，    即2个部分可以合一使用：
    LOADER阶段内存规划如下：
        0x00500 ~ 0x02FFF 预留， 11008B，10.75KB
        0x03000 ~ 0x06FFF loader, 16384B, 16KB
        0x07000 ~ 0x07FF8 栈， 4088B
        0x08000 ~ 0x083FF 物理内存信息存储区域，1KB
        0x08400 ~ 0x08FFF 预留，31KB
        0x10000 ~ 0x8FFFF 预留，64KB*8=512KB
        0x90000 ~ 0x92FFF 页表使用，12KB
        0x93000 ~ 0x9FFFF 预留，52KB
*/


// 开始地址在0x03000，实模式
.globl loader_start
loader_start:
.code16                                             # Assemble for 16-bit mode
real_mode:
    cli                                             # Disable interrupts
    cld                                             # String operations increment

    # Set up the important data segment registers (DS, ES, SS).
    movw %cs, %ax
    movw %ax, %ds                                   # -> Data Segment
    movw %ax, %es                                   # -> Extra Segment
    movw %ax, %ss                                   # -> Stack Segment

// 开启A20
seta20.1:
    inb $0x64, %al                                  # Wait for not busy
    testb $0x2, %al
    jnz seta20.1

    movb $0xd1, %al                                 # 0xd1 -> port 0x64
    outb %al, $0x64

seta20.2:
    inb $0x64, %al                                  # Wait for not busy
    testb $0x2, %al
    jnz seta20.2

    movb $0xdf, %al                                 # 0xdf -> port 0x60
    outb %al, $0x60

probe_memory:
    movl $(MEM_ADDR), %eax
    movl $0, (%eax)
    xorl %ebx, %ebx
    movw $(MEM_ADDR+4), %di
start_probe:
    movl $0xE820, %eax
    movl $20, %ecx
    movl $(SMAP), %edx
    int $0x15
    jnc cont
    movl $(MEM_ADDR), %eax
    movl $12345, (%eax)
    jmp finish_probe
cont:
    addw $20, %di
    movl $(MEM_ADDR), %eax
    incl (%eax)
    cmpl $0, %ebx
    jnz start_probe
finish_probe:

// 加载gdt，使能CR0.PE位，进入保护模式
    lgdt p_gdt_pointer
    movl %cr0, %eax
    orl $(CR0_PE_ON), %eax
    movl %eax, %cr0
    
    ljmp $(P_CODE_SELECTOR), $protect_mode

.code32
protect_mode:
// 把剩余所有段寄存器都指向数据段描述符
    movw $(P_DATA_SELECTOR), %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

// 检测是否支持long模式
is_support_long_mode:
    movl $0x80000000, %eax
    cpuid
    cmp $0x80000001, %eax
    ja prepare_page_table
p_spin:
    jmp p_spin

// 准备一个临时页表，页表基地址定为0x90000，二级页表基地址为0x91000，三级页表基地址为0x92000
prepare_page_table:
/* 把页目录加载到cr3 */
    movl $(E_PML4T_BASE), %eax
    movl %eax, %cr3
/* 设置页表项 */
    movl $(E_PML4E_DEF(E_PDPT_BASE)), 0(%eax)
    movl $(E_PDPT_BASE), %eax
    movl $(E_PDPTE_DEF(E_PDT_BASE)), 0(%eax)
    movl $(E_PDT_BASE), %eax
    movl $(E_PDE_2M_DEF(ASM_MB(0))), 0(%eax)
    movl $0, 4(%eax)
    movl $(E_PDE_2M_DEF(ASM_MB(2))), 8(%eax)
    movl $0, 12(%eax)
    movl $(E_PDE_2M_DEF(ASM_MB(4))), 16(%eax)
    movl $0, 20(%eax)
    movl $(E_PDE_2M_DEF(ASM_MB(6))), 24(%eax)
    movl $0, 28(%eax)
    movl $(E_PDE_2M_DEF(ASM_MB(8))), 32(%eax)
    movl $0, 36(%eax)
    
/* 加载64位的段描述符表 */
    lgdt e_gdt_pointer

/* 使能CR4的PAE位 */
    movl %cr4, %eax
    orl $(CR4_PAE_ON), %eax
    movl %eax, %cr4

/* 设置EFER */
    movl $(IA32_EFER), %ecx
    rdmsr
    orl $(EFER_LME), %eax
    wrmsr
enable_ia32_mode:
/* 使能分页机制，激活IA-32e模式 */
    movl %cr0, %eax
    orl $(CR0_PG_ON), %eax
    movl %eax, %cr0 
    ljmpl $(E_CODE64_SELECTOR), $long_mode

.code64
long_mode:
    //movq $(E_PDE_2M_DEF(TB(255))), %rbx //目前宏在汇编里无法扩展到64位，只能扩展到32位
    
    /* 其余段都设置为数据段 */
    movw $(E_DATA64_SELECTOR), %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw %ax, %fs
    movw %ax, %gs
    
    /* 寄存器清零 */
    movq $0,   %rax
    movq %rax, %rbx
    movq %rax, %rcx
    movq %rax, %rdx
    movq %rax, %rsi
    movq %rax, %rdi

set_loader_stack:
    movq $0x0, %rbp
    movq $(MEM_ADDR-8), %rsp

    //call kernel_loader
    movq switch_c_loader(%rip), %rax
    pushq $(E_CODE64_SELECTOR)
    pushq %rax
    lretq

// never back here
spin:
    jmp spin

switch_c_loader:
    .quad kernel_loader

# 32位保护模式GDT
.align 8
p_gdt_table:
    .quad P_SEG_DESC_NULL
    .quad P_CODE_SEG_DESC_DEF(0x0, 0xffffffff)
    .quad P_DATA_SEG_DESC_DEF(0x0, 0xffffffff)
p_gdt_end:

p_gdt_pointer:
p_gdt_limit: .word p_gdt_end - p_gdt_table - 1
p_gdt_base:  .long p_gdt_table

# 64位IA-32e模式GDT
.align 8
e_gdt_table:
    .quad E_SEG_DESC_NULL
    .quad E_CODE64_SEG_DESC_DEF(DPL(0))
    .quad E_DATA64_SEG_DESC_DEF(DPL(0))
e_gdt_end:

e_gdt_pointer:
e_gdt_limit: .word e_gdt_end - e_gdt_table - 1
e_gdt_base:  .long e_gdt_table

