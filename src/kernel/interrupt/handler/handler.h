/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:18
 * LastEditTime : 2022-06-14 23:54:30
 * Description  : 
 */

#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "os_type.h"
#include "interruptframe.h"

#define Interrupt __attribute__((interrupt))
#define Naked __attribute__((naked))

//C HANDLER FUNCTIONS
// extern "C" void emptyHandler();
Interrupt void timeInterruptHandler(InterruptFrame* frame);
Interrupt void doubleFaultInterruptHandler(InterruptFrame* frame, uint32 errCode);
// __attribute__ ((interrupt)) void pageFaultInterruptHandler(uint32 errCode, uint32 virtualAddr);
Interrupt void emptyHandler(InterruptFrame* frame);
Naked void syscallInterruptHandler();


void eoi();

#endif