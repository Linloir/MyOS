/*** 
 * Author       : Linloir
 * Date         : 2022-06-07 14:01:59
 * LastEditTime : 2022-06-07 14:22:39
 * Description  : 
 */

#include "algorithm.h"

template<typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
void _partition(Vec<T> arr, int start, int end, bool (*cmp)(T& a, T& b)) {
    int pivot = arr[start];

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
void qsort(Vec<T> arr, bool (*cmp)(T& a, T& b)) {
    if(arr.isEmpty())  return;

    int start = 0;
    int end = arr.size() - 1;

    int p = _partition(arr, start, end, cmp);

    qsort(arr, start, p - 1);
    qsort(arr, p + 1, end);
}