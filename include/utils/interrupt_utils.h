#ifndef _INTERRUPT_UTILS_H_
#define _INTERRUPT_UTILS_H_

#include "os_type.h"

void asm_load_idtr(uint32* tableAddr, uint16 tableLimit);
int asm_interrupt_status();
void asm_enable_interrupt();
void asm_disable_interrupt();
void asm_empty_interrupt_handler();
void asm_time_interrupt_handler();

#endif