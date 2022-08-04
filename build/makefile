ASM_COMPILER := nasm
CPP_COMPILER := clang++-12
CXX_COMPILER_FLAGS := -g -Wall -march=i386 -m32 -nostdlib
LINKER := ld

SRC_PATH := ../src
RUN_PATH := ../run
INCLUDE_PATH := $(addprefix -I, $(dir $(shell find '$(SRC_PATH)' -name '*.h' -o -name '*.inc')))

CPP_SRC := $(shell find '$(SRC_PATH)' -name '*.cpp')
CPP_OBJ := $(patsubst %.cpp,%.o,$(notdir $(CPP_SRC)))

LIB_SRC := $(shell find '$(SRC_PATH)/lib' -name '*.cpp') $(shell find '$(SRC_PATH)/driver' -name '*.cpp')
LIB_OBJ := $(patsubst %.cpp,%.o,$(notdir $(LIB_SRC)))

KERNEL_SRC := $(shell find '$(SRC_PATH)/kernel' -name '*.cpp')
KERNEL_OBJ := $(patsubst %.cpp,%.o,$(notdir $(KERNEL_SRC)))

.PHONY:
build: mbr.bin bootloader.bin kernelloader.bin kernelloaderdebug.o kernel.bin
	qemu-img create $(RUN_PATH)/hd.img 32m
	dd if=mbr.bin of=$(RUN_PATH)/hd.img bs=512 count=1 seek=0 conv=notrunc
	dd if=bootloader.bin of=$(RUN_PATH)/hd.img bs=512 count=4 seek=1 conv=notrunc
	dd if=kernelloader.bin of=$(RUN_PATH)/hd.img bs=512 count=128 seek=5 conv=notrunc
	- umount /mnt/osimg
	- rm -rf /mnt/osimg
	- losetup -d /dev/loop0
	losetup /dev/loop0 --partscan $(RUN_PATH)/hd.img
	mkfs.fat -f 2 -s 1 -S 512 -h 2048 -v /dev/loop0p1
	mkdir -p /mnt/osimg
	mount /dev/loop0p1 /mnt/osimg
	mkdir /mnt/osimg/system
	cp kernel.bin /mnt/osimg/system/kernel.bin
	umount /mnt/osimg
	rm -rf /mnt/osimg
	losetup -d /dev/loop0
	chown linloir $(RUN_PATH)/hd.img

mbr.bin: $(SRC_PATH)/boot/mbr/mbr.asm
	$(ASM_COMPILER) -o $@ -f bin $(INCLUDE_PATH) $^

bootloader.bin: $(SRC_PATH)/boot/bl/bootloader.asm
	$(ASM_COMPILER) -o $@ -f bin $(INCLUDE_PATH) $^

kernelloader.bin: klentry.o kernelloader.o $(LIB_OBJ)
	$(LINKER) -o $@ $^ -m elf_i386 -N -T boot.ld --oformat binary

kernel.bin: kernelentry.o $(KERNEL_OBJ) $(LIB_OBJ)
	$(LINKER) -o $@ $^ -m elf_i386 -N -T kernel.ld --oformat binary

kernelloaderdebug.o: klentry.o kernelloader.o $(LIB_OBJ)
	$(LINKER) -o $@ $^ -m elf_i386 -N -T boot.ld

klentry.o: $(SRC_PATH)/boot/kl/klentry.asm
	$(ASM_COMPILER) -o $@ -g -f elf32 $(INCLUDE_PATH) $^

kernelentry.o: $(SRC_PATH)/kernel/kernelentry.asm
	$(ASM_COMPILER) -o $@ -g -f elf32 $(INCLUDE_PATH) $^

$(CPP_OBJ): $(CPP_SRC)
	$(CPP_COMPILER) $(CXX_COMPILER_FLAGS) $(INCLUDE_PATH) -c $^

.PHONY:
clean:
	rm -f *.o* *.bin
	rm -f $(RUN_PATH)/*.img

.PHONY:
run:
	qemu-system-i386 -hda $(RUN_PATH)/hd.img -vga virtio -serial null -parallel stdio -no-reboot

.PHONY:
debug:
	qemu-system-i386 -S -s -parallel stdio -hda $(RUN_PATH)/hd.img -serial null -no-reboot -d int,cpu_reset