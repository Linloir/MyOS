/*** 
 * Author       : Linloir
 * Date         : 2022-06-17 11:01:14
 * LastEditTime : 2022-06-17 11:56:55
 * Description  : 
 */

#ifndef _USER_DISPLAY_H_
#define _USER_DISPLAY_H_

#include "systemtype.h"
#include "mmu.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

namespace userlib {
    void enableCursor();
    void disableCursor();
    uint16 getCursor();
    void setCursor(uint8 row, uint8 col);
    void setCursor(uint16 position);
    int print(char ch);
    int print(char ch, uint8 color);
    int print(uint8 row, uint8 col, char ch);
    int print(uint8 row, uint8 col, char ch, uint8 color);
    int print(const char* string);
    int print(const char* string, uint8 color);
    int print(const char* string, uint8 row, uint8 col);
    int print(const char* string, uint8 row, uint8 col, uint8 color);
    void screenScrollUp();
    void clearScreen();
}

#endif