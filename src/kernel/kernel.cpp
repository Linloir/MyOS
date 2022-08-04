/*** 
 * Author       : Linloir
 * Date         : 2022-08-04 11:10:39
 * LastEditTime : 2022-08-04 17:30:20
 * Description  : Kernel
 */

#include "kernel.h"
#include "vga.h"
#include "serialport.h"

extern "C" void kernel() {
    VGA scrn(0xB8000, 80, 25);
    scrn.clearScreen();
    scrn.printString("Enter kernel!");
    SerialPort uart(0x3F8);
    uart.init();
    char message[19] = "Hello\nworld!\n\r";
    for(int i = 0; i < 18; i++) {
        uart.send(message[i]);
    }
    while(true) {}
}
