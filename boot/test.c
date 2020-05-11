
#include <stdio.h>
#include <stdlib.h>

#define ASM_C_DEBUG
#include "asm.h"
#include "boot_asm.h"

int main(void)
{
    printf("E_CODE64_SELECTOR: 0x%04X\n", E_CODE64_SELECTOR);
    printf("E_DATA64_SELECTOR: 0x%04X\n", E_DATA64_SELECTOR);
    printf("E_CODE64_SEG_DESC_DEF(DPL(0)): 0x%016llX\n", E_CODE64_SEG_DESC_DEF(DPL(0)));
    printf("E_DATA64_SEG_DESC_DEF(DPL(0)): 0x%016llX\n", E_DATA64_SEG_DESC_DEF(DPL(0)));
    printf("e_cr3:    0x%016llX\n", E_CR3_DEF(E_PML4T_BASE));
    printf("e_pml4e:  0x%016llX\n", E_PML4E_DEF(E_PDPT_BASE));
    printf("e_pdpte:  0x%016llX\n", E_PDPTE_DEF(E_PDT_BASE));
    printf("e_pde:    0x%016llX\n", E_PDE_2M_DEF(0));
    printf("e_pde(2M):0x%016llX\n", E_PDE_2M_DEF(ASM_MB(2)));
    return 0;
}
