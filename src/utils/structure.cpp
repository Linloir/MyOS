/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 12:40:15
 * LastEditTime : 2022-06-05 10:54:49
 * Description  : Data structures
 */

#include "structure.h"

List::List() {
    head = ListNode();
    length = 0;
}

void List::pushFront(ListNode* node) {
    if(node == nullptr) {
        return;
    }
    node->next = head.next;
    node->prev = &head;
    head.next = node;
    length++;
}

void List::pushBack(ListNode* node) {
    if(node == nullptr) {
        return;
    }
    ListNode* ptr = &head;
    while(ptr->next) {
        ptr = ptr->next;
    }
    ptr->next = node;
    node->prev = ptr;
    length++;
}

ListNode* List::popFront() {
    ListNode* front = head.next;
    if(front == nullptr) {
        return nullptr;
    }
    head.next = front->next;
    if(front->next != nullptr) {
        front->next->prev = &head;
    }
    front->prev = nullptr;
    front->next = nullptr;
    length--;
    return front;
}

ListNode* List::popBack() {
    ListNode* back = &head;
    while(back->next) {
        back = back->next;
    }
    if(back == &head) {
        return nullptr;
    }
    back->prev->next = nullptr;
    back->prev = nullptr;
    length--;
    return back;
}

bool List::isEmpty() {
    return length == 0;
}

ListNode* List::front() {
    return head.next;
}

ListNode* List::back() {
    ListNode* back = &head;
    while(back->next) {
        back = back->next;
    }
    if(back == &head) {
        return nullptr;
    }
    return back;
}