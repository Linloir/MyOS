#include "cp_problem.h"
#include "proc.h"
#include "lock.h"
#include "std_utils.h"

void _non_lock_cp_problem() {
    int count = 0;
    List providerList = List();
    List consumerList = List();
    void* args[3] = {&count, &providerList, &consumerList};
    Scheduler::executeThread(_non_lock_provider, args, 10);
    Scheduler::executeThread(_non_lock_consumer, args, 10);
    while(true){}
}

void cp_problem() {
    int count = 0;
    SemLock remainLock = SemLock(0);
    SemLock emptyLock = SemLock(5);
    SemLock mutexLock = SemLock(1);
    void* args[4] = {&count, &remainLock, &emptyLock, &mutexLock};
    Scheduler::executeThread(provider, args, 10);
    Scheduler::executeThread(consumer, args, 10);
    Scheduler::executeThread(provider, args, 2);
    Scheduler::executeThread(consumer, args, 6);
    Scheduler::executeThread(provider, args, 5);
    Scheduler::executeThread(consumer, args, 8);
    Scheduler::executeThread(consumer, args, 10);
    Scheduler::executeThread(provider, args, 7);
    while(true){}
}

void _non_lock_provider(void** args) {
    int* count = (int*)args[0];
    List* providerList = (List*)args[1];
    List* consumerList = (List*)args[2];
    while(true) {
        if(*count == 5) {
            PCB* currentThread = Scheduler::currentRunningThread();
            currentThread->status = JobStatus::BLOCKED;
            providerList->pushBack(&currentThread->scheduleListNode);
            Scheduler::schedule();
        }
        int before = *count;
        (*count)++;
        int after = *count;
        printf("provider: %d -> %d\n", before, after);
        if(*count == 1 && !consumerList->isEmpty()) {
            PCB* newConsumer = nodeToPCB(consumerList->popFront());
            Scheduler::awakeThreadHasen(newConsumer);
        }
        for(int i = 0; i < 200000000; i++) {}
    }
}

void _non_lock_consumer(void** args) {
    int* count = (int*)args[0];
    List* providerList = (List*)args[1];
    List* consumerList = (List*)args[2];
    while(true) {
        if(*count == 0) {
            for(int i = 0; i < 50000000; i++) {}
            PCB* currentThread = Scheduler::currentRunningThread();
            currentThread->status = JobStatus::BLOCKED;
            consumerList->pushBack(&currentThread->scheduleListNode);
            Scheduler::schedule();
        }
        for(int i = 0; i < 100000000; i++) {}
        int before = *count;
        (*count)--;
        int after = *count;
        printf("consumer: %d -> %d\n", before, after);
        if(*count == 4 && !providerList->isEmpty()) {
            PCB* newProvider = nodeToPCB(providerList->popFront());
            Scheduler::awakeThreadHasen(newProvider);
        }
    }
}

void provider(void** args) {
    int* count = (int*)args[0];
    SemLock* remainSem = (SemLock*)args[1];
    SemLock* emptySem = (SemLock*)args[2];
    SemLock* mutexLock = (SemLock*)args[3];
    while(true) {
        emptySem->acquire();
        mutexLock->acquire();
        int before = *count;
        (*count)++;
        int after = *count;
        printf("provider: %d -> %d\n", before, after);
        mutexLock->release();
        remainSem->release();
        for(int i = 0; i < 200000000; i++) {}
    }
}

void consumer(void** args) {
    int* count = (int*)args[0];
    SemLock* remainSem = (SemLock*)args[1];
    SemLock* emptySem = (SemLock*)args[2];
    SemLock* mutexLock = (SemLock*)args[3];
    while(true) {
        remainSem->acquire();
        for(int i = 0; i < 100000000; i++) {}
        mutexLock->acquire();
        int before = *count;
        (*count)--;
        int after = *count;
        printf("consumer: %d -> %d\n", before, after);
        mutexLock->release();
        emptySem->release();
    }
}