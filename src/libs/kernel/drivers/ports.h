/*** 
 * Author       : Linloir
 * Date         : 2022-06-17 11:22:57
 * LastEditTime : 2022-06-17 11:29:49
 * Description  : 
 */

#ifndef _PORTS_H_
#define _PORTS_H_

#include "systemtype.h"

void writePort(uint16 port, uint8 val);
byte readPort(uint16 port);

#endif