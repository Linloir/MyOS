/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:29:38
 * LastEditTime : 2022-06-16 10:42:18
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
        static void _schedule();
        static void _switchProcess(Process* cur, Process* next);
        static void _hasenAwakeProcess(Process* cur);
    public:
        enum AwakeMethod {
            DEFAULT = 1,
            HASEN = 1,
        };
        __attribute__ ((interrupt)) static void _processStart(InterruptFrame* f);
        static void _processExit();
        static void initialize();
        static void onTimeTick();
        static Process* curProcess();
        static Process* processOfPID(uint32 pid);
        static uint32 allocPID();
        static void executeProcess(Process* process);
        static void haltProcess(Process* process);
        static void awakeProcess(Process* process, AwakeMethod method);
};

extern "C" void asm_switch_process(uint32* curEsp, uint32 cr3, uint32 nextEsp);

#endif