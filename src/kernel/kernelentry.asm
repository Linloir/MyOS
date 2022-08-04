[bits 32]

global kernelentry
extern kernel
kernelentry:
    jmp kernel