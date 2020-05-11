#ifndef __KDEBUG_H__
#define __KDEBUG_H__

#define STACK_MAX_DEPTH 20

void print_kernel_info(void);
void dump_stack(void);
void backtrace_init(void);

#endif
