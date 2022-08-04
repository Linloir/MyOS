/*** 
 * Author       : Linloir
 * Date         : 2022-08-04 11:14:15
 * LastEditTime : 2022-08-04 12:18:43
 * Description  : VGA driver
 */

#include "vga.h"

VGAChar::VGAChar(uint8 chr) :
_character(chr),
_color(0x0F) {}

VGAChar::VGAChar(uint8 chr, VGA_COLOR fgc, VGA_COLOR bgc) :
_character(chr),
_color((bgc << 4) | (fgc & 0x0F)) {}

VGA_COLOR VGAChar::foregroundColor() {
    return static_cast<VGA_COLOR>(_color & 0x0F);
}

VGA_COLOR VGAChar::backgroundColor() {
    return static_cast<VGA_COLOR>(_color >> 4);
}

void VGAChar::setColor(VGA_COLOR foreground, VGA_COLOR background) {
    _color = (background << 4) | (foreground & 0x0F);
}

uint8 VGAChar::character() {
    return _character;
}

void VGAChar::setCharacter(uint8 chr) {
    _character = chr;
}

VGA::VGA(uint32 displayMatrixAddr, uint32 width, uint32 height) :
_displayMatrix((VGAChar*)displayMatrixAddr),
_displayWidth(width),
_displayHeight(height) {}

void VGA::enableTextCursor(uint8 strtScanLine, uint8 endScanLine) {
    uint8 tmp;
    _cmdPort.write(0x0A);
    tmp = _dataPort.read();
    tmp = (tmp & 0xC0) | strtScanLine;
    _dataPort.write(tmp);
    _cmdPort.write(0x0B);
    tmp = _dataPort.read();
    tmp = (tmp & 0xE0) | endScanLine;
    _dataPort.write(tmp);
}

void VGA::disableTextCursor() {
    _cmdPort.write(0x0A);
    _dataPort.write(0x20);
}

uint16 VGA::getTextCursor() {
    uint16 position = 0;
    _cmdPort.write(0x0E);
    position |= ((uint16)_dataPort.read() << 8);
    _cmdPort.write(0x0F);
    position |= _dataPort.read();
    return position;
}

void VGA::setTextCursor(uint8 row, uint8 col) {
    if(col >= _displayWidth) {
        return;
    }
    while(row >= _displayHeight) {
        scrollUp();
        row -= 1;
    }
    uint16 pos = row * _displayWidth + col;
    _cmdPort.write(0x0E);
    _dataPort.write((uint8)(pos >> 8) & 0xFF);
    _cmdPort.write(0x0F);
    _dataPort.write((uint8)(pos & 0xFF));
}

void VGA::setTextCursor(uint16 position) {
    uint16 maxPos = _displayHeight * _displayWidth;
    while(position >= maxPos) {
        scrollUp();
        position -= _displayWidth;
    }
    _cmdPort.write(0x0E);
    _dataPort.write((uint8)(position >> 8) & 0xFF);
    _cmdPort.write(0x0F);
    _dataPort.write((uint8)(position & 0xFF));
}

void VGA::printCharacter(VGAChar chr) {
    uint16 position = getTextCursor();
    if(chr.character() == '\n') {
        setTextCursor(position / _displayWidth + 1, 0);
    }
    _displayMatrix[position] = chr;
    setTextCursor(position + 1);
}

void VGA::printCharacter(VGAChar chr, uint8 row, uint8 col) {
    setTextCursor(row, col);
    printCharacter(chr);
}

int VGA::printString(const char* string) {
    int cnt = 0;
    for(int i = 0; string[i]; i++) {
        printCharacter(string[i]);
        cnt++;
    }
    return cnt;
}

int VGA::printString(const char* string, uint8 row, uint8 col) {
    setTextCursor(row, col);
    return printString(string);
}

int VGA::printString(const char* string, VGA_COLOR fgcolor, VGA_COLOR bgcolor) {
    int cnt = 0;
    for(int i = 0; string[i]; i++) {
        printCharacter(VGAChar(string[i], fgcolor, bgcolor));
        cnt++;
    }
    return cnt;
}

int VGA::printString(const char* string, uint8 row, uint8 col, VGA_COLOR fgcolor, VGA_COLOR bgcolor) {
    setTextCursor(row, col);
    return printString(string, fgcolor, bgcolor);
}

void VGA::scrollUp() {
    uint16 position = 0;
    for(int i = 0; i < _displayHeight - 1; i++) {
        for(int j = 0; j < _displayWidth; j++) {
            _displayMatrix[position] = _displayMatrix[position + _displayWidth];
            position += 1;
        }
    }
    for(int i = 0; i < _displayWidth; i++) {
        _displayMatrix[position] = ' ';
        position += 1;
    }
}

void VGA::clearScreen() {
    for(int i = 0; i < _displayHeight * _displayWidth; i++) {
        _displayMatrix[i] = ' ';
    }
    setTextCursor(0);
}