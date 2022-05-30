#include "cb_problem.h"
#include "proc.h"
#include "lock.h"
#include "std_utils.h"

void _non_lock_cheeseBurger_problem() {
    int cheeseBurger = 0;
    void* args[1] = {&cheeseBurger};
    Scheduler::executeThread(_non_lock_mom, args, 1);
    Scheduler::executeThread(_non_lock_son, args, 1);
    while(true){}
}

void _spin_lock_cheeseBurger_problem() {
    int cheeseBurger = 0;
    SpinLock lock = SpinLock();
    void* args[2] = {&cheeseBurger, &lock};
    Scheduler::executeThread(_spin_lock_mom, args, 1);
    Scheduler::executeThread(_spin_lock_son, args, 1);
    while(true){}
}

void _sem_lock_cheeseBurger_problem() {
    int cheeseBurger = 0;
    SemLock lock = SemLock(1);
    void* args[2] = {&cheeseBurger, &lock};
    Scheduler::executeThread(_sem_lock_mom, args, 1);
    Scheduler::executeThread(_sem_lock_son, args, 1);
    while(true){}
}

void _non_lock_mom(void** args) {
    int* cheeseBurger = (int*)args[0];
    printf("mom: There are %d cheese burgers\n", (*cheeseBurger));
    for(int i = 0; i < 200000000; i++){}
    (*cheeseBurger) += 10;
    printf("mom: made 10 cheese burgers\n");
    printf("mom: now there are %d cheese burgers\n", (*cheeseBurger));
    for(int i = 0; i < 200000000; i++){}
    if((*cheeseBurger) == 0) {
        printf("mom: What the ? where are my cheese burgers!");
    }
    else {
        printf("mom: Safe and sound\n");
    }
}

void _non_lock_son(void** args) {
    int* cheeseBurger = (int*)args[0];
    for(int i = 0; i < 200000000; i++){}
    printf("son: Look what I found! %d burgers!\n", (*cheeseBurger));
    for(int i = 0; i < 100000000; i++){}
    if((*cheeseBurger) >= 10) {
        (*cheeseBurger) -= 10;
    }
    printf("son: Now there are %d cheese burgers\n", (*cheeseBurger));
}

void _spin_lock_mom(void** args) {
    int* cheeseBurger = (int*)args[0];
    SpinLock* lock = (SpinLock*)args[1];
    lock->lock();
    printf("mom: There are %d cheese burgers\n", (*cheeseBurger));
    for(int i = 0; i < 200000000; i++){}
    (*cheeseBurger) += 10;
    printf("mom: made 10 cheese burgers\n");
    printf("mom: now there are %d cheese burgers\n", (*cheeseBurger));
    for(int i = 0; i < 200000000; i++){}
    if((*cheeseBurger) == 0) {
        printf("mom: What the ? where are my cheese burgers!");
    }
    else {
        printf("mom: Safe and sound\n");
    }
    lock->release();
}

void _spin_lock_son(void** args) {
    int* cheeseBurger = (int*)args[0];
    SpinLock* lock = (SpinLock*)args[1];
    lock->lock();
    for(int i = 0; i < 200000000; i++){}
    printf("son: Look what I found! %d burgers!\n", (*cheeseBurger));
    for(int i = 0; i < 100000000; i++){}
    if((*cheeseBurger) >= 10) {
        (*cheeseBurger) -= 10;
    }
    printf("son: Now there are %d cheese burgers\n", (*cheeseBurger));
    lock->release();
}

void _sem_lock_mom(void** args) {
    int* cheeseBurger = (int*)args[0];
    SemLock* lock = (SemLock*)args[1];
    lock->acquire();
    printf("mom: There are %d cheese burgers\n", (*cheeseBurger));
    for(int i = 0; i < 200000000; i++){}
    (*cheeseBurger) += 10;
    printf("mom: made 10 cheese burgers\n");
    printf("mom: now there are %d cheese burgers\n", (*cheeseBurger));
    for(int i = 0; i < 200000000; i++){}
    if((*cheeseBurger) == 0) {
        printf("mom: What the ? where are my cheese burgers!");
    }
    else {
        printf("mom: Safe and sound\n");
    }
    lock->release();
}

void _sem_lock_son(void** args) {
    int* cheeseBurger = (int*)args[0];
    SemLock* lock = (SemLock*)args[1];
    lock->acquire();
    for(int i = 0; i < 200000000; i++){}
    printf("son: Look what I found! %d burgers!\n", (*cheeseBurger));
    for(int i = 0; i < 100000000; i++){}
    if((*cheeseBurger) >= 10) {
        (*cheeseBurger) -= 10;
    }
    printf("son: Now there are %d cheese burgers\n", (*cheeseBurger));
    lock->release();
}