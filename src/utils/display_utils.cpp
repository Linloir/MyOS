#include "display_utils.h"

//Reference: [TextModeCursor](https://wiki.osdev.org/Text_Mode_Cursor)

void enableCursor(uint8 strtLine, uint8 endLine) {
    uint8 tmp;
    asm_ports_write(0x03D4, 0x0A);
    tmp = asm_ports_read(0x03D5);
    tmp = (tmp & 0xC0) | strtLine;
    asm_ports_write(0x03D5, tmp);
    asm_ports_write(0x03D4, 0x0B);
    tmp = asm_ports_read(0x03D5);
    tmp = (tmp & 0xE0) | endLine;
    asm_ports_write(0x03D5, tmp);
}

void disableCursor() {
    asm_ports_write(0x03D4, 0x0A);
    asm_ports_write(0x03D5, 0x20);
}

uint16 getCursor() {
    uint16 position = 0;
    asm_ports_write(0x03D4, 0x0E);
    position |= (uint16)(asm_ports_read(0x03D5)) << 8;
    asm_ports_write(0x03D4, 0x0F);
    position |= asm_ports_read(0x03D5);
    return position;
}

void setCursor(uint8 row, uint8 col) {
    if(col >= 80) {
        return;
    }
    while(row >= 25) {
        screenScrollUp();
        --row;
    }
    uint16 pos = row * VGA_WIDTH + col;
    asm_ports_write(0x03D4, 0x0E);
    asm_ports_write(0x03D5, (uint8)((pos >> 8) & 0xFF));
    asm_ports_write(0x03D4, 0x0F);
    asm_ports_write(0x03D5, (uint8)(pos & 0xFF));
}

void setCursor(uint16 position) {
    uint16 maxPosition = VGA_WIDTH * VGA_HEIGHT;
    while(position >= maxPosition) {
        position -= VGA_WIDTH;
        screenScrollUp();
    }
    asm_ports_write(0x03D4, 0x0E);
    asm_ports_write(0x03D5, (uint8)((position >> 8) & 0xFF));
    asm_ports_write(0x03D4, 0x0F);
    asm_ports_write(0x03D5, (uint8)(position & 0xFF));
}

int print(char ch) {
    uint16 position = getCursor();
    if(ch == '\n') {
        setCursor(position / VGA_WIDTH + 1, 0);
        return 1;
    }
    uint8 color = 0x0F;
    uint16 data = 0;
    data |= (uint16)(ch & 0xFF);
    data |= ((uint16)color << 8) & 0xFF00;
    screen[position] = data;
    setCursor(position + 1);
    return 1;
}

int print(char ch, uint8 color) {
    uint16 position = getCursor();
    if(ch == '\n') {
        setCursor(position / VGA_WIDTH + 1, 0);
        return 1;
    }
    uint16 data = 0;
    data |= (uint16)(ch & 0xFF);
    data |= ((uint16)color << 8) & 0xFF00;
    screen[position] = data;
    setCursor(position + 1);
    return 1;
}

int print(char ch, uint8 row, uint8 col) {
    setCursor(row, col);
    return print(ch);
}

int print(char ch, uint8 row, uint8 col, uint8 color) {
    setCursor(row, col);
    return print(ch, color);
}

int print(const char* string) {
    int count = 0;
    for(int i = 0; string[i]; i++) {
        print(string[i]);
        count++;
    }
    return count;
}

int print(const char* string, uint8 color) {
    int count = 0;
    for(int i = 0; string[i]; i++) {
        print(string[i], color);
        count++;
    }
    return count;
}

int print(const char* string, uint8 row, uint8 col) {
    setCursor(row, col);
    return print(string);
}

int print(const char* string, uint8 row, uint8 col, uint8 color) {
    setCursor(row, col);
    return print(string, color);
}

void screenScrollUp() {
    uint16 position = 0;
    for(int i = 0; i < VGA_HEIGHT - 1; i++) {
        for(int j = 0; j < VGA_WIDTH; j++) {
            screen[position] = screen[position + VGA_WIDTH];
            position += 1;
        }
    }
    uint8 fillColor = 0x0F;
    uint8 emptyChar = ' ';
    uint16 data = 0x0000;
    data |= (uint16)emptyChar & 0xFF;
    data |= ((uint16)fillColor << 8) & 0xFF00;
    for(int i = 0; i < VGA_WIDTH; i++) {
        screen[position] = data;
        position += 1;
    }
}

void clearScreen() {
    uint16 position = 0;
    for(; position < VGA_HEIGHT * VGA_WIDTH; position++) {
        setCursor(position);
        print(' ');
    }
    setCursor(0);
}
