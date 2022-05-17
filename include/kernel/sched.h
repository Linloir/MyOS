/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 11:47:19
 * LastEditTime : 2022-05-17 15:34:14
 * Description  : 
 */
#ifndef _SCHED_H_
#define _SCHED_H_

#include "os_constant.h"
#include "os_type.h"
#include "structure.h"

typedef enum JobStatus {
    NEW,
    READY,
    RUNNING,
    DEAD
} JobStatus;

struct PCB {
    uint32*     stack;
    uint32      pID;
    uint32      tID;
    uint32      priority;
    JobStatus   status;
    uint32      tickRemaining;
    /* Used for pcb lists, will delete after malloc is implemented */
    ListNode    readyTaskListNode;
};

PCB* nodeToPCB(ListNode* node);

PCB* allocPCB();
void freePCB(PCB* process);

class Scheduler {
private:
    static const int timeQuantum = 100;
    static List readyTaskList;
    static PCB* runningThread;
public:
    static void initialize();
    static void onTimeInterrupt();
    static void schedule();
    static void executeThread(void (*function)(), void* parameters, uint32 priority);
    static void threadExit();
};

#endif
