/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:24:46
 * LastEditTime : 2022-06-17 09:40:34
 * Description  : Process Control Block
 */

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "systemtype.h"
#include "page.h"
#include "vector.h"

enum class ProcessStatus {
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    DEAD,
};

enum class ProcessPriviledge {
    KERNEL = 0,
    USER = 3,
};

class ProcessState {
    public:
        uint32 _cr3 = 0x0;
        uint32 _eax = 0x0;
        uint32 _ebx = 0x0;
        uint32 _ecx = 0x0;
        uint32 _edx = 0x0;
        uint32 _ebp = 0x0;
        uint32 _edi = 0x0;
        uint32 _esi = 0x0;
        uint32 _ds = 0x0;
        uint32 _es = 0x0;
        uint32 _fs = 0x0;
        uint32 _gs = 0x0;
        uint32 _stack = 0x0;
        uint32 _eip = 0x0;
        uint32 _cs = 0x0;
        uint32 _eflags = 0x0;
        uint32 _esp = 0x0;
        uint32 _ss = 0x0;
};

class ProcessSegment {
    private:
        uint32 _startAddr = 0x0;
        uint32 _endAddr = 0x0;
    public:
        static ProcessSegment kernelDataSegment;
        static ProcessSegment kernelStackSegment;
        static ProcessSegment userDataSegment;
        static ProcessSegment userStackSegment;
        static ProcessSegment ESP0Segment;
        ProcessSegment() {}
        ProcessSegment(uint32 start, uint32 end);
        bool includeAddr(uint32 addr);
        uint32 sizeOfPages();
        uint32 sizeOfBytes();
        uint32 endAddr();
        uint32 startAddr();
        Vec<Page> toPages();
        Vec<Page> toPages(int offset);
};

class Process {
    friend class ProcessManager;

    private:
        uint32 _pid;
        ProcessPriviledge _priviledge;
        PageTable* _table;
        ProcessState _state;

        ProcessSegment _dataSegment;
        ProcessSegment _stackSegment;
        ProcessSegment _usedStackSegment;

        Process* _parent;
        Vec<Process*> _children;

        uint32 _ticks;
        uint32 _remainingTicks;
        ProcessStatus _status;
        static Process* _fork(Process* parent);
    public:
        Process() {}
        Process(
            //Process info
            ProcessPriviledge priviledge, 
            ProcessSegment dataSegment,
            Process* parent,
            uint32 ticks,
            uint32 startPoint
        );
        uint32 pid();
        PageTable* pageTable();
        ProcessPriviledge priviledge();
        ProcessStatus status();
        uint32 remainingTicks();
        bool stackIncludeAddr(uint32 addr);
        bool dataIncludeAddr(uint32 addr);
        void tickOnce();
        void resetTick();
        void addChild(Process* child);
        void save(ProcessState* state);
        void restore(ProcessState* state);
};

#endif
