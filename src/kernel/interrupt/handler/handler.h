/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:18
 * LastEditTime : 2022-06-16 19:52:33
 * Description  : 
 */

#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "os_type.h"
#include "interruptframe.h"
#include "proc.h"

#define Interrupt __attribute__((interrupt))
#define Naked __attribute__((naked))
#define DeclareConsistentHandler(fn) Naked void fn(); void __##fn##_asm__ (ProcessState* preservedState)

#define __consistent_handler__(fn)                                                          \
    Naked void fn()                                                                         \
    {                                                                                       \
        /*Force change to ESP0 stack*/                                                      \
        asm volatile(                                                                       \
            "subl $4, %%esp\n\t"    /*make space for esp*/                                  \
                                                                                            \
            "push %%ebp\n\t"                                                                \
            "movl %%esp, %%ebp\n\t"                                                         \
            "addl $8, %%ebp\n\t"    /*Now ebp points at %eip (top of interrupt frame)*/     \
                                                                                            \
            "push %%eax\n\t"                                                                \
            "movl %[esp0], %%eax\n\t"                                                       \
            "subl $20, %%eax\n\t"    /*Now ebp points at %eip (top of interrupt frame)*/    \
                                                                                            \
            "push %%ebx\n\t"    /*transfer register*/                                       \
                                                                                            \
            "movl 0(%%ebp), %%ebx\n\t"                                                      \
            "movl %%ebx, 0(%%eax)\n\t"                                                      \
            "movl 4(%%ebp), %%ebx\n\t"                                                      \
            "movl %%ebx, 4(%%eax)\n\t"                                                      \
            "movl 8(%%ebp), %%ebx\n\t"                                                      \
            "movl %%ebx, 8(%%eax)\n\t"   /*copy eip, cs, eflags to esp0 stack*/             \
                                                                                            \
            "subl $8, %%ebp\n\t"                                                            \
            "movl %%ebp, -4(%%ebp)\n\t"  /*copy actual esp to esp0 stack top*/              \
                                                                                            \
            "pop %%ebx\n\t"                                                                 \
            "pop %%eax\n\t"                                                                 \
            "pop %%ebp\n\t"                                                                 \
                                                                                            \
            "movl %%eax, %%esp\n\t" /*move to esp0 stack*/                                  \
            :                                                                               \
            : [esp0]"i"(ESP0_STACK_TOP)                                                     \
        );                                                                                  \
        asm volatile(                                                                       \
            "push %%gs\n\t"                                                                 \
            "push %%fs\n\t"                                                                 \
            "push %%es\n\t"                                                                 \
            "push %%ds\n\t"                                                                 \
            "push %%esi\n\t"                                                                \
            "push %%edi\n\t"                                                                \
            "push %%ebp\n\t"                                                                \
            "push %%edx\n\t"                                                                \
            "push %%ecx\n\t"                                                                \
            "push %%ebx\n\t"                                                                \
            "push %%eax\n\t"                                                                \
            "movl %%esp, %%eax\n\t"                                                         \
            "push %%eax\n\t"                                                                \
            "call %P0\n\t"                                                                  \
            "addl $4, %%esp\n\t"                                                            \
            "pop %%eax\n\t"                                                                 \
            "pop %%ebx\n\t"                                                                 \
            "pop %%ecx\n\t"                                                                 \
            "pop %%edx\n\t"                                                                 \
            "pop %%ebp\n\t"                                                                 \
            "pop %%edi\n\t"                                                                 \
            "pop %%esi\n\t"                                                                 \
            "pop %%ds\n\t"                                                                  \
            "pop %%es\n\t"                                                                  \
            "pop %%fs\n\t"                                                                  \
            "pop %%gs\n\t"                                                                  \
            :                                                                               \
            : "i"(__##fn##_asm__)                                                           \
        );                                                                                  \
        /*Update change to whatever stack it used to be*/                                   \
        asm volatile(                                                                       \
            "pop %%esp\n\t" /*Return to whatever stack it used to be*/                      \
                                                                                            \
            "push %%ebp\n\t"                                                                \
            "movl %%esp, %%ebp\n\t"                                                         \
            "addl $4, %%ebp\n\t" /*Now ebp points at %eip (top of prev interrupt frame)*/   \
                                                                                            \
            "push %%eax\n\t"                                                                \
            "movl %[esp0], %%eax\n\t"                                                       \
            "subl $20, %%eax\n\t" /*Now ebp points at %eip (top of new interrupt frame)*/   \
                                                                                            \
            "push %%ebx\n\t" /*transfer register*/                                          \
                                                                                            \
            "movl 0(%%eax), %%ebx\n\t"                                                      \
            "movl %%ebx, 0(%%ebp)\n\t"                                                      \
            "movl 4(%%eax), %%ebx\n\t"                                                      \
            "movl %%ebx, 4(%%ebp)\n\t"                                                      \
            "movl 8(%%eax), %%ebx\n\t"                                                      \
            "movl %%ebx, 8(%%ebp)\n\t" /*copy eip, cs, eflags to prev stack*/               \
                                                                                            \
            "pop %%ebx\n\t"                                                                 \
            "pop %%eax\n\t"                                                                 \
            "pop %%ebp\n\t"                                                                 \
            :                                                                               \
            : [esp0] "i"(ESP0_STACK_TOP));                                                  \
        asm volatile(                                                                       \
            "iret\n\t" /*iret*/                                                             \
        );                                                                                  \
    }                                                                                       \
    void __##fn##_asm__ (ProcessState* preservedState)

//C HANDLER FUNCTIONS
// extern "C" void emptyHandler();
// Interrupt void timeInterruptHandler(InterruptFrame* frame);
Interrupt void doubleFaultInterruptHandler(InterruptFrame* frame, uint32 errCode);
// __attribute__ ((interrupt)) void pageFaultInterruptHandler(uint32 errCode, uint32 virtualAddr);
Interrupt void emptyHandler(InterruptFrame* frame);
Naked void syscallInterruptHandler();

DeclareConsistentHandler(timeInterruptHandler);

void eoi();

#endif