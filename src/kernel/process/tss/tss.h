/*** 
 * Author       : Linloir
 * Date         : 2022-06-13 11:27:29
 * LastEditTime : 2022-06-17 13:15:39
 * Description  : 
 */

#ifndef _TSS_H_
#define _TSS_H_

#include "systemtype.h"

class TaskStateSegment {
    friend class TSSManager;

    private:
        uint32 _previous = 0x0;

        uint32 _esp0 = 0x0;
        uint32 _ss0 = 0x0;

        uint32 _esp1 = 0x0;
        uint32 _ss1 = 0x0;

        uint32 _esp2 = 0x0;
        uint32 _ss2 = 0x0;

        uint32 _cr3 = 0x0;
        uint32 _eip = 0x0;
        uint32 _eflags = 0x0;

        uint32 _eax = 0x0;
        uint32 _ecx = 0x0;
        uint32 _edx = 0x0;
        uint32 _ebx = 0x0;

        uint32 _esp = 0x0;
        uint32 _ebp = 0x0;
        uint32 _esi = 0x0;
        uint32 _edi = 0x0;

        uint32 _es = 0x0; //!!THE VALUE TO LOAD INTO ES WHEN CHANGED TO KERNEL MODE
        uint32 _cs = 0x0; //!!THE VALUE TO LOAD INTO CS WHEN CHANGED TO KERNEL MODE
        uint32 _ss = 0x0; //!!THE VALUE TO LOAD INTO SS WHEN CHANGED TO KERNEL MODE
        uint32 _ds = 0x0; //!!THE VALUE TO LOAD INTO DS WHEN CHANGED TO KERNEL MODE
        uint32 _fs = 0x0; //!!THE VALUE TO LOAD INTO FS WHEN CHANGED TO KERNEL MODE
        uint32 _gs = 0x0; //!!THE VALUE TO LOAD INTO GS WHEN CHANGED TO KERNEL MODE

        uint32 _ldt = 0x0;

        uint32 _iopb = 0x0;
        
        uint32 _ssp = 0x0;
} __attribute__((packed));

class TSSManager {
    private:
        static TaskStateSegment _tss;
        static void _load();
    public:
        static void initialize();
        static uint32 esp0();
        static void setEsp0(uint32 esp0);
};

#endif
