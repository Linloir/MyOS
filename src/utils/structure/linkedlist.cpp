/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:15:47
 * LastEditTime : 2022-06-06 21:55:15
 * Description  : 
 */

#include "linkedlist.h"
#include "allocator.h"

template<typename T>
LinkedList<T>::LinkedList() {
    _head = (LinkedListNode<T>*)malloc(sizeof(LinkedListNode<T>));
    _head->_next = nullptr;
    _tail = _head;
    _length = 0;
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) {
    _length = other._length;
    _head = (LinkedListNode<T>*)malloc(sizeof(LinkedListNode<T>*));
    _head->_next = nullptr;
    
    LinkedListNode<T>* iter_other = other._head;
    LinkedListNode<T>* iter_this = _head;
    while(iter_other->_next != nullptr) {
        LinkedListNode<T>* newNode = (LinkedListNode<T>*)malloc(sizeof(LinkedListNode<T>));
        
        newNode->_val = iter_other->_next->_val;
        newNode->_next = nullptr;

        iter_other = iter_other->_next;
        iter_this->_next = newNode;
        iter_this = iter_this->_next;
    }
    _tail = iter_this;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    LinkedListNode<T>* iter = _head;
    while(iter->_next != nullptr) {
        LinkedListNode<T>* next = iter->_next;
        free(iter);
        iter = next;
    }
}

template<typename T>
void LinkedList<T>::pushFront(const T& val) {
    LinkedListNode<T>* newNode = (LinkedListNode<T>*)malloc(sizeof(LinkedListNode<T>));
    if(newNode == nullptr)  return;

    newNode->_val = val;
    newNode->_next = _head->_next;
    _head->_next = newNode;
    if(_tail == _head)  _tail = newNode;

    _length++;
}

template<typename T>
void LinkedList<T>::popFront() {
    if(_length <= 0)  return;

    LinkedListNode<T>* deletedNode = _head->_next;
    _head->_next = _head->_next->_next;
    if(_tail == deletedNode)  _tail = _head;
    free(deletedNode);

    _length--;
}

template<typename T>
void LinkedList<T>::pushBack(const T& val) {
    LinkedListNode<T>* newNode = (LinkedListNode<T>*)malloc(sizeof(LinkedListNode<T>));
    if(newNode == nullptr)  return;

    newNode->_next = nullptr;
    newNode->_val = val;
    _tail->_next = newNode;
    _tail = newNode;

    _length++;
}

template<typename T>
void LinkedList<T>::popBack() {
    if(_length <= 0)  return;

    //find before last node
    LinkedListNode<T>* iter = _head;
    while(iter->_next != _tail) {
        iter = iter->_next;
    }

    free(_tail);
    iter->_next = nullptr;
    _tail = iter;

    _length--;
}

template<typename T>
void LinkedList<T>::insertAt(int index, const T& val) {
    if(index < 0 || index > _length)  return;

    LinkedListNode<T>* newNode = (LinkedListNode<T>*)malloc(sizeof(LinkedListNode<T>));
    if(newNode == nullptr)  return;
    
    //find node before insert point
    LinkedListNode<T>* iter = _head;
    for(int i = 0; i < index; i++) {
        iter = iter->_next;
    }

    newNode->_val = val;
    newNode->_next = iter->_next;
    iter->_next = newNode;

    if(index == _length) {
        _tail = newNode;
    }
    _length++;
}

template<typename T>
void LinkedList<T>::removeAt(int index) {
    if(_length <= 0)  return;
    if(index < 0 || index >= _length)  return;

    //find node before remove point
    LinkedListNode<T>* iter = _head;
    for(int i = 0; i < index, i++) {
        iter = iter->_next;
    }
    
    free(iter->_next);
    iter->_next = nullptr;
    if(index == _length - 1) {
        _tail = iter;
    }

    _length--;
}

template<typename T>
T& LinkedList<T>::operator[](int index) {
    if(_length == 0)  return LinkedListNode<T>();

    if(index < 0)  index = 0;
    if(index >= _length)  index = _length;

    if(index == 0)  return _head->_next->_val;
    if(index == _length - 1)  return _tail->_val;

    LinkedListNode<T>* iter = _head->_next;
    for(int i = 0; i < index; i++) {
        iter = iter->_next;
    }

    return iter->_val;
}

template<typename T>
T& LinkedList<T>::at(int index) {
    if(_length == 0)  return LinkedListNode<T>();

    if(index < 0)  index = 0;
    if(index >= _length)  index = _length;

    if(index == 0)  return _head->_next->_val;
    if(index == _length - 1)  return _tail->_val;

    LinkedListNode<T>* iter = _head->_next;
    for(int i = 0; i < index; i++) {
        iter = iter->_next;
    }

    return iter->_val;
}

template<typename T>
T& LinkedList<T>::front() {
    return this->at(0);
}

template<typename T>
T& LinkedList<T>::back() {
    return this->at(_length - 1);
}

template<typename T>
int LinkedList<T>::length() {
    return _length;
}