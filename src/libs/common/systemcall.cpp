/*** 
 * Author       : Linloir
 * Date         : 2022-06-16 20:38:14
 * LastEditTime : 2022-06-17 16:02:42
 * Description  : 
 */

#include "systemcall.h"

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
        "addl $4, %esp\n\t"
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
        "addl $4, %esp\n\t"
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
        "addl $4, %esp\n\t"
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
        "addl $4, %esp\n\t"
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
        "addl $4, %esp\n\t"
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
        "addl $4, %esp\n\t"
    );
    //Return
    asm volatile(
        "ret\n\t"
    );
}

void syscall_panic(uint32 code) {
    syscall(0x0);
}

void syscall_sleep(uint32 time) {
    syscall(0x1, time);
}

void syscall_exit(uint32 retval) {
    syscall(0x2, retval);
}

uint32 syscall_wait(int* retptr) {
    return syscall(0x3, (uint32)retptr);
}

void syscall_hibernate() {
    syscall(0x4);
}

void syscall_awake(uint32 pid) {
    syscall(0x5, pid);
}

uint32 syscall_fork() {
    return syscall(0x6);
}

uint32 syscall_span() {
    return syscall(0x7);
}

void syscall_portWrite(uint32 port, uint32 val) {
    syscall(0x8, port, val);
}

uint32 syscall_portRead(uint32 port) {
    return syscall(0x9, port);
}

uint32 syscall_pid() {
    return syscall(0xA);
}