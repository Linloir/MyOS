#ifndef _INTERRUPT_UTILS_H_
#define _INTERRUPT_UTILS_H_

#include "os_type.h"

extern "C" void asm_load_idtr(uint32* tableAddr, uint16 tableLimit);
extern "C" int asm_interrupt_status();
extern "C" void asm_enable_interrupt();
extern "C" void asm_disable_interrupt();
extern "C" void asm_empty_interrupt_handler();
extern "C" void asm_time_interrupt_handler();

#endif