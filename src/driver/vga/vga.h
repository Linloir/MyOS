/*** 
 * Author       : Linloir
 * Date         : 2022-07-26 21:39:22
 * LastEditTime : 2022-08-04 15:14:59
 * Description  : VGA Interface
 */

#include "types.h"
#include "ports.h"

#ifndef _VGA_H_
#define _VGA_H_

enum VGA_COLOR {
    BLACK           = 0x0,
    BLUE            = 0x1,
    GREEN           = 0x2,
    CYAN            = 0x3,
    RED             = 0x4,
    MEGENTA         = 0x5,
    BROWN           = 0x6,
    WHITE           = 0x7,
    BRIGHT_FLASH    = 0x8
};

class VGAChar {
    public:
        VGAChar(uint8 chr);
        VGAChar(uint8 chr, VGA_COLOR fgc, VGA_COLOR bgc);
    private:
        uint8 _character;
        uint8 _color;
    public:
        VGA_COLOR foregroundColor();
        VGA_COLOR backgroundColor();
        void setColor(VGA_COLOR foreground, VGA_COLOR background);
        uint8 character();
        void setCharacter(uint8 chr);
};

class VGA {
    public:
        VGA(uint32 displayMatrixAddr, uint32 width, uint32 height);
    private:
        VGAChar* _displayMatrix;
        uint32 _displayWidth;
        uint32 _displayHeight;
        const PortWriteOnly<uint8> _cmdPort = 0x3D4;
        const Port<uint8> _dataPort = 0x3D5;
    public:
        void enableTextCursor(uint8 strtScanLine, uint8 endScanLine);
        void disableTextCursor();
        uint16 getTextCursor();
        void setTextCursor(uint8 row, uint8 col);
        void setTextCursor(uint16 pos);
        void printCharacter(VGAChar chr);
        void printCharacter(VGAChar chr, uint8 row, uint8 col);
        int printString(const char* string);
        int printString(const char* string, uint8 row, uint8 col);
        int printString(const char* string, VGA_COLOR fgcolor, VGA_COLOR bgcolor);
        int printString(const char* string, uint8 row, uint8 col, VGA_COLOR fgcolor, VGA_COLOR bgcolor);
        void scrollUp();
        void clearScreen();
};

#endif