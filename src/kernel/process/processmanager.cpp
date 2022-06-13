/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:29:47
 * LastEditTime : 2022-06-12 15:30:55
 * Description  : 
 */

// #include "processmanager.h"

// BitMap ProcessManager::_pids;
// Vec<Process*> ProcessManager::_readyProcesses;
// Process* ProcessManager::_curProcess;

// void ProcessManager::_processExit() {
//     _curProcess->setStatus(ProcessStatus::DEAD);
//     _forceSchedule();
// }

// void ProcessManager::_forceSchedule() {

// }

// void ProcessManager::initialize() {
//     _pids = BitMap(256);
//     _readyProcesses = Vec<Process*>(256);
//     _curProcess = nullptr;

//     _pids.set(0, true);
    
//     Process* kernelProcess = (Process*)malloc(sizeof(Process));
//     *kernelProcess = Process(
//         0,
//         ProcessPriviledge::KERNEL,
        
//     )
// }

// void ProcessManager::schedule() {
//     if(_curProcess->remainingTicks() > 0) {
//         _curProcess->tickOnce();
//         return;
//     }
//     else {
//         _curProcess->resetTick();
//         _forceSchedule();
//     }
// }

// Process* ProcessManager::curProcess() {
//     return _curProcess;
// }

// void ProcessManager::executeProcess(uint32 loadAddr, uint32 dataSize, uint32 priority) {

// }