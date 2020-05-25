
OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)
SECTIONS
{
    . = 0xffff800000000000 + 0x100000;
    //. = 0x100000;
    _text = .;
    .text : {
        *(.text .stub .text.* .gnu.linkonce.t.*)
    }
    _etext = .;

    _rodata = .;
    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }
    _erodata = .;

    _stab = .;
    /* Include debugging information in kernel memory */
    PROVIDE(__STAB_BEGIN__ = .);
    .stab : {
        *(.stab);
        BYTE(0)     /* Force the linker to allocate space
                   for this section */
    }
    PROVIDE(__STAB_END__ = .);

    PROVIDE(__STABSTR_BEGIN__ = .);
    .stabstr : {
        *(.stabstr);
        BYTE(0)     /* Force the linker to allocate space
                   for this section */
    }
    PROVIDE(__STABSTR_END__ = .);
    _estab = .;
    
    . = ALIGN(0x1000);
    _data = .;
    .data :
    {
        *(.data)
    }
    
    . = ALIGN(0x1000);
    .data.pgdir : {
        *(.data.pgdir)
    }
    _edata = .;
	
	_bss = .;
    .bss :
    {
        *(.bss)
    }
	_ebss = .;
    _end = .;
}

