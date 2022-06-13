/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:24:46
 * LastEditTime : 2022-06-12 20:01:58
 * Description  : Process Control Block
 */

#ifndef _PCB_H_
#define _PCB_H_

#include "os_type.h"
#include "page.h"

#define START_ADDR_MASK(MAXSIZE) (!((MAXSIZE) - 1))

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

class ProcessData {
    friend class Process;

    private:
        uint32 _dataStartAddr;
        uint32 _dataMaxSize;
        uint32 _stackStartAddr;
        uint32 _stackMaxSize;
    public:
        ProcessData(uint32 dataStart, uint32 dataMaxSize, uint32 stackStart, uint32 stackMaxSize);
};

class Process {
    private:
        uint32 _pid;
        ProcessPriviledge _priviledge;
        PageTable* _table;
        uint32 _stack;
        ProcessData _data;

        Process* _parent;
        Vec<Process*> _children;

        uint32 _ticks;
        uint32 _remainingTicks;
        ProcessStatus _status;
    public:
        Process(uint32 pid, ProcessPriviledge priviledge, PageTable* table, uint32 stack, ProcessData data, Process* parent, uint32 ticks);
        PageTable* pageTable();
        ProcessPriviledge priviledge();
        uint32 remainingTicks();
        bool isData(uint32 addr);
        bool isStack(uint32 addr);
        void tickOnce();
        void resetTick();
        void addChild(Process* child);
        ProcessStatus status();
        void setStatus(ProcessStatus status);
};

#endif
