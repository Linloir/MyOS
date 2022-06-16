/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:18
 * LastEditTime : 2022-06-16 09:03:28
 * Description  : 
 */

#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "os_type.h"
#include "interruptframe.h"

#define Interrupt __attribute__((interrupt))
#define Naked __attribute__((naked))

#define __interrupt_handler__(fn) \
    Naked void (fn)() {\
        asm( \
            "push %%ds\n\t" \
            "push %%es\n\t" \
            "push %%fs\n\t" \
            "push %%gs\n\t" \
            "push %%esi\n\t" \
            "push %%edi\n\t" \
            "push %%ebp\n\t" \
            "push %%edx\n\t" \
            "push %%ecx\n\t" \
            "push %%ebx\n\t" \
            "push %%eax\n\t" \
            "push %%esp\n\t" \
            "movl $.return, %%eax" \
            "push %%eax" \
            "call %[handler]\n\t" \
            ".return:\n\t" \
            "pop %%eax"
            : \
            : [handler]"i"((fn)) \
        ); \
    }

//C HANDLER FUNCTIONS
// extern "C" void emptyHandler();
Interrupt void timeInterruptHandler(InterruptFrame* frame);
Interrupt void doubleFaultInterruptHandler(InterruptFrame* frame, uint32 errCode);
// __attribute__ ((interrupt)) void pageFaultInterruptHandler(uint32 errCode, uint32 virtualAddr);
Interrupt void emptyHandler(InterruptFrame* frame);
Naked void syscallInterruptHandler();


void eoi();

#endif