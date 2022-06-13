/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:29:38
 * LastEditTime : 2022-06-13 22:02:13
 * Description  : 
 */

#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "os_type.h"
#include "bitmap.h"
#include "vector.h"
#include "process.h"

class ProcessManager {
    private:
        static BitMap _pids;
        static Vec<Process*> _allProcesses;
        static Vec<Process*> _readyProcesses;
        static Process* _curProcess;
        static void _schedule();
        static void _switchProcess(Process* cur, Process* next);
    public:
        static __attribute__((interrupt)) void _processStart(void*);
        static void _processExit();
        static void initialize();
        static void onTimeTick();
        static Process* curProcess();
        static Process* processOfPID(uint32 pid);
        static uint32 allocPID();
        static void executeProcess(Process* process);
};

#endif