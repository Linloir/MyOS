INCLUDE_PATH := ../include
SRC_PATH := ../src
SRC := $(wildcard $(SRC_PATH)*.asm) $(wildcard $(SRC_PATH)**/*.asm) $(wildcard $(SRC_PATH)*.c) $(wildcard $(SRC_PATH)**/*.c)
OBJ := $($(SRC:.c=.o):.asm=.o)

.PHONY
test:
	echo $(SRC)
	echo $(OBJ)

build: mbr.bin bootloader.bin kernel.bin

kernel.bin: entry.obj $(OBJ):
	$(LINKER) -o $@ -melf_i386 -N $^ -e kernel_entry -Ttext 0x00020000 --oformat binary

%.bin: $(SRC_PATH)/boot/%.asm
	$(ASM_COMPILER) -o $@ -f bin -I$(INCLUDE_PATH)/ $^

entry.obj: $(SRC_PATH)/boot/entry.asm
	$(ASM_COMPILER) -o $@ -g -f elf32 $^