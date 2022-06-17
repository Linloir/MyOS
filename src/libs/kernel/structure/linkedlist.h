/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:15:43
 * LastEditTime : 2022-06-06 21:56:44
 * Description  : 
 */

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

template<typename T>
struct LinkedListNode {
    T _val;
    LinkedListNode<T>* _next;
};

template<typename T>
class LinkedList {
    private:
        LinkedListNode<T>* _head;
        LinkedListNode<T>* _tail;
        int _length;
    public:
        LinkedList();
        LinkedList(const LinkedList<T>& other);
        ~LinkedList();
        void pushFront(const T& val);
        void popFront();
        void pushBack(const T& val);
        void popBack();
        void insertAt(int index, const T& val);
        void removeAt(int index);
        T& operator[](int index);
        T& at(int index);
        T& front();
        T& back();
        int length();
};

#endif