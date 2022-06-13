/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:18
 * LastEditTime : 2022-06-12 21:32:17
 * Description  : 
 */

#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "os_type.h"
#include "interruptframe.h"

//ASM WRAPPER FUNCTIONS
extern "C" void asm_empty_interrupt_handler();
extern "C" void asm_time_interrupt_handler();
extern "C" void asm_double_fault_interrupt_handler();
extern "C" void asm_page_fault_interrupt_handler();

//C HANDLER FUNCTIONS
// extern "C" void emptyHandler();
extern "C" void timeInterruptHandler();
extern "C" void doubleFaultInterruptHandler();
extern "C" void pageFaultInterruptHandler(uint32 errCode, uint32 virtualAddr);
__attribute__ ((interrupt)) void emptyHandler(InterruptFrame* frame);

#endif