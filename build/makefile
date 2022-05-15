ASM_COMPILER := nasm
CPP_COMPILER := g++
CXX_COMPILER_FLAGS := -g -Wall -march=i386 -m32 -nostdlib -fno-builtin -ffreestanding -fno-pic
LINKER := ld

INCLUDE_PATH := $(addprefix -I,$(wildcard ../include/*/))
SRC_PATH := ../src
RUN_PATH := ../run

CPP_SRC := $(shell find '$(SRC_PATH)' -name '*.cpp' -not -path '$(SRC_PATH)/boot/*')
ASM_SRC := $(shell find '$(SRC_PATH)' -name '*.asm' -not -path '$(SRC_PATH)/boot/*')
CPP_OBJ := $(patsubst %.cpp,%.o,$(notdir $(CPP_SRC)))
ASM_OBJ := $(patsubst %.asm,%.o,$(notdir $(ASM_SRC)))

.PHONY:
build: mbr.bin bootloader.bin kernel.bin debug.o
	qemu-img create $(RUN_PATH)/hd.img 10m
	dd if=mbr.bin of=$(RUN_PATH)/hd.img bs=512 count=1 seek=0 conv=notrunc
	dd if=bootloader.bin of=$(RUN_PATH)/hd.img bs=512 count=5 seek=1 conv=notrunc
	dd if=kernel.bin of=$(RUN_PATH)/hd.img bs=512 count=200 seek=6 conv=notrunc

kernel.bin: entry.obj $(CPP_OBJ) $(ASM_OBJ)
	$(LINKER) -o $@ -melf_i386 -N $^ -e kernel_entry -Ttext 0x00020000 --oformat binary

debug.o: entry.obj $(CPP_OBJ) $(ASM_OBJ)
	$(LINKER) -o $@ -melf_i386 -N $^ -e kernel_entry -Ttext 0x00020000

%.bin: $(SRC_PATH)/boot/%.asm
	$(ASM_COMPILER) -o $@ -f bin $(INCLUDE_PATH) $^

entry.obj: $(SRC_PATH)/boot/entry.asm
	$(ASM_COMPILER) -o $@ -g -f elf32 $^

$(CPP_OBJ): $(CPP_SRC)
	$(CPP_COMPILER) $(CXX_COMPILER_FLAGS) $(INCLUDE_PATH) -c $(CPP_SRC)

$(ASM_OBJ): $(ASM_SRC)
	$(ASM_COMPILER) -o $@ -g -f elf32 $(filter %/$*.asm,$^)

.PHONY:
clean:
	rm -f *.o* *.bin
	rm -f $(RUN_PATH)/*.img

.PHONY:
run:
	qemu-system-i386 -hda $(RUN_PATH)/hd.img -serial null -parallel stdio -no-reboot

.PHONY:
debug:
	qemu-system-i386 -S -s -parallel stdio -hda $(RUN_PATH)/hd.img -serial null -no-reboot -d int,cpu_reset &
	@sleep 1
	gnome-terminal -- gdb -q -x $(RUN_PATH)/gdbinit