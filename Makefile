
BOOT_CFLAGS	:= -Iinclude -fno-builtin -Wall -ggdb -m64 -gstabs -nostdinc -Os #-fno-stack-protector
BOOT_LDFLAGS := -m elf_x86_64 -nostdlib -N

#KERNEL_CFLAGS := -Iinclude -mcmodel=large -fno-builtin -Wall -ggdb -m64 -gstabs -nostdinc -fno-stack-protector
KERNEL_CFLAGS := -mcmodel=large -fno-builtin -m64 -Wall -ggdb -nostdinc -fno-stack-protector
KERNEL_LDFLAGS := -b elf64-x86-64 -z muldefs -m elf_x86_64 -nostdlib -N

HOST_CFLAGS := -Itools/ -g -Wall -O2

KERNEL_S := kernel/head.S
KERNEL_C := kernel/init.c kernel/driver/serial.c kernel/driver/console.c \
	lib/string.c kernel/printk.c kernel/cpu.c kernel/debug/kdebug.c
KERNEL_SRC := $(KERNEL_S) $(KERNEL_C)
KERNEL_INCLUDE := -Iinclude $(addprefix -I,$(sort $(dir $(KERNEL_C))))
KERNEL_S_OBJ := $(patsubst %.S,%.o,$(KERNEL_S))
KERNEL_C_OBJ := $(patsubst %.c,%.o,$(KERNEL_C))

include layout.mak
BOOT_CFLAGS += $(MACRO_FLAGS)

all: bootblock loaderblock kernelblock
	dd if=/dev/zero of=ucore.img count=10240
	dd if=bin/bootblock of=ucore.img conv=notrunc
	dd if=bin/loaderblock of=ucore.img seek=$(LOADER_SECTOR) conv=notrunc
	dd if=bin/kernelblock of=ucore.img seek=$(KERNEL_SECTOR) conv=notrunc
	./run.sh

# bootloader +++++++++++++++++++++++++++++++++++++++++
bootblock:  tools
	gcc -c boot/boot_asm.S -o obj/boot/bootasm.o $(BOOT_CFLAGS)
	ld -o obj/bootblock.o $(BOOT_LDFLAGS) -e start -Ttext $(BOOT_ADDR) obj/boot/bootasm.o
	objcopy -S -O binary obj/bootblock.o obj/bootblock.out	
	bin/sign obj/bootblock.out bin/bootblock mbr  #512B

loaderblock:  tools
	gcc $(BOOT_CFLAGS) -c boot/loader_asm.S -o obj/boot/loader_asm.o
	gcc $(BOOT_CFLAGS) -c boot/loader.c -o obj/boot/loader.o
	ld $(BOOT_LDFLAGS) -e loader_start -Ttext $(LOADER_ADDR) obj/boot/loader_asm.o obj/boot/loader.o -o obj/loaderblock.o
	objcopy -S -O binary obj/loaderblock.o obj/loaderblock.out
	bin/sign obj/loaderblock.out bin/loaderblock $(LOADER_MAX_SIZE)
# kernel +++++++++++++++++++++++++++++++++++++++
kernelblock: tools lib system
	objcopy -S -R ".eh_frame" -R ".comment" -O binary obj/kernel.o obj/kernel.out 
	bin/sign obj/kernel.out bin/kernelblock $(KERNEL_MAX_SIZE)

system :$(KERNEL_S_OBJ) $(KERNEL_C_OBJ)
	ld -o obj/kernel.o $(KERNEL_LDFLAGS) -T kernel.lds $(addprefix obj/,$(KERNEL_S_OBJ) $(KERNEL_C_OBJ))

$(KERNEL_S_OBJ): %.o: %.S
	gcc -c $< -o $(addprefix obj/,$@) $(KERNEL_INCLUDE) $(KERNEL_CFLAGS)

$(KERNEL_C_OBJ): %.o: %.c
	gcc -c $< -o $(addprefix obj/,$@) $(KERNEL_INCLUDE) $(KERNEL_CFLAGS)

# tool +++++++++++++++++++++++++++++++++++++++++++

tools: sign

sign: tmp_dir
	gcc $(HOST_CFLAGS) -c tools/sign.c -o obj/tools/sign.o
	gcc $(HOST_CFLAGS) obj/tools/sign.o -o bin/sign

tmp_dir:
	mkdir -p bin obj/{boot,tools} $(addprefix obj/,$(sort $(dir $(KERNEL_C))))

clean:
	rm -rf obj bin ucore.img

