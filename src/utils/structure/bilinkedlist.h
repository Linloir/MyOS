/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 21:54:46
 * LastEditTime : 2022-06-07 23:39:46
 * Description  : 
 */

#ifndef _BI_LINKED_LIST_H_
#define _BI_LINKED_LIST_H_

template<typename T>
struct DualLinkedListNode {
    T _val;
    DualLinkedListNode<T>* _prev;
    DualLinkedListNode<T>* _next;
};

template<typename T>
class DualLinkedList {
    private:
        DualLinkedListNode<T>* _head;
        int _length;
    public:
        DualLinkedList(): _head(nullptr) {}
        ~DualLinkedList();
        void pushFront(T val);
        void popFront();
        void pushBack(T val);
        void popBack();
        void insertAt(T val);
        void removeAt(int index);
        DualLinkedListNode<T>* operator[](int index);
        DualLinkedListNode<T>* at(int index);
        DualLinkedListNode<T>* front();
        DualLinkedListNode<T>* back();
        int length();
};

#endif