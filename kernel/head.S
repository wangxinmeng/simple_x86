
#include <asm_basetype.h>
#include <x86_64_reg.h>
#include <x86_64_segment.h>
#include <x86_64_page.h>
#include "head_asm.h"

.section .text
.global start_kernel
.code64
ENTRY(_start)
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %ss
    movl $(KERNEL_STACK_ADDR), %esp

//reload gdt
    lgdt GDT_POINTER(%rip)
//load idt
    lidt IDT_POINTER(%rip)
#if 0
    movw $(K_DATA64_SELECTOR),  %ax
    movw %ax,    %ds
    movw %ax,    %es
    movw %ax,    %fs
    movw %ax,    %gs
    movw %ax,    %ss
#endif
    //movq    _stack_start(%rip), %rsp
//reload page table
    movq $(KERNEL_REAL_ADDR + PML4T_BASE_ORI_OFFSET), %rax
    movq %rax, %cr3 

//jump C main
    movq $(KERNEL_VIR_START + KERNEL_STACK_ADDR), %rsp
    movq $(KERNEL_VIR_START), %rbp
    movq go_to_kernel(%rip), %rax
    pushq $0x08
    pushq %rax
    lretq

go_to_kernel:
    .quad start_kernel

/=======    init page
.align 8

.org    PML4T_BASE_ORI_OFFSET

__PML4E:
    .quad   PML4E_DEF(KERNEL_REAL_ADDR + PDPT_BASE_ORI_OFFSET)
    .fill   255,8,0
    .quad   PML4E_DEF(KERNEL_REAL_ADDR + PDPT_BASE_ORI_OFFSET)
    .fill   255,8,0

.org    PDPT_BASE_ORI_OFFSET

__PDPTE:
    
    .quad   PDPTE_DEF(KERNEL_REAL_ADDR + PDT_BASE_ORI_OFFSET)    /* 0x103003 */
    .fill   511,8,0

.org    PDT_BASE_ORI_OFFSET

__PDE:
    .quad   PDE_2M_DEF(MB(0))
    .quad   PDE_2M_DEF(MB(2))
    .quad   PDE_2M_DEF(MB(4))
    .quad   PDE_2M_DEF(MB(6))
    .quad   PDE_2M_DEF(MB(8))        /* 0x800083 */
    .quad   PDE_2M_DEF(MB(0xa))
    .quad   PDE_2M_DEF(MB(0xc))
    .quad   PDE_2M_DEF(MB(0xe))
    .quad   PDE_2M_DEF(MB(0x10))
    .quad   PDE_2M_DEF(MB(0x12))
    .quad   PDE_2M_DEF(MB(0x14))
    .quad   PDE_2M_DEF(MB(0x16))
    .quad   PDE_2M_DEF(MB(0x18))
    .quad   PDE_2M_DEF(MB(0x1a))
    .quad   PDE_2M_DEF(MB(0x1c))
    .quad   PDE_2M_DEF(MB(0x1e))
    .quad   PDE_2M_DEF(MB(0x20))
    .quad   PDE_2M_DEF(MB(0x22))
    .quad   PDE_2M_DEF(MB(0x24))
    .quad   PDE_2M_DEF(MB(0x26))
    .quad   PDE_2M_DEF(MB(0x28))
    .quad   PDE_2M_DEF(MB(0x2a))
    .quad   PDE_2M_DEF(MB(0x2c))
    .quad   PDE_2M_DEF(MB(0x2e))

    .quad   PDE_2M_DEF(MB(0xe00))      /*0x 3000000*/
    .quad   PDE_2M_DEF(MB(0xe02))
    .quad   PDE_2M_DEF(MB(0xe04))
    .quad   PDE_2M_DEF(MB(0xe06))
    .quad   PDE_2M_DEF(MB(0xe08))
    .quad   PDE_2M_DEF(MB(0xe0a))
    .quad   PDE_2M_DEF(MB(0xe0c))
    .quad   PDE_2M_DEF(MB(0xe0e))
    .fill   480,8,0

//=======   GDT_Table

.section .data

.globl GDT_Table

GDT_Table:
    .quad   E_SEG_DESC_NULL              /*0 NULL descriptor             00*/
    .quad   CODE64_SEG_DESC_DEF(DPL(0))  /*1 KERNEL  Code    64-bit  Segment 08*/
    .quad   DATA64_SEG_DESC_DEF(DPL(0))  /*2 KERNEL  Data    64-bit  Segment 10*/
    .quad   E_SEG_DESC_NULL              /*3 USER    Code    32-bit  Segment 18*/
    .quad   E_SEG_DESC_NULL              /*4 USER    Data    32-bit  Segment 20*/
    .quad   CODE64_SEG_DESC_DEF(DPL(3))  /*5 USER    Code    64-bit  Segment 28*/
    .quad   DATA64_SEG_DESC_DEF(DPL(3))  /*6 USER    Data    64-bit  Segment 30*/
    .quad   CODE32_SEG_DESC_DEF(DPL(0))  /*7 KERNEL  Code    32-bit  Segment 38*/
    .quad   DATA32_SEG_DESC_DEF(DPL(0))  /*8 KERNEL  Data    32-bit  Segment 40*/
    .fill   100,8,0                 /*10 ~ 11 TSS (jmp one segment <9>) in long-mode 128-bit 50*/
GDT_END:

GDT_POINTER:
GDT_LIMIT:  .word   GDT_END - GDT_Table - 1
GDT_BASE:   .quad   GDT_Table

//=======   IDT_Table

.globl IDT_Table

IDT_Table:
    .fill  512,8,0
IDT_END:

IDT_POINTER:
IDT_LIMIT:  .word   IDT_END - IDT_Table - 1
IDT_BASE:   .quad   IDT_Table


