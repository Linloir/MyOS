/*** 
 * Author       : Linloir
 * Date         : 2022-06-17 12:38:45
 * LastEditTime : 2022-06-17 16:03:04
 * Description  : 
 */

#include "proc.h"
#include "systemio.h"
#include "processmanager.h"

#include "userio.h"
#include "systemcall.h"

void parent() {
    userlib::printf("[Parent] Parent Process, pid = %d\n", syscall_pid());
    uint32 childPid = syscall_fork();
    if(childPid == 0) {
        userlib::printf("[Child] Forked from parent, pid = %d\n", syscall_pid());
        userlib::printf("[Child] Returning..\n");
        syscall_exit(1);
    }
    else {
        userlib::printf("[Parent] Forked to child, parent pid = %d, child pid = %d\n", syscall_pid(), childPid);
        int ret = 0;
        uint32 pid = syscall_wait(&ret);
        userlib::printf("[Parent] Received return val %d from pid %d\n", ret, pid);
    }

    while(true) {
        // userlib::printf("process of pid %d\n", syscall_pid());
        // for(int i = 0; i < 0xFFFFFFF; i++) {}
    }
}

void lab8() {
    //DO THIS IN KERNEL MODE
    printf("\n");
    printf("[LAB8] =======================================\n");
    printf("- 20303042 Linloir\n");
    printf("==============================================\n");
    Process* newProcess = (Process*)malloc(sizeof(Process));
    *newProcess = Process(
        ProcessPriviledge::KERNEL,
        ProcessSegment(0x0, 0x0),
        ProcessManager::processOfPID(0),
        30,
        (uint32)parent
    );
    ProcessManager::execute(newProcess);
}