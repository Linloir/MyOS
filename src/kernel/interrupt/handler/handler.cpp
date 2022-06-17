/*** 
 * Author       : Linloir
 * Date         : 2022-06-16 14:36:06
 * LastEditTime : 2022-06-17 11:22:09
 * Description  : 
 */
/***
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:21
 * LastEditTime : 2022-06-16 17:17:10
 * Description  :
 */

#include "systemconstant.h"
#include "handler.h"
#include "interrupt.h"
#include "systemio.h"
#include "framemanager.h"
#include "processmanager.h"
#include "paging.h"
#include "proc.h"
#include "syscallmanager.h"

// extern "C" void emptyHandler() {
//     asm_disable_interrupt();
//     printf("Unhandled interrupt\n");
//     while(true){
//         //Halt
//     }
// }

Interrupt void emptyHandler(InterruptFrame *frame)
{
    printf("Empty handler\n");
    while (true)
    {
    }
}

__consistent_handler__ (timeInterruptHandler) (ProcessState* state) {
    eoi();
    ProcessManager::schedule(state);
}

Interrupt void doubleFaultInterruptHandler(InterruptFrame *frame, uint32 errCode)
{

    printf("Double fault, err code: %d\n", errCode);
    while (true)
    {
    }
}

__consistent_handler__ (syscallHandler) (ProcessState* state) {
    eoi();
    SyscallManager::handler(state, state->_edi, state->_esi, state->_edx, state->_ecx, state->_ebx, state->_eax);
}

void eoi()
{
    asm(
        "movb $0x20, %al\n\t"
        "outb %al, $0x20\n\t"
        "outb %al, $0xA0\n\t");
}