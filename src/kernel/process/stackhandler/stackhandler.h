/*** 
 * Author       : Linloir
 * Date         : 2022-06-13 20:36:42
 * LastEditTime : 2022-06-14 10:19:02
 * Description  : 
 */

#ifndef _STACK_HANDLER_H_
#define _STACK_HANDLER_H_

#include "os_type.h"

class StackHandler {
    friend class Process;

    private:
        uint32 _top;
    public:
        StackHandler(uint32 stackTop): _top(stackTop) {}

        template<typename T>
        void push(T val);
        
        template<typename T>
        T pop();

        uint32 top();
};

template<typename T>
T StackHandler::pop() {
    T val = *(T*)_top;
    _top += sizeof(T);
    return val;
}

template<typename T>
void StackHandler::push(T val) {
    _top -= sizeof(T);
    *(T*)_top = val;
}

#endif
