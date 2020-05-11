#include <asm_basetype.h>
#include <basetype.h>
#include <elf.h>
#include <x86_64_asm.h>
//#include <layout.h>

//#define SECTOR_SIZE     512

/* waitdisk - wait for disk ready */
static void waitdisk(void) {
    while ((inb(0x1F7) & 0xC0) != 0x40)
        /* do nothing */;
}
static void waitdata(void){
    while ((inb(0x1F7) & 0x88) != 0x08);
}

/* readsect - read a single sector at @secno into @dst */
static void readsect(void* dst, uint32_t secno) {
    // wait for disk to be ready
    waitdisk();

    outb(0x1F2, 1);
    outb(0x1F3, secno & 0xFF);
    outb(0x1F4, (secno >> 8) & 0xFF);
    outb(0x1F5, (secno >> 16) & 0xFF);
    outb(0x1F6, ((secno >> 24) & 0xF) | 0xE0);
    outb(0x1F7, 0x20);                      // cmd 0x20 - read sectors

    // wait for disk data to be ready
    waitdata();

    // read a sector
    //insl(0x1F0, dst, SECTOR_SIZE / 4);
    insw(0x1F0, dst, SECTOR_SIZE / 2);
}

void loadfromdisk(uint64_t start_addr, uint32_t offset, uint32_t len) 
{
    uint64_t end_addr = start_addr + len;
    uint32_t secno = (offset / SECTOR_SIZE) + KERNEL_SECTOR;

    for (; start_addr < end_addr; start_addr += SECTOR_SIZE, secno ++) {
        readsect((void *)start_addr, secno);
    }
}
typedef void (*pfStart)(void);
void kernel_loader(void) 
{
    pfStart kernel_start = (pfStart)KERNEL_ADDR;
    uint64_t *iskernel = (uint64_t*)KERNEL_ADDR;
    uint64_t start_addr = KERNEL_ADDR;
    uint64_t end_addr   = KERNEL_ADDR + SECTOR_SIZE*KERNEL_SECTOR_NUM;
    uint32_t secno      = KERNEL_SECTOR;
    for (; start_addr < end_addr; start_addr += SECTOR_SIZE, secno ++) {
        readsect((void *)start_addr, secno);
    }
    //loadfromdisk((uint64_t)KERNEL_ADDR, 0, SECTOR_SIZE*KERNEL_SECTOR_NUM);
    if (0 != *iskernel)
    {
        kernel_start();
    }
    /* do nothing */
    while (1);
}

