; RAM PLAN------------------------------
; [BLOCK USAGE] | [BLOCK START] | [BLOCK END]   | [NOTE]
; MBR           | 0x07C00       | 0x07E00       | -
; BOOTLOADER    | 0x07E00       | 0x08800       | -
; GDT           | 0x08800       | 0x08880       | 16 * descriptors
; IDT           | 0x08880       | 0x08C80       | 256 * descriptors
; KERNEL        | 0x20000       | -

; CONSTANTS FOR BOOTLOADER--------------
BL_SECTOR_COUNT     equ 5
BL_SECTOR_START     equ 1
BL_START_ADDR       equ 0x7E00
; CONSTANTS FOR GDT---------------------
GDT_START_ADDR      equ 0x8800
EMPTY_SELECTOR      equ 0x00
DATA_SELECTOR       equ 0x08
CODE_SELECTOR       equ 0x10
STACK_SELECTOR      equ 0x18
VIDEO_SELECTOR      equ 0x20
; CONSTANTS FOR KERNEL LOADER----------
KERNEL_LOADER_SECTOR_START equ 6
KERNEL_LOADER_SECTOR_COUNT equ 10
KERNEL_LOADER_START_ADDR   equ 0x10000 ; 64KiB