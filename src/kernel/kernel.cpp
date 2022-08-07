/*** 
 * Author       : Linloir
 * Date         : 2022-08-04 11:10:39
 * LastEditTime : 2022-08-07 22:52:51
 * Description  : Kernel
 */

#include "kernel.h"
#include "vga.h"
#include "serialport.h"

extern "C" void kernel(BootInfo& bootInfo) {
    VGA scrn(0xB8000 + bootInfo.memoryMappingOffset, 80, 25);
    scrn.clearScreen();
    scrn.printString("Enter kernel!");
    SerialPort uart(0x3F8);
    uart.init();
    uart.send("\n");
    uart.send("Hello world!\n");
    while(true) {}
}
