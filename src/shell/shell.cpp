/*** 
 * Author       : Linloir
 * Date         : 2022-06-17 16:10:43
 * LastEditTime : 2022-06-17 18:07:22
 * Description  : 
 */

#include "shell.h"
#include "systemcall.h"
#include "allocator.h"
#include "processmanager.h"
#include "lab8.h"
#include "userio.h"

void shell() {
    userlib::printf("[SHELL] Shell Initialized!");
    lab8();
    while(true){}
}

void Shell::initialize() {
    Process* shellProc = (Process*)malloc(sizeof(Process));
    *shellProc = Process(
        ProcessPriviledge::KERNEL,
        ProcessSegment(0x0, 0x0),
        ProcessManager::processOfPID(0),
        10,
        (uint32)shell
    );
    ProcessManager::execute(shellProc);
}