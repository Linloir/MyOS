/*** 
 * Author       : Linloir
 * Date         : 2022-06-13 11:27:29
 * LastEditTime : 2022-06-13 11:38:09
 * Description  : 
 */

#ifndef _TSS_H_
#define _TSS_H_

#include "os_type.h"

class TaskStateSegment {
    uint16 _link;
    uint16 _reserved_0;

    uint32 _esp0;
    uint16 _ss0;
    uint16 _reserved_1;

    uint32 _esp1;
    uint16 _ss1;
    uint16 _reserved_2;

    uint32 _esp2;
    uint16 _ss2;
    uint16 _reserved_3;

    uint32 _cr3;
    uint32 _eip;
    uint32 _eflags;

    uint32 _eax;
    uint32 _ecx;
    uint32 _edx;
    uint32 _ebx;

    uint32 _esp;
    uint32 _ebp;
    uint32 _esi;
    uint32 _edi;

    uint16 _es;
    uint16 _reserved_4;
    uint16 _cs;
    uint16 _reserved_5;
    uint16 _ss;
    uint16 _reserved_6;
    uint16 _ds;
    uint16 _reserved_7;
    uint16 _fs;
    uint16 _reserved_8;
    uint16 _gs;
    uint16 _reserved_9;

    uint16 _ldtr;
    uint16 _reserved_10;

    uint16 _reserved_11;
    uint16 _iobp;
    
    uint32 _ssp;
};

class TSSDescriptor {
    private:
        uint64 val;
    public:
        TSSDescriptor(TaskStateSegment* tss, )
};

#endif
