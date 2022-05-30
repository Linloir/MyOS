/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:21
 * LastEditTime : 2022-05-17 20:46:10
 * Description  : 
 */
#ifndef _DISPLAY_UTILS_H_
#define _DISPLAY_UTILS_H_

#include "os_type.h"
#include "ports_utils.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

uint16* const screen = (uint16*)(0xB8000);

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

#endif