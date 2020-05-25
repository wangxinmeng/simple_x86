#ifndef __ELF_H__
#define __ELF_H__

/* 32-bit ELF base types */
typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf32_Word;

/* 64-bit ELF base types */
typedef uint64_t Elf64_Addr;
typedef uint16_t Elf64_Half;
typedef int16_t  Elf64_SHalf;
typedef uint64_t Elf64_Off;
typedef int32_t  Elf64_Sword;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Xword;
typedef int64_t  Elf64_Sxword;

#define ELF_MAGIC   0x464C457FU         // "\x7FELF" in little endian

#define EI_NIDENT  16

/* elf文件头 */
/* 可用readelf -h xx.o进行查看 */
typedef struct {
    /* e_ident, 前4个字节必须为0x7F, 0x45, 0x4c, 0x46 */
    /* 第1个字节对应ASCII字符里的DEL控制符，后3个字节刚好是ELF字母的ASCII码 */
    /* 第5个字节，用来标识ELF文件类型，0x01表示32位，0x02表示64位 */
    /* 第6个字节，为字节序，0无效，1小端，2大端 */
    /* 第7个字节，为ELF文件主版本号，一般为1 */
    /* 后面9个字节ELF标准没有定义，一般填0 */
    uint8_t    e_ident[EI_NIDENT];
    /* e_type, 文件类型, ET_开头定义的宏 */
    Elf32_Half e_type;
    /* e_machine, 机器类型, EM_开头定义的宏 */
    Elf32_Half e_machine;
    /* e_version, ELF版本号，一般为1 */
    Elf32_Word e_version;
    /* e_entry, 可执行文件的入口地址，如果是非可执行文件，为0 */
    Elf32_Addr e_entry;
    /* e_phoff, 程序头program header在文件中的偏移，如果没有program header则为0 */
    Elf32_Off  e_phoff;
    /* e_shoff, 节头section header在文件中的偏移，如果没有section header则为0 */
    Elf32_Off  e_shoff;
    /* e_flags, ELF标志位，架构相关，一般为0 */
    Elf32_Word e_flags;
    /* e_ehsize, 此头，即文件头的大小 */
    Elf32_Half e_ehsize;
    /* e_phentsize, program header中1个条目的大小 */
    Elf32_Half e_phentsize;
    /* e_phnum, program header中的条目数量 */
    Elf32_Half e_phnum;
    /* e_shentsize, section header中1个条目的大小 */
    Elf32_Half e_shentsize;
    /* e_shnum, section header中条目的数量 */
    Elf32_Half e_shnum;
    /* e_shstrndx, 节头字符串表所在段在节头表中的下标 */
    Elf32_Half e_shstrndx;  // section number that contains section name strings
}Elf32_Ehdr;

/* e_type对应的类型 */
#define ET_NONE   0
#define ET_REL    1 //可重定位文件，一般为.o文件
#define ET_EXEC   2 //可执行文件
#define ET_DYN    3 //共享目标文件，一般为.so文件
#define ET_CORE   4 //核心文件，在程序崩溃或者进程遇到SIGSEGV段错误时产生
                    //可以使用gdb读取此类文件进行调试
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

/* e_machine对应的类型 */
#define EM_M32    1  //AT&T WE 32100
#define EM_SPARC  2  //SPARC
#define EM_386    3  //Intel x86
#define EM_68K    4  //Motorola 68000
#define EM_88K    5  //Motorola 88000
#define EM_860    6  //Intel 80860

typedef struct {
    /* e_ident, 前4个字节必须为0x7F, 0x45, 0x4c, 0x46 */
    /* 第1个字节对应ASCII字符里的DEL控制符，后3个字节刚好是ELF字母的ASCII码 */
    /* 第5个字节，用来标识ELF文件类型，0x01表示32位，0x02表示64位 */
    /* 第6个字节，为字节序，0无效，1小端，2大端 */
    /* 第7个字节，为ELF文件主版本号，一般为1 */
    /* 后面9个字节ELF标准没有定义，一般填0 */
    uint8_t   e_ident[EI_NIDENT];
    /* e_type, 文件类型, ET_开头定义的宏 */
    Elf64_Half e_type;
    /* e_machine, 机器类型, EM_开头定义的宏 */
    Elf64_Half e_machine;
    /* e_version, ELF版本号，一般为1 */
    Elf64_Word e_version;
    /* e_entry, 可执行文件的入口地址，如果是非可执行文件，为0 */
    Elf64_Addr e_entry;
    /* e_phoff, 程序头program header在文件中的偏移，如果没有program header则为0 */
    Elf64_Off  e_phoff;
    /* e_shoff, 节头section header在文件中的偏移，如果没有section header则为0 */
    Elf64_Off  e_shoff;
    /* e_flags, ELF标志位，架构相关，一般为0 */
    Elf64_Word e_flags;
    /* e_ehsize, 此头，即文件头的大小 */
    Elf64_Half e_ehsize;
    /* e_phentsize, program header中1个条目的大小 */
    Elf64_Half e_phentsize;
    /* e_phnum, program header中的条目数量 */
    Elf64_Half e_phnum;
    /* e_shentsize, section header中1个条目的大小 */
    Elf64_Half e_shentsize;
    /* e_shnum, section header中条目的数量 */
    Elf64_Half e_shnum;
    /* e_shstrndx, 节头字符串表所在段在节头表中的下标  */
    Elf64_Half e_shstrndx;  // section number that contains section name strings
}Elf64_Ehdr;


/* 节头 Section Header Table */
/* 节头中的每一个元素又可称为节描述符Section Descriptor */
/* 节头的第0个元素是无效的段描述符，类型为NULL */
/* 可用readelf -S xx.o进行查看 */
typedef struct {
    /* sh_name, 段名，为一个字符串，位于.shstrtab的字符串表中 */
    /* sh_name是段名字符串在.shstrstab中的偏移 */
    Elf32_Word sh_name;
    /* sh_type, section type段的类型, SHT_开头定义的宏 */
    Elf32_Word sh_type;
    /* sh_flags, section flag段的标志, SHF_开头定义的宏 */
    Elf32_Word sh_flags;
    /* sh_addr, section address段虚拟地址 */
    /* 如果该段可以被加载，则为被加载后在进程空间中的虚拟地址，否则为0 */
    Elf32_Addr sh_addr;
    /* sh_offset, 段偏移 */
    /* 如果段存在于文件中，则表示该段在文件中的偏移 */
    Elf32_Off  sh_offset;
    /* sh_size, 段的长度 */
    Elf32_Word sh_size;
    /* sh_link/sh_info, 段的链接信息 */
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    /* sh_addralign, 段地址对齐 */
    Elf32_Word sh_addralign;
    /* sh_entsize, 项的长度 */
    Elf32_Word sh_entsize;
}Elf32_Shdr;

/* sh_type类型 */
#define SHT_NULL        0 //无效段
#define SHT_PROGBITS    1 //程序段。代码段、数据段均是此类型
#define SHT_SYMTAB      2 //符号表
#define SHT_STRTAB      3 //字符串表
#define SHT_RELA        4 //重定位表
#define SHT_HASH        5 //符号表的哈希表
#define SHT_DYNAMIC     6 //动态链接信息
#define SHT_NOTE        7 //提示性信息
#define SHT_NOBITS      8 //表示该段在文件中没内容，如.bss段
#define SHT_REL         9 //包含重定位信息
#define SHT_SHLIB       10 //保留
#define SHT_DNYSYM      11 //动态链接的符号表
#define SHT_NUM         12
#define SHT_LOPROC      0x70000000
#define SHT_HIPROC      0x7fffffff
#define SHT_LOUSER      0x80000000
#define SHT_HIUSER      0xffffffff

/* sh_flags段标志 */
#define SH_WRITE        1 //表示该段在进程空间中可写
#define SH_ALLOC        2 //表示该段在进程空间中必须要分配空间，如代码段、数据段和bss段
#define SH_EXECINSTR    4 //表示该段在进程空间中可以被执行，一般指代码段
#define SHF_RELA_LIVEPATCH  0x00100000
#define SHF_RO_AFTER_INIT   0x00200000
#define SHF_MASKPROC        0xf0000000

/* 节头 Section Header Table */
/* 节头中的每一个元素又可称为节描述符Section Descriptor */
/* 节头的第0个元素是无效的段描述符，类型为NULL */
/* 可用readelf -S xx.o进行查看 */
typedef struct {
    /* sh_name, 段名，为一个字符串，位于.shstrtab的字符串表中 */
    /* sh_name是段名字符串在.shstrstab中的偏移 */
    Elf64_Word sh_name;
    /* sh_type, section type段的类型, SHT_开头定义的宏 */
    Elf64_Word sh_type;
    /* sh_flags, section flag段的标志, SHF_开头定义的宏 */
    Elf64_Word sh_flags;
    /* sh_addr, section address段虚拟地址 */
    /* 如果该段可以被加载，则为被加载后在进程空间中的虚拟地址，否则为0 */
    Elf64_Addr sh_addr;
    /* sh_offset, 段偏移 */
    /* 如果段存在于文件中，则表示该段在文件中的偏移 */
    Elf64_Off  sh_offset;
    /* sh_size, 段的长度 */
    Elf64_Word sh_size;
    /* sh_link/sh_info, 段的链接信息 */
    Elf64_Word sh_link;
    Elf64_Word sh_info;
    /* sh_addralign, 段地址对齐 */
    Elf64_Word sh_addralign;
    /* sh_entsize, 项的长度 */
    Elf64_Word sh_entsize;
}Elf64_Shdr;


/* 符号表Symbol，对应类型SHT_SYMTAB */
/* 可以使用nm xx.o查看 */
typedef struct {
    /* st_name, 符号名，该符号名在字符串表中的下标 */
    Elf32_Word st_name;
    /* st_value, 符号相对应的值 */
    /* 1.在可执行文件中，如果符号是一个函数或者变量的定义，
        则为这个函数或变量的地址; */
    /* 2.在目标文件中，此符号不是"COMMON"块类型，
        则表示该符号在所属段中的偏移 */
    /* 3.如果是"COMMON"类型的，则表示该符号的对齐属性 */
    Elf32_Addr st_value;
    /* st_size, 符号大小，对于包含数据的符号，为该数据类型的大小 */
    Elf32_Word st_size;
    /* st_info, 低4位为符号类型symbol type，STT_开头的宏; 
        高28位为绑定信息symbol binding，STB_开头的宏*/
    uint8_t    st_info;
    /* st_other, 暂未用 */
    uint8_t    st_other;
    /* st_shndx, 符号所在的段索引 */
    Elf32_Half st_shndx;
} Elf32_Sym;

/* 符号绑定信息, st_info[31:4] */
#define STB_LOCAL       0 //局部符号，对于目标文件的外部不可见
#define STB_GLOBAL      1 //全局符号，外部可见
#define STB_WEAK        2 //弱符号

/* 符号类型, st_info[3:0] */
#define STT_NOTYPE      0 //未知类型符号
#define STT_OBJECT      1 //该符号是个数据对象，比如变量、数组等
#define STT_FUNC        2 //该符号是个函数或其他可执行代码
#define STT_SECTION     3 //该符号表示一个段，这种符号必须是STB_LOCAL的
#define STT_FILE        4 //该符号表示文件名
#define STT_COMMON      5
#define STT_TLS         6

/* 符号所在段特殊常量, st_shndx */
#define SHN_UNDEF       0 //表示该符号未定义。这个符号在本目标文件中被引用，但在其它文件定义
#define SHN_ABS         0xfff1 //表示该符号包含了一个绝对的值，比如文件名
#define SHN_COMMON      0xfff2 //表示该符号是一个"COMMON"块的符号，比如未初始化的全局符号
#define SHN_LORESERVE   0xff00
#define SHN_LOPROC      0xff00
#define SHN_HIPROC      0xff1f
#define SHN_LIVEPATCH   0xff20
#define SHN_HIRESERVE   0xffff

/* 符号表Symbol，对应类型SHT_SYMTAB */
/* 可以使用nm xx.o查看 */
typedef struct {
    /* st_name, 符号名，该符号名在字符串表中的下标 */
    Elf64_Word st_name;
    /* st_value, 符号相对应的值 */
    /* 1.在可执行文件中，如果符号是一个函数或者变量的定义，
        则为这个函数或变量的地址; */
    /* 2.在目标文件中，此符号不是"COMMON"块类型，
        则表示该符号在所属段中的偏移 */
    /* 3.如果是"COMMON"类型的，则表示该符号的对齐属性 */
    Elf64_Addr st_value;
    /* st_size, 符号大小，对于包含数据的符号，为该数据类型的大小 */
    Elf64_Word st_size;
    /* st_info, 低4位为符号类型symbol type，STT_开头的宏; 
        高28位为绑定信息symbol binding，STB_开头的宏*/
    uint8_t    st_info;
    /* st_other, 暂未用 */
    uint8_t    st_other;
    /* st_shndx, 符号所在的段索引 */
    Elf64_Half st_shndx;
} Elf64_Sym;

/* 重定位表， 对应类型SHT_RELA */
/* 可以使用objdump -r xx.o查看 */
typedef struct {
    /* r_offset, 重定位入口的偏移 */
    /* 对于可重定位文件来说，这个值是该重定位入口所要修正的位置的第一个字节相对于段起始的偏移 */
    /* 对于可执行文件或共享对象文件来说，这个值是该重定位入口所要修正的位置的第一个字节的虚拟地址 */
    Elf32_Addr r_offset;
    /* r_info, 重定位入口的类型和符号，低8位表示类型，高24位表示符号在符号表中的下标 */
    /* 对于可执行文件和共享文件来说，重定位入口是动态链接类型的 */
    Elf32_Word r_info;
} Elf32_Rel;

#define R_386_32    1 //绝对寻址修正S+A
#define R_386_PC32  2 //相对寻址修正S+A-P

#define ELF32_R_SYM(x)  ((x) >> 8)
#define ELF32_R_TYPE(x) ((x) & 0xff)

#define ELF64_R_SYM(i)  ((i) >> 32)
#define ELF64_R_TYPE(i) ((i) & 0xffffffff)

/* 重定位表， 对应类型SHT_RELA */
/* 可以使用objdump -r xx.o查看 */
typedef struct {
    /* r_offset, 重定位入口的偏移 */
    /* 对于可重定位文件来说，这个值是该重定位入口所要修正的位置的第一个字节相对于段起始的偏移 */
    /* 对于可执行文件或共享对象文件来说，这个值是该重定位入口所要修正的位置的第一个字节的虚拟地址 */
    Elf64_Addr r_offset;
    /* r_info, 重定位入口的类型和符号，低8位表示类型，高24位表示符号在符号表中的下标 */
    /* 对于可执行文件和共享文件来说，重定位入口是动态链接类型的 */
    Elf64_Word r_info;
} Elf64_Rel;



/* 程序头表Program Header Table */
/* 可以使用readelf -l xx.o查看 */
typedef struct {
    /* p_type, Segment类型，主要关注LOAD类型，PT_开头的宏 */
    Elf32_Word p_type;
    /* p_offset, Segment在文件中的偏移 */
    Elf32_Off  p_offset;
    /* p_vaddr, Segment的第一个字节在进程虚拟地址空间的起始位置 */
    /*  整个程序头表中，所有LOAD类型的元素按照p_vaddr从小到大排列 */
    Elf32_Addr p_vaddr;
    /* p_paddr, Segment的物理装载位置 */
    /* 一般情况下与p_vaddr一致 */
    Elf32_Addr p_paddr;
    /* p_filesz, Segment在ELF文件中所占空间的长度 */
    /*  可能为0，即在文件中不存在内容，如bss */
    Elf32_Word p_filesz; 
    /* p_memsz, Segment在进程虚拟地址空间中所占用的长度 */
    Elf32_Word p_memsz;
    /* p_flags, Segment的权限属性，比如R/W/X,    以PF_开头的宏 */
    Elf32_Word p_flags;
    /* p_align, Segment的对齐属性 */
    /* 实际对齐字节为2的p_align次 */
    Elf32_Word p_align; 
}Elf32_Phdr;

/* p_type */
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_TLS     7               /* Thread local storage segment */
#define PT_LOOS    0x60000000      /* OS-specific */
#define PT_HIOS    0x6fffffff      /* OS-specific */
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7fffffff
#define PT_GNU_EH_FRAME     0x6474e550

#define PT_GNU_STACK        (PT_LOOS + 0x474e551)

/* p_flags */
#define PF_R        0x4
#define PF_W        0x2
#define PF_X        0x1

/* 程序头表Program Header Table */
/* 可以使用readelf -l xx.o查看 */
typedef struct {
    /* p_type, Segment类型，主要关注LOAD类型，PT_开头的宏 */
    Elf64_Word p_type;
    /* p_offset, Segment在文件中的偏移 */
    Elf64_Off  p_offset;
    /* p_vaddr, Segment的第一个字节在进程虚拟地址空间的起始位置 */
    /*  整个程序头表中，所有LOAD类型的元素按照p_vaddr从小到大排列 */
    Elf64_Addr p_vaddr;
    /* p_paddr, Segment的物理装载位置 */
    /* 一般情况下与p_vaddr一致 */
    Elf64_Addr p_paddr;
    /* p_filesz, Segment在ELF文件中所占空间的长度 */
    /*  可能为0，即在文件中不存在内容，如bss */
    Elf64_Word p_filesz; 
    /* p_memsz, Segment在进程虚拟地址空间中所占用的长度 */
    Elf64_Word p_memsz;
    /* p_flags, Segment的权限属性，比如R/W/X,    以PF_开头的宏 */
    Elf64_Word p_flags;
    /* p_align, Segment的对齐属性 */
    /* 实际对齐字节为2的p_align次 */
    Elf64_Word p_align; 
}Elf64_Phdr;

/* .dynamic段 */
/* 可以使用readelf -d xxx.o查看 */
typedef struct {
    Elf32_Sword d_tag;
    union {
        Elf32_Word d_val;
        Elf32_Addr d_ptr;
    } d_un;
} Elf32_Dyn;
typedef struct {
    Elf64_Sword d_tag;
    union {
        Elf64_Word d_val;
        Elf64_Addr d_ptr;
    } d_un;
} Elf64_Dyn;

#define DT_NULL     0
#define DT_NEEDED   1 //依赖的共享对象文件，d_ptr表示所依赖的共享对象文件名
#define DT_PLTRELSZ 2
#define DT_PLTGOT   3
#define DT_HASH     4 //动态链接哈希表地址，d_ptr表示.hash的地址
#define DT_STRTAB   5 //动态链接字符串表的地址，d_ptr表示.dynstr的地址
#define DT_SYMTAB   6 //动态链接符号表的地址，d_ptr表示.dynsym的地址
#define DT_RELA     7 //动态链接重定位表地址
#define DT_RELASZ   8
#define DT_RELAENT  9 //动态重读位表入口数量
#define DT_STRSZ    10 //动态链接字符串表大小，d_val表示大小
#define DT_SYMENT   11
#define DT_INIT     12 //初始化代码地址
#define DT_FINI     13 //结束代码地址
#define DT_SONAME   14 //本共享对象的SO-NAME
#define DT_RPATH    15 //动态链接对象搜索路径
#define DT_SYMBOLIC 16
#define DT_REL      17 //动态链接重定位表地址
#define DT_RELSZ    18
#define DT_RELENT   19 //动态重读位表入口数量
#define DT_PLTREL   20
#define DT_DEBUG    21
#define DT_TEXTREL  22
#define DT_JMPREL   23
#define DT_ENCODING 32
#define OLD_DT_LOOS 0x60000000
#define DT_LOOS     0x6000000d
#define DT_HIOS     0x6ffff000
#define DT_VALRNGLO 0x6ffffd00
#define DT_VALRNGHI 0x6ffffdff
#define DT_ADDRRNGLO    0x6ffffe00
#define DT_ADDRRNGHI    0x6ffffeff
#define DT_VERSYM   0x6ffffff0
#define DT_RELACOUNT    0x6ffffff9
#define DT_RELCOUNT 0x6ffffffa
#define DT_FLAGS_1  0x6ffffffb
#define DT_VERDEF   0x6ffffffc
#define DT_VERDEFNUM    0x6ffffffd
#define DT_VERNEED  0x6ffffffe
#define DT_VERNEEDNUM   0x6fffffff
#define OLD_DT_HIOS     0x6fffffff
#define DT_LOPROC   0x70000000
#define DT_HIPROC   0x7fffffff


/* *
 * STABS debugging info
 *
 * The kernel debugger can understand some debugging information in
 * the STABS format.  For more information on this format, see
 * http://sources.redhat.com/gdb/onlinedocs/stabs_toc.html
 *
 * The constants below define some symbol types used by various debuggers
 * and compilers.  Kernel uses the N_SO, N_SOL, N_FUN, and N_SLINE types.
 * */

#define N_GSYM      0x20    // global symbol
#define N_FNAME     0x22    // F77 function name
#define N_FUN       0x24    // procedure name
#define N_STSYM     0x26    // data segment variable
#define N_LCSYM     0x28    // bss segment variable
#define N_MAIN      0x2a    // main function name
#define N_PC        0x30    // global Pascal symbol
#define N_RSYM      0x40    // register variable
#define N_SLINE     0x44    // text segment line number
#define N_DSLINE    0x46    // data segment line number
#define N_BSLINE    0x48    // bss segment line number
#define N_SSYM      0x60    // structure/union element
#define N_SO        0x64    // main source file name
#define N_LSYM      0x80    // stack variable
#define N_BINCL     0x82    // include file beginning
#define N_SOL       0x84    // included source file name
#define N_PSYM      0xa0    // parameter variable
#define N_EINCL     0xa2    // include file end
#define N_ENTRY     0xa4    // alternate entry point
#define N_LBRAC     0xc0    // left bracket
#define N_EXCL      0xc2    // deleted include file
#define N_RBRAC     0xe0    // right bracket
#define N_BCOMM     0xe2    // begin common
#define N_ECOMM     0xe4    // end common
#define N_ECOML     0xe8    // end common (local name)
#define N_LENG      0xfe    // length of preceding entry

/* Entries in the STABS table are formatted as follows. */
typedef struct {
    Elf32_Word n_strx;        // index into string table of name
    uint8_t n_type;         // type of symbol
    uint8_t n_other;        // misc info (usually empty)
    Elf32_Half n_desc;        // description field
    Elf32_Addr n_value;      // value of symbol
}Elf32_Stab;

typedef struct {
    Elf64_Word n_strx;        // index into string table of name
    uint8_t n_type;         // type of symbol
    uint8_t n_other;        // misc info (usually empty)
    Elf64_Half n_desc;        // description field
    Elf64_Addr n_value;      // value of symbol
}Elf64_Stab;


#endif
