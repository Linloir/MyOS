/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:29:38
 * LastEditTime : 2022-06-16 20:53:56
 * Description  : 
 */

#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "os_type.h"
#include "bitmap.h"
#include "vector.h"
#include "proc.h"
#include "interruptframe.h"

class Process;

class ProcessManager {
    private:
        static BitMap _pids;
        static Vec<Process*> _allProcesses;
        static Vec<Process*> _readyProcesses;
        static Vec<Process*> _awaitProcesses;
        static Process* _curProcess;
        static void _schedule(ProcessState* state);
    public:
        static void _processExit();
        static void initialize();
        static void schedule(ProcessState* state);
        static Process* current();
        static Process* processOfPID(uint32 pid);
        static void execute(Process* process);
        static void sleep(ProcessState* state);
        static void awake(uint32 pid, ProcessState* state);
        static uint32 fork(ProcessState* state);
};

#endif