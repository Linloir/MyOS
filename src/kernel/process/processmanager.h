/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:29:38
 * LastEditTime : 2022-06-08 23:10:51
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
        static Vec<Process*> _readyProcesses;
        static Process* _curProcess;
        static void _processExit();
        static void _forceSchedule();
    public:
        static void initialize();
        static void schedule();
        static Process* curProcess();
        static void executeProcess(uint32 loadAddr, uint32 dataSize, uint32 priority);
};

#endif