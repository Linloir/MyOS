ASM_COMPILER := nasm

SRC_PATH := ../src
RUN_PATH := ../run
INCLUDE_PATH := $(addprefix -I, $(dir $(shell find '$(SRC_PATH)' -name '*.inc')))

.PHONY:
build: mbr.bin bootloader.bin
	qemu-img create $(RUN_PATH)/hd.img 10m
	dd if=mbr.bin of=$(RUN_PATH)/hd.img bs=512 count=1 seek=0 conv=notrunc
	dd if=bootloader.bin of=$(RUN_PATH)/hd.img bs=512 count=4 seek=1 conv=notrunc

%.bin: $(SRC_PATH)/boot/%.asm
	$(ASM_COMPILER) -o $@ -f bin $(INCLUDE_PATH) $^

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