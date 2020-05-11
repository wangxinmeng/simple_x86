#add-symbol-file obj/bootblock.o 0x7C00
#add-symbol-file obj/loaderblock.o 0x3000
#add-symbol-file obj/kernel.o 0x100000
file obj/kernel.o 
target remote :1234
break start_kernel
continue
