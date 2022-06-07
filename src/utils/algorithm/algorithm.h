/*** 
 * Author       : Linloir
 * Date         : 2022-06-07 14:01:56
 * LastEditTime : 2022-06-07 14:22:28
 * Description  : 
 */

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "vector.h"
#include "linkedlist.h"

template<typename T>
void qsort(Vec<T> arr, bool (*cmp)(T& a, T& b));

#endif