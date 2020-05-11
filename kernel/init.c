
#include <basetype.h>
#include <string.h>
#include <console.h>
#include <cpu.h>
#include <kdebug.h>

__noreturn int start_kernel(void)
{
    //0. 将bss段区间置0
    extern char __sbss[], __ebss[];
    memset(__sbss, 0, __ebss - __sbss);
    
    //1. 初始化串口
    console_init();
    
    //2. 初始化cpu
    cpu_init();
    
    //3. 打印内核信息
    print_kernel_info();
    
    //4. 调用栈功能初始化，便于定位后续问题
    backtrace_init();
    
#if 0
    //5. 初始化中断描述符表
    idt_pre_init();
    
    //6. 初始化物理内存
    mm_init();
    
    //7. 初始化中断控制器
    pic_init();
    
    //8. 初始化虚拟内存
    vmm_init();
    
    //9. 初始化进程调度器
    sched_init();
    
    //10. 初始化时钟
    clock_init();
    
    //11. 初始化其它设备
    device_init();
    
    //12. 开中断
    enable_interrupt();
    
    //13. 最后本函数成为idle进程
    cpu_idle();
#else
    while(1);
#endif
}


