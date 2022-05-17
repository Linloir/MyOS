/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 11:47:19
 * LastEditTime : 2022-05-17 21:28:17
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
    BLOCKED,
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
    ListNode    scheduleListNode;
};

PCB* nodeToPCB(ListNode* node);

PCB* allocPCB();
void freePCB(PCB* process);

class Scheduler {
private:
    static const int timeQuantum = 1;
    static List readyTaskList;
    static PCB* runningThread;
public:
    static void initialize();
    static void onTimeInterrupt();
    static PCB* currentRunningThread();
    static void schedule();
    static void executeThread(void (*function)(), void* parameters, uint32 priority);
    static void awakeThreadMESA(PCB* thread);
    static void awakeThreadHasen(PCB* thread);
    static void awakeThreadHoare(PCB* thread);
    static void threadExit();
};

#endif
