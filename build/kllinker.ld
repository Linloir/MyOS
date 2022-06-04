ENTRY(kernel_loader_entry)

KERNEL_BEGIN = 0x10000;

SECTIONS {

  . = KERNEL_BEGIN;

  .text ALIGN(4K):
  {
    *(.text .text.*)
  }

  .rodata ALIGN(4K):
  {
    *(.rodata .rodata.*)
  }

  .data ALIGN(4K):
  {
    *(.data .data.*)
  }

  .got ALIGN(4K):
  {
    *(.got .got.*)
  }

  .bss ALIGN(4K):
  {
    *(.bss .bss.*)
  }
}