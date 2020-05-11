BOOT_ADDR   = 0x7C00
LOADER_ADDR = 0x3000
MEM_ADDR    = 0x8000
KERNEL_ADDR = 0x100000

SECTOR_SIZE         = 512
BOOT_SECTOR   		= 0
BOOT_SECTOR_NUM     = 1
LOADER_SECTOR 		= $(shell echo $(BOOT_SECTOR)+$(BOOT_SECTOR_NUM) | bc)
LOADER_SECTOR_NUM 	= 32
KERNEL_SECTOR 		= $(shell echo $(LOADER_SECTOR)+$(LOADER_SECTOR_NUM) | bc)
KERNEL_SECTOR_NUM 	= 4096
LOADER_MAX_SIZE		= $(shell echo $(LOADER_SECTOR_NUM)*$(SECTOR_SIZE) | bc)
KERNEL_MAX_SIZE		= $(shell echo $(KERNEL_SECTOR_NUM)*$(SECTOR_SIZE) | bc)

MACRO_FLAGS := -DSECTOR_SIZE=$(SECTOR_SIZE) -DMEM_ADDR=$(MEM_ADDR)\
	-DLOADER_ADDR=$(LOADER_ADDR) -DKERNEL_ADDR=$(KERNEL_ADDR) \
	-DLOADER_SECTOR=$(LOADER_SECTOR) -DLOADER_SECTOR_NUM=$(LOADER_SECTOR_NUM) \
	-DKERNEL_SECTOR=$(KERNEL_SECTOR) -DKERNEL_SECTOR_NUM=$(KERNEL_SECTOR_NUM)

