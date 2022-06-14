/*** 
 * Author       : Linloir
 * Date         : 2022-06-07 14:01:56
 * LastEditTime : 2022-06-08 09:28:01
 * Description  : 
 */

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "vector.h"

template<typename T>
void qsort(Vec<T> arr, bool (*cmp)(T& a, T& b));

template<typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
int _partition(Vec<T> arr, int start, int end, bool (*cmp)(T& a, T& b)) {
    T pivot = arr[start];

    int count = 0;
    for(int i = start + 1; i <= end; i++) {
        if(cmp(arr[i], pivot)) {
            count++;
        }
    }

    int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);

    int i = start, j = end;
    while (i < pivotIndex && j > pivotIndex) {
 
        while (cmp(arr[i], pivot)) {
            i++;
        }
 
        while (cmp(arr[j], pivot)) {
            j--;
        }
 
        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }

    return pivotIndex;
}

template<typename T>
void qsort(Vec<T> arr, int start, int end, bool (*cmp)(T& a, T& b)) {
    if(arr.isEmpty())  return;

    int p = _partition(arr, start, end, cmp);

    qsort(arr, start, p - 1, cmp);
    qsort(arr, p + 1, end, cmp);
}

#endif