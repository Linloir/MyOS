/*** 
 * Author       : Linloir
 * Date         : 2022-06-17 12:38:45
 * LastEditTime : 2022-06-17 18:06:36
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
        userlib::printf("[Child] Forking another child..\n");
        childPid = syscall_fork();
        if(childPid == 0) {
            userlib::printf("[Child2] Forked from child, pid = %d\n", syscall_pid());
            userlib::printf("[Child2] Returning without using syscall..\n");
            return;
        }
        else {
            userlib::printf("[Child] Forked child, pid = %d\n", childPid);
            userlib::printf("[Child] Calling wait syscall..\n");
            int ret = -1;
            uint32 pid = syscall_wait(&ret);
            userlib::printf("[Child] Received return val %d from pid %d\n", ret, pid);
        }
        userlib::printf("[Child] Returning by calling syscall with return value 255\n");
        syscall_exit(255);
    }
    else {
        userlib::printf("[Parent] Forked to child, parent pid = %d, child pid = %d\n", syscall_pid(), childPid);
        int ret = 0;
        uint32 pid = syscall_wait(&ret);
        userlib::printf("[Parent] Received return val %d from pid %d\n", ret, pid);
    }
    userlib::printf("[Parent] Making another wait attempt, expect to retrieve -1 pid...\n");
    int ret = 0;
    uint32 pid = syscall_wait(&ret);
    userlib::printf("[Parent] Received return val %d from pid %d\n", ret, pid);

    userlib::printf("[Parent] Testing zombie reclaiming strategy..\n");
    childPid = syscall_fork();
    if(childPid != 0) {
        userlib::printf("[Child3] Forked from parent...\n");
        for(int i = 0; i < 0xFFFFFFF; i++) {}
        userlib::printf("[Child3] Child returning...\n");
        syscall_exit(-1);
    }
    else {
        userlib::printf("[Parent] Parent returning...\n");
        syscall_exit(0);
    }

    while(true) {
        // userlib::printf("process of pid %d\n", syscall_pid());
        // for(int i = 0; i < 0xFFFFFFF; i++) {}
    }
}

void lab8() {
    //DO THIS IN SHELL
    printf("\n");
    printf("[LAB8] =======================================\n");
    printf("- 20303042 Linloir\n");
    printf("==============================================\n");
    Process* newProcess = (Process*)malloc(sizeof(Process));
    *newProcess = Process(
        ProcessPriviledge::USER,
        ProcessSegment(0x0, 0x0),
        ProcessManager::processOfPID(0),
        10,
        (uint32)parent
    );
    ProcessManager::execute(newProcess);
}