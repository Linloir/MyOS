/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:18
 * LastEditTime : 2022-06-13 10:37:34
 * Description  : 
 */

#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "os_type.h"
#include "interruptframe.h"

//C HANDLER FUNCTIONS
// extern "C" void emptyHandler();
__attribute__ ((interrupt)) void timeInterruptHandler(InterruptFrame* frame);
__attribute__ ((interrupt)) void doubleFaultInterruptHandler(InterruptFrame* frame, uint32 errCode);
// __attribute__ ((interrupt)) void pageFaultInterruptHandler(uint32 errCode, uint32 virtualAddr);
__attribute__ ((interrupt)) void emptyHandler(InterruptFrame* frame);

void eoi();

#endif