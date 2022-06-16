/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:24:46
 * LastEditTime : 2022-06-16 17:37:54
 * Description  : Process Control Block
 */

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "os_type.h"
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
    friend class Process;

    private:
        uint32 _eax;
        uint32 _ebx;
        uint32 _ecx;
        uint32 _edx;
        uint32 _ebp;
        uint32 _edi;
        uint32 _esi;
        uint32 _ds;
        uint32 _es;
        uint32 _fs;
        uint32 _gs;
        uint32 _eip;
        uint32 _cs;
        uint32 _eflags;
        uint32 _ss;
        uint32 _esp;
};

class ProcessSegment {
    private:
        uint32 _startAddr = 0x0;
        uint32 _endAddr = 0x0;
    public:
        static ProcessSegment defaultKernelDataSegment();
        static ProcessSegment defaultUserDataSegment();
        static ProcessSegment defaultStackSegment();
        static ProcessSegment defaultESP0Segment();
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
        uint32 _esp;

        ProcessSegment _dataSegment;
        ProcessSegment _stackSegment;
        //Refreshed by page fault
        ProcessSegment _usedStackSegment;
        //For user mode process only
        ProcessSegment _esp0Segment;

        Process* _parent;
        Vec<Process*> _children;

        uint32 _ticks;
        uint32 _remainingTicks;
        ProcessStatus _status;
        static Process* _fork(Process* parent, uint32 stackTop);
    public:
        Process() {}
        Process(
            //Process info
            ProcessPriviledge priviledge, 
            ProcessSegment dataSegment,
            Process* parent,
            uint32 ticks,
            //Execute info
            uint32 entryPoint
        );
        uint32 pid();
        PageTable* pageTable();
        ProcessPriviledge priviledge();
        ProcessStatus status();
        uint32 esp();
        uint32 esp0();
        uint32 remainingTicks();
        bool stackIncludeAddr(uint32 addr);
        bool dataIncludeAddr(uint32 addr);
        bool esp0IncludeAddr(uint32 addr);
        void tickOnce();
        void resetTick();
        void addChild(Process* child);
        void setEsp(uint32 esp);
        void setStatus(ProcessStatus status);
};

#endif
