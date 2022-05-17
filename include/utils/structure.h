/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 12:40:41
 * LastEditTime : 2022-05-17 13:41:16
 * Description  : 
 */

#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

struct ListNode {
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
};

class List {
private:
    ListNode head;
    int length;
public:
    List();
    void pushFront(ListNode* node);
    void pushBack(ListNode* node);
    ListNode* popFront();
    ListNode* popBack();
    bool isEmpty();
    ListNode* front();
    ListNode* back();
};

#endif