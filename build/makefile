ASM_COMPILER := nasm
CPP_COMPILER := g++
CXX_COMPILER_FLAGS := -g -Wall -march=i386 -m32 -nostdlib -fno-builtin -ffreestanding -fno-pic -fno-exceptions -fno-unwind-tables -fno-threadsafe-statics
LINKER := ld

# INCLUDE_PATH := $(addprefix -I,$(wildcard ../include/*/))
SRC_PATH := ../src
RUN_PATH := ../run
INCLUDE_PATH := $(addprefix -I, $(dir $(shell find '$(SRC_PATH)' -name '*.h' -o -name '*.inc')))

BOOT_CPP_SRC := $(shell find '$(SRC_PATH)/boot/kernelloader' -name '*.cpp')
BOOT_ASM_SRC := $(shell find '$(SRC_PATH)/boot/kernelloader' -name '*.asm')
BOOT_CPP_OBJ := $(patsubst %.cpp,%.o,$(notdir $(BOOT_CPP_SRC)))
BOOT_ASM_OBJ := $(patsubst %.asm,%.o,$(notdir $(BOOT_ASM_SRC)))

CPP_SRC := $(shell find '$(SRC_PATH)' -name '*.cpp' -not -path '$(SRC_PATH)/boot/*')
ASM_SRC := $(shell find '$(SRC_PATH)' -name '*.asm' -not -path '$(SRC_PATH)/boot/*')
CPP_OBJ := $(patsubst %.cpp,%.o,$(notdir $(CPP_SRC)))
ASM_OBJ := $(patsubst %.asm,%.o,$(notdir $(ASM_SRC)))

.PHONY:
test: cppAll.o

.PHONY:
build: mbr.bin bootloader.bin kernelloader.bin kernel.bin debug.o kernelloaderdebug.o
	qemu-img create $(RUN_PATH)/hd.img 10m
	dd if=mbr.bin of=$(RUN_PATH)/hd.img bs=512 count=1 seek=0 conv=notrunc
	dd if=bootloader.bin of=$(RUN_PATH)/hd.img bs=512 count=5 seek=1 conv=notrunc
	dd if=kernelloader.bin of=$(RUN_PATH)/hd.img bs=512 count=10 seek=6 conv=notrunc
	dd if=kernel.bin of=$(RUN_PATH)/hd.img bs=512 count=200 seek=16 conv=notrunc

kernelloader.bin: $(BOOT_CPP_OBJ) $(BOOT_ASM_OBJ)
	$(LINKER) -o $@ -melf_i386 -N entry.o $(filter-out entry.o,$^) -Tkllinker.ld --oformat binary

kernelloaderdebug.o: $(BOOT_CPP_OBJ) $(BOOT_ASM_OBJ)
	$(LINKER) -o $@ -melf_i386 -N entry.o $(filter-out entry.o,$^) -Tkllinker.ld

kernel.bin: $(CPP_OBJ) $(ASM_OBJ)
	$(LINKER) -o $@ -melf_i386 -N kernelentry.o $(filter-out kernelentry.o, $^) -Tlinker.ld --oformat binary

debug.o: $(CPP_OBJ) $(ASM_OBJ)
	$(LINKER) -o $@ -melf_i386 -N kernelentry.o $(filter-out kernelentry.o, $^) -Tlinker.ld

%.bin: $(SRC_PATH)/boot/%.asm
	$(ASM_COMPILER) -o $@ -f bin $(INCLUDE_PATH) $^

$(CPP_OBJ): $(CPP_SRC)
	$(CPP_COMPILER) $(CXX_COMPILER_FLAGS) $(INCLUDE_PATH) -c $(CPP_SRC)

cppAll.o: $(CPP_SRC)
	$(CPP_COMPILER) -o $@ $(CXX_COMPILER_FLAGS) $(INCLUDE_PATH) $(CPP_SRC)

$(ASM_OBJ): $(ASM_SRC)
	$(ASM_COMPILER) -o $@ -g -f elf32 $(filter %/$*.asm,$^)

$(BOOT_CPP_OBJ): $(BOOT_CPP_SRC)
	$(CPP_COMPILER) $(CXX_COMPILER_FLAGS) $(INCLUDE_PATH) -c $(BOOT_CPP_SRC)

$(BOOT_ASM_OBJ): $(BOOT_ASM_SRC)
	$(ASM_COMPILER) -o $@ -g -f elf32 $(INCLUDE_PATH) $(filter %/$*.asm,$^)

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