/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:24:55
 * LastEditTime : 2022-06-08 23:15:19
 * Description  : 
 */

#include "process.h"
#include "paging.h"
#include "framemanager.h"
#include "swap.h"


ProcessData::ProcessData(
    uint32 dataStart, 
    uint32 dataMaxSize, 
    uint32 stackStart, 
    uint32 stackMaxSize
) : _dataStartAddr(dataStart), _dataMaxSize(dataMaxSize),
    _stackStartAddr(stackStart), _stackMaxSize(stackMaxSize) {}

Process::Process(
    uint32 pid,
    ProcessPriviledge priviledge,
    PageTable* table,
    uint32 stack,
    ProcessData data,
    Process* parent,
    uint32 ticks
) : _pid(pid), _priviledge(priviledge), _table(table), _stack(stack), _data(data),
    _parent(parent), _ticks(ticks), _status(ProcessStatus::NEW) 
{
    _remainingTicks = ticks;
}


PageTable* Process::pageTable() {
    return _table;
}

ProcessPriviledge Process::priviledge() {
    return _priviledge;
}

uint32 Process::remainingTicks() {
    return _remainingTicks;
}

bool Process::isData(uint32 addr) {
    return addr & START_ADDR_MASK(_data._dataMaxSize) == _data._dataStartAddr;
}

bool Process::isStack(uint32 addr) {
    return addr & START_ADDR_MASK(_data._stackMaxSize) == _data._stackStartAddr;
}

void Process::tickOnce() {
    _remainingTicks = _remainingTicks == 0 ? 0 : _remainingTicks - 1;
}

void Process::resetTick() {
    _remainingTicks = _ticks;
}

void Process::addChild(Process* child) {
    _children.pushBack(child);
}

ProcessStatus Process::status() {
    return _status;
}

void Process::setStatus(ProcessStatus status) {
    _status = status;
}