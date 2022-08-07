ENTRY(kernel)

SECTIONS
{
    . = 0xC0000000 ; /*Kernel loader should be loaded at 0x10000*/

    .text ALIGN(4K):
    {
        *(.text)
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