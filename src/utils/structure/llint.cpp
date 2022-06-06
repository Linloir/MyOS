/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:15:47
 * LastEditTime : 2022-06-06 16:22:38
 * Description  : 
 */

#include "llist.h"
#include "allocator.h"

template<typename T>
struct LinkedListNode {
    T val;
    LinkedListNode<T>* next;
};