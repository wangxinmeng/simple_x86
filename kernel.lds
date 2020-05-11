
OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)
SECTIONS
{
    . = 0xffff800000000000 + 0x100000;
    //. = 0x100000;
    .text :
    {
        _text = .;
        *(.text)
        _etext = .;
    }
    __etext = .;

    . = ALIGN(8);

    .data :
    {
        _data = .;
        *(.data)
        _edata = .;
    }
	__edata = .;
    .rodata : 
    {
        _rodata = .;
        *(.rodata)
        _erodata = .;
    }
	__erodata = .;
	
    /* Include debugging information in kernel memory */
    .stab : {
        PROVIDE(__STAB_BEGIN__ = .);
        *(.stab);
        PROVIDE(__STAB_END__ = .);
        BYTE(0)     /* Force the linker to allocate space
                   for this section */
    }

    .stabstr : {
        PROVIDE(__STABSTR_BEGIN__ = .);
        *(.stabstr);
        PROVIDE(__STABSTR_END__ = .);
        BYTE(0)     /* Force the linker to allocate space
                   for this section */
    }
    . = ALIGN(32768);
    .data.init_task : { *(.data.init_task) }
	__sbss = .;
    .bss :
    {
        _bss = .;
        *(.bss)
        _ebss = .;
    }
	__ebss = .;
    _end = .;
}

