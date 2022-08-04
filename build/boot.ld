ENTRY(klentry)

SECTIONS
{
    . = 0x10000 ; /*Kernel loader should be loaded at 0x10000*/

    .text ALIGN(4K):
    {
        *(.text)
    }

    .rodata ALIGN(4K):
    {
        *(.rodata .rodata.*)
    }

    .data ALIGN(4K):
    {
        *(.data)
    }

    .bss ALIGN(4K):
    {
        *(.bss)
    }
}