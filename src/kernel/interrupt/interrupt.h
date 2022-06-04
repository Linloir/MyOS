/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 21:03:12
 * LastEditTime : 2022-06-04 18:35:05
 * Description  : 
 */
#ifndef _INTERRUPT_UTILS_H_
#define _INTERRUPT_UTILS_H_

#include "os_type.h"

#define IRQ0_8259A_MASTER 0x20
#define IRQ0_8259A_SLAVE 0x28

extern "C" int asm_interrupt_status();
extern "C" void asm_enable_interrupt();
extern "C" void asm_disable_interrupt();

void set8259A();
void enableTimeInterrupt();
void disableTimeInterrupt();
bool getInterruptStatus();
void setInterruptStatus(bool status);

#endif