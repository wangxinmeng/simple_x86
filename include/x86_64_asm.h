#ifndef __X86_64_ASM_H__
#define __X86_64_ASM_H__

#include <basetype.h>

static inline uint8_t inb(uint16_t port){
    uint8_t data;
    asm volatile(
        "inb %1, %0" 
        : "=a" (data)
        : "d" (port)
        : "memory");
    return data;
}

static inline uint16_t inw(uint16_t port){
    uint16_t data;
    asm volatile(
        "inw %1, %0" 
        : "=a" (data)
        : "d" (port)
        : "memory");
    return data;
}

static inline void insw(uint32_t port, void *addr, int cnt){
    asm volatile (
        ".insw_to_read:\n\t"
        "inw %%dx, %%ax\n\t"
        "movw %%ax, (%%rbx)\n\t"
        "addq $2, %%rbx\n\t"
        "loop .insw_to_read"
        : "=b" (addr), "=c" (cnt)
        : "d" (port), "b" (addr), "c" (cnt)
        : "memory", "cc");
}

static inline void insl(uint32_t port, void *addr, int cnt){
    asm volatile (
        "cld;"
        "repne; insl;"
        : "=D" (addr), "=c" (cnt)
        : "d" (port), "0" (addr), "1" (cnt)
        : "memory", "cc");
}

static inline void outb(uint16_t port, uint8_t data){
    asm volatile(
        "outb %0, %1"
        :: "a" (data), "d" (port)
        : "memory");
}

static inline void outw(uint16_t port, uint16_t data){
    asm volatile(
        "outw %0, %1"
        :: "a" (data), "d" (port)
        : "memory");
}

static inline void outsl(uint32_t port, const void *addr, int cnt){
    asm volatile (
        "cld;"
        "repne; outsl;"
        : "=S" (addr), "=c" (cnt)
        : "d" (port), "0" (addr), "1" (cnt)
        : "memory", "cc");
}

static inline uint32_t read_ebp(void) {
    uint32_t ebp;
    asm volatile ("movl %%ebp, %0" : "=r" (ebp));
    return ebp;
}

static inline void lidt(void *pd) {
    asm volatile ("lidt (%0)" :: "r" (pd) : "memory");
}

static inline void sti(void) {
    asm volatile ("sti");
}

static inline void cli(void) {
    asm volatile ("cli" ::: "memory");
}

static inline void ltr(uint16_t sel) {
    asm volatile ("ltr %0" :: "r" (sel) : "memory");
}

static inline uint32_t read_eflags(void) {
    uint32_t eflags;
    asm volatile ("pushfl; popl %0" : "=r" (eflags));
    return eflags;
}

static inline void write_eflags(uint32_t eflags) {
    asm volatile ("pushl %0; popfl" :: "r" (eflags));
}

static inline void lcr0(uint32_t cr0) {
    asm volatile ("mov %0, %%cr0" :: "r" (cr0) : "memory");
}

static inline void lcr3(uint32_t cr3) {
    asm volatile ("mov %0, %%cr3" :: "r" (cr3) : "memory");
}

static inline uint32_t rcr0(void) {
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r" (cr0) :: "memory");
    return cr0;
}

static inline uint32_t rcr1(void) {
    uint32_t cr1;
    asm volatile ("mov %%cr1, %0" : "=r" (cr1) :: "memory");
    return cr1;
}

static inline uint32_t rcr2(void) {
    uint32_t cr2;
    asm volatile ("mov %%cr2, %0" : "=r" (cr2) :: "memory");
    return cr2;
}

static inline uint32_t rcr3(void) {
    uint32_t cr3;
    asm volatile ("mov %%cr3, %0" : "=r" (cr3) :: "memory");
    return cr3;
}

/* REP NOP (PAUSE) is a good thing to insert into busy-wait loops. */
static inline void rep_nop(void)
{
    asm volatile ("rep;nop": : :"memory");
}

#define cpu_relax()     rep_nop()

static inline uint64_t read_rbp(void) {
    uint64_t rbp;
    asm volatile ("movq %%rbp, %0" : "=r" (rbp));
    return rbp;
}

static inline void save_regs(uint64_t *arg)
{
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
                : : "b" (arg)
                : "memory");
}

static inline void read_args(uint64_t *arg)
{
    asm volatile("movq %%rdi, (%%rbx)\n\r"
                "movq %%rsi, 8(%%rbx)\n\r"
                "movq %%rdx, 16(%%rbx)\n\r"
                "movq %%rcx, 24(%%rbx)\n\r"
                "movq %%r8, 32(%%rbx)\n\r"
                "movq %%r9, 40(%%rbx)\n\r"
                : : "b" (arg)
                : "memory");
}

#endif
