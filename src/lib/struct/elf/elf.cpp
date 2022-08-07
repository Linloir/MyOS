/*** 
 * Author       : Linloir
 * Date         : 2022-08-07 14:02:27
 * LastEditTime : 2022-08-07 22:52:06
 * Description  : Elf Structure
 */

#include "./elf.h"

Elf32Header* Elf32Header::from(uint32 addr) {
    return (Elf32Header*)addr;
}

bool Elf32Header::isValid() {
    //In order to verify the validity of the elf file, the file's
    //first 4 bytes should hold the "magic number", which is 0x7F,
    //'E', 'L' and 'F', as well as the EI_CLASS field should be
    //ELFCLASS32.
    return e_ident[EI_MAG0] == 0x7F &&
           e_ident[EI_MAG1] == 'E' &&
           e_ident[EI_MAG2] == 'L' &&
           e_ident[EI_MAG3] == 'F' &&
           e_ident[EI_CLASS] == ELFCLASS32;
}

uint32 Elf32Header::segmentCnt() {
    return e_phnum;
}

Elf32ProgramHeader* Elf32Header::segmentAt(uint32 idx) {
    if(idx >= e_phnum) {
        //Index out of range
        idx = e_phnum - 1;
    }
    uint32 fileBaseAddr = (uint32)this;
    uint32 programHeaderBaseAddr = e_phoff + fileBaseAddr;
    uint32 segAddr = programHeaderBaseAddr + idx * e_phentsize;
    return (Elf32ProgramHeader*)segAddr;
}

uint32 Elf32Header::entry() {
    return e_entry;
}

bool Elf32ProgramHeader::shouldLoad() {
    return p_type == PT_LAOD;
}