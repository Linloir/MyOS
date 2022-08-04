[bits 32]

global klentry
extern kernelLoader
klentry:
    jmp kernelLoader