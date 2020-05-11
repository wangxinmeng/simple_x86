
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

    BOOT阶段只使用第1部分，划分；
        0x00500 ~ 0x02FFF 预留， 11008B，10.75KB
        0x03000 ~ 0x06FFF loader, 16384B, 16KB
        0x07000 ~ 0x07BFF 预留， 3072B，3KB
*/


/* boot的功能是从硬盘加载sector1 ~ sector 33，共16KB(loader)，到0x03000~0x06FFF
*/

# 开始地址在0:7c00，实模式
.globl start
start:
.code16
real_mode:
/* 初始化CS除外的寄存器 */
    movw %cs, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw %ax, %fs
    movw $0x7C00, %sp
    movw $0xb800, %ax
    movw %ax, %gs

/* 设置读硬盘参数 */
    movl $(LOADER_ADDR), %ebx     //写入地址, offset
read_disk:
/*
    根据ebx地址，计算待读取的扇区号 ，存放在eax里面
    ToLoadMemAddr[ebx] = LOADER_ADDR + (secno[eax] - LOADER_SECTOR)*512
    则eax = ((ebx - LOADER_ADDR) >> 9)  + LOADER_SECTOR
*/
    movl %ebx, %eax
    sub $(LOADER_ADDR), %eax
    ror $9, %eax
    add $(LOADER_SECTOR), %eax
    movw $1, %cx         //待读入的扇区数，每次只读取1个扇区
    call read_disk_one_sector16  //此函数会更新bx的值，更新为bx=bx+SECTOR_SIZE
    cmp $(LOADER_ADDR+LOADER_SECTOR_NUM*512), %bx
    jb read_disk

read_over:
    jmp LOADER_ADDR

read_disk_one_sector16:

    movl %eax, %esi //备份eax
    movw %cx, %di   //备份cx

//1.设置读取的扇区数
    movw $0x1f2, %dx
    movb %cl, %al
    outb %al, %dx  //读取的扇区数
    
    movl %esi, %eax //恢复eax

//2.将LBA地址存入0x1f3 ~ 0x1f6
    movw $0x1f3, %dx  //LBA地址0~7写入0x1f3
    outb %al, %dx

    movb $8, %cl 
    ror %cl, %eax 
    
    movw $0x1f4, %dx  //LBA地址8~15写入0x1f4
    outb %al, %dx 
    
    ror %cl, %eax 
    movw $0x1f5, %dx  //LBA地址16~23写入0x1f5
    outb %al, %dx 
    
    ror %cl, %eax 
    and $0xf, %al
    or  $0xe0, %al    //设置4~7位为1110，表示LBA模式
    movw $0x1f6, %dx  //LBA地址24~27写入0x1f6
    outb %al, %dx 

//3.向0x1f7端口写入读命令，0x20
    movw $0x1f7, %dx
    movb $0x20, %al 
    outb %al, %dx

//4.检测硬盘状态
.not_ready:
    nop
    inb %dx, %al 
    and $0x88, %al 
    cmp $0x08, %al 
    jne .not_ready

//5.从0x1f0端口读数据
    movw %di, %ax
    movw $256, %dx
    mul %dx
    movw %ax, %cx
    movw $0x1f0, %dx
.go_on_read:
    inw %dx, %ax
    movw %ax, (%bx)
    addw $2, %bx 
    loop .go_on_read
    ret


