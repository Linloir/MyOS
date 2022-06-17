/*** 
 * Author       : Linloir
 * Date         : 2022-06-17 11:25:18
 * LastEditTime : 2022-06-17 12:09:25
 * Description  : 
 */

#include "ports.h"

void writePort(uint16 port, uint8 val) {
    asm(
        "movw %[port], %%ax\n\t"
        "movw %%ax, %%dx\n\t"
        "movb %[val], %%al\n\t"
        "out %%al, %%dx\n\t"
        : 
        : [port]"r"(port), [val]"r"(val)
    );
}

byte readPort(uint16 port) {
    byte val = 0;
    asm(
        "movw %[port], %%dx\n\t"
        "in %%dx, %[val]\n\t"
        : [val]"=r"(val)
        : [port]"r"(port)  
    );
    return val;
}