/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:15:43
 * LastEditTime : 2022-06-06 16:55:48
 * Description  : 
 */

#ifndef _LLIST_H_
#define _LLIST_H_

template<typename T>
struct LinkedListNode {
    T val;
    LinkedListNode<T>* next;
};

template<typename T>
struct DualLinkedListNode {
    T val;
    LinkedListNode<T>* prev;
    LinkedListNode<T>* next;
};

template<typename T>
class LinkedList {
    private:
        LinkedListNode<T>* head;
        int length;
    public:
        LinkedList(): head(nullptr) {}
        ~LinkedList();
        void pushFront(T val);
        void popFront();
        void pushBack(T val);
        void popBack();
        void insertAt(T val);
        void removeAt(int index);
        LinkedListNode<T>* operator[](int index);
        LinkedListNode<T>* at(int index);
        LinkedListNode<T>* front();
        LinkedListNode<T>* back();
        int length();
};

template<typename T>
class DualLinkedList {
    private:
        DualLinkedListNode<T>* head;
        int length;
    public:
        DualLinkedList(): head(nullptr) {}
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
        LinkedListNode<T>* back();
        int length();
};

#endif