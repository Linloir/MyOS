/*** 
 * Author       : Linloir
 * Date         : 2022-06-14 21:41:21
 * LastEditTime : 2022-06-15 10:09:03
 * Description  : 
 */
/***
 * Author       : Linloir
 * Date         : 2022-06-14 21:41:21
 * LastEditTime : 2022-06-14 23:52:57
 * Description  :
 */

#include "syscallmanager.h"

Syscall SyscallManager::_syscall[MAX_SYSTEM_CALL];

uint32 SyscallManager::_handler(uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4) {
    return _syscall[vec](p0, p1, p2, p3, p4);
} 

void SyscallManager::initialize() {
    for(int i = 0; i < MAX_SYSTEM_CALL; i++) {
        _syscall[i] = (Syscall)0;
    }
    _syscall[0] = syscallTest;
}

uint32 syscall(uint32 vec) {
    //Previous on stack:
    //uint32 vec
    //uint32 eip
    //Preserve State
    asm volatile(
        "subl $4, %esp\n\t"     //uint32 return val
        "push %ebp\n\t"
        "movl %esp, %ebp\n\t"
        "pushal\n\t"
    );
    //Set parameters
    asm volatile(
        "movl $0, %eax\n\t"             //p4 = 0
        "movl $0, %ebx\n\t"             //p3 = 0
        "movl $0, %ecx\n\t"             //p2 = 0
        "movl $0, %edx\n\t"             //p1 = 0
        "movl $0, %esi\n\t"             //p0 = 0
        "movl 12(%ebp), %edi\n\t"   //vec = vec
    );
    //Call interrupt
    asm volatile(
        "int $0x80\n\t"
    );
    //Save return val
    asm volatile(
        "movl %eax, 4(%ebp)\n\t"
    );
    //Restore State and set return val
    asm volatile(
        "popal\n\t"
        "movl 4(%ebp), %eax\n\t"
        "pop %ebp\n\t"
    );
    //Return
    asm volatile(
        "ret\n\t"
    );
}

uint32 syscall(uint32 vec, uint32 p0) {
    //Previous on stack:
    //uint32 p0
    //uint32 vec
    //uint32 eip
    //Preserve State
    asm volatile(
        "subl $4, %esp\n\t"     //uint32 return val
        "push %ebp\n\t"
        "movl %esp, %ebp\n\t"
        "pushal\n\t"
    );
    //Set parameters
    asm volatile(
        "movl $0, %eax\n\t"         //p4 = 0
        "movl $0, %ebx\n\t"         //p3 = 0
        "movl $0, %ecx\n\t"         //p2 = 0
        "movl $0, %edx\n\t"         //p1 = 0
        "movl 16(%ebp), %esi\n\t"   //p0 = p0
        "movl 12(%ebp), %edi\n\t"   //vec = vec
    );
    //Call interrupt
    asm volatile(
        "int $0x80\n\t"
    );
    //Save return val
    asm volatile(
        "movl %eax, 4(%ebp)\n\t"
    );
    //Restore State and set return val
    asm volatile(
        "popal\n\t"
        "movl 4(%ebp), %eax\n\t"
        "pop %ebp\n\t"
    );
    //Return
    asm volatile(
        "ret\n\t"
    );
}

uint32 syscall(uint32 vec, uint32 p0, uint32 p1) {
    //Previous on stack:
    //uint32 p1
    //uint32 p0
    //uint32 vec
    //uint32 eip
    //Preserve State
    asm volatile(
        "subl $4, %esp\n\t"     //uint32 return val
        "push %ebp\n\t"
        "movl %esp, %ebp\n\t"
        "pushal\n\t"
    );
    //Set parameters
    asm volatile(
        "movl $0, %eax\n\t"         //p4 = 0
        "movl $0, %ebx\n\t"         //p3 = 0
        "movl $0, %ecx\n\t"         //p2 = 0
        "movl 20(%ebp), %edx\n\t"   //p1 = p1
        "movl 16(%ebp), %esi\n\t"   //p0 = p0
        "movl 12(%ebp), %edi\n\t"   //vec = vec
    );
    //Call interrupt
    asm volatile(
        "int $0x80\n\t"
    );
    //Save return val
    asm volatile(
        "movl %eax, 4(%ebp)\n\t"
    );
    //Restore State and set return val
    asm volatile(
        "popal\n\t"
        "movl 4(%ebp), %eax\n\t"
        "pop %ebp\n\t"
    );
    //Return
    asm volatile(
        "ret\n\t"
    );
}

uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2) {
    //Previous on stack:
    //uint32 p2
    //uint32 p1
    //uint32 p0
    //uint32 vec
    //uint32 eip
    //Preserve State
    asm volatile(
        "subl $4, %esp\n\t"     //uint32 return val
        "push %ebp\n\t"
        "movl %esp, %ebp\n\t"
        "pushal\n\t"
    );
    //Set parameters
    asm volatile(
        "movl $0, %eax\n\t"         //p4 = 0
        "movl $0, %ebx\n\t"         //p3 = 0
        "movl 24(%ebp), %ecx\n\t"   //p2 = p2
        "movl 20(%ebp), %edx\n\t"   //p1 = p1
        "movl 16(%ebp), %esi\n\t"   //p0 = p0
        "movl 12(%ebp), %edi\n\t"   //vec = vec
    );
    //Call interrupt
    asm volatile(
        "int $0x80\n\t"
    );
    //Save return val
    asm volatile(
        "movl %eax, 4(%ebp)\n\t"
    );
    //Restore State and set return val
    asm volatile(
        "popal\n\t"
        "movl 4(%ebp), %eax\n\t"
        "pop %ebp\n\t"
    );
    //Return
    asm volatile(
        "ret\n\t"
    );
}

uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3) {
    //Previous on stack:
    //uint32 p3
    //uint32 p2
    //uint32 p1
    //uint32 p0
    //uint32 vec
    //uint32 eip
    //Preserve State
    asm volatile(
        "subl $4, %esp\n\t"     //uint32 return val
        "push %ebp\n\t"
        "movl %esp, %ebp\n\t"
        "pushal\n\t"
    );
    //Set parameters
    asm volatile(
        "movl $0, %eax\n\t"         //p4 = 0
        "movl 28(%ebp), %ebx\n\t"   //p3 = p3
        "movl 24(%ebp), %ecx\n\t"   //p2 = p2
        "movl 20(%ebp), %edx\n\t"   //p1 = p1
        "movl 16(%ebp), %esi\n\t"   //p0 = p0
        "movl 12(%ebp), %edi\n\t"   //vec = vec
    );
    //Call interrupt
    asm volatile(
        "int $0x80\n\t"
    );
    //Save return val
    asm volatile(
        "movl %eax, 4(%ebp)\n\t"
    );
    //Restore State and set return val
    asm volatile(
        "popal\n\t"
        "movl 4(%ebp), %eax\n\t"
        "pop %ebp\n\t"
    );
    //Return
    asm volatile(
        "ret\n\t"
    );
}

uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4) {
    //Previous on stack:
    //uint32 p4
    //uint32 p3
    //uint32 p2
    //uint32 p1
    //uint32 p0
    //uint32 vec
    //uint32 eip
    //Preserve State
    asm volatile(
        "subl $4, %esp\n\t"     //uint32 return val
        "push %ebp\n\t"
        "movl %esp, %ebp\n\t"
        "pushal\n\t"
    );
    //Set parameters
    asm volatile(
        "movl 32(%ebp), %eax\n\t"   //p4 = p4
        "movl 28(%ebp), %ebx\n\t"   //p3 = p3
        "movl 24(%ebp), %ecx\n\t"   //p2 = p2
        "movl 20(%ebp), %edx\n\t"   //p1 = p1
        "movl 16(%ebp), %esi\n\t"   //p0 = p0
        "movl 12(%ebp), %edi\n\t"   //vec = vec
    );
    //Call interrupt
    asm volatile(
        "int $0x80\n\t"
    );
    //Save return val
    asm volatile(
        "movl %eax, 4(%ebp)\n\t"
    );
    //Restore State and set return val
    asm volatile(
        "popal\n\t"
        "movl 4(%ebp), %eax\n\t"
        "pop %ebp\n\t"
    );
    //Return
    asm volatile(
        "ret\n\t"
    );
}