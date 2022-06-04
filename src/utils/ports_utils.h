/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 21:03:12
 * LastEditTime : 2022-06-04 11:47:06
 * Description  : 
 */
#ifndef _PORTS_UTILS_H_
#define _PORTS_UTILS_H_

#include "os_type.h"

extern "C" void asm_ports_write(uint16 port, uint8 value);
extern "C" uint8 asm_ports_read(uint16 port);

#endif