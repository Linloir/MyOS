/*** 
 * Author       : Linloir
 * Date         : 2022-06-12 21:31:33
 * LastEditTime : 2022-06-16 17:32:25
 * Description  : 
 */

#ifndef _INTERRUPT_FRAME_H_
#define _INTERRUPT_FRAME_H_

#include "systemtype.h"
#include "eflags.h"

class InterruptFrame {
    private:
        uint32 _eip;
        uint32 _codeSelector;
        uint32 _eflags;
};

#endif
