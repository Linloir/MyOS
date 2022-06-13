/*** 
 * Author       : Linloir
 * Date         : 2022-06-12 21:31:33
 * LastEditTime : 2022-06-12 22:18:15
 * Description  : 
 */

#ifndef _INTERRUPT_FRAME_H_
#define _INTERRUPT_FRAME_H_

#include "os_type.h"
#include "eflags.h"

class InterruptFrame {
    friend class StackFrame;

    private:
        uint32 _eip;
        uint32 _codeSelector;
        uint32 _eflags;
        //Exist only when piviledge changed--
        uint32 _esp;
        uint32 _stackSelector;
        //-----------------------------------
};

#endif
