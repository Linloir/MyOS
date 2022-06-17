/*** 
 * Author       : Linloir
 * Date         : 2022-06-16 21:11:21
 * LastEditTime : 2022-06-17 00:13:38
 * Description  : 
 */

#include "syscallhandler.h"
#include "std_utils.h"
#include "processmanager.h"

void testSyscall(ProcessState* state, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4) {
    printf("%d, %d, %d, %d, %d\n", p0, p1, p2, p3, p4);
    state->_eax = p0 + p1 + p2 + p3 + p4;
}

void __syscall_fork__(ProcessState* state, uint32, uint32, uint32, uint32, uint32) {
    ProcessManager::fork(state);
}
