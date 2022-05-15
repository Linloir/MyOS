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
void print(char ch);
void print(char ch, uint8 color);
void print(uint8 row, uint8 col, char ch);
void print(uint8 row, uint8 col, char ch, uint8 color);
void print(const char* string);
void print(const char* string, uint8 color);
void print(const char* string, uint8 row, uint8 col);
void print(const char* string, uint8 row, uint8 col, uint8 color);
void screenScrollUp();
void clearScreen();

#endif