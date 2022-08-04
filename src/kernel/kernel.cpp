/*** 
 * Author       : Linloir
 * Date         : 2022-08-04 11:10:39
 * LastEditTime : 2022-08-04 12:05:10
 * Description  : Kernel
 */

#include "kernel.h"
#include "vga.h"

extern "C" void kernel() {
    VGA scrn(0xB8000, 80, 25);
    scrn.clearScreen();
    scrn.printString("Enter kernel!");
    while(true) {}
}
