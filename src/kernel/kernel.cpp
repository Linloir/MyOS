/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-05-18 14:00:09
 * Description  : 
 */
#include "interrupt.h"
#include "os_constant.h"
#include "kernel.h"
#include "sched.h"
#include "std_utils.h"
#include "lock.h"

void firstThread(void** args);
void secondThread(void** args);
void thirdThread(void** args);
void provider(void** args);
void consumer(void** args);

void firstThread(void**) {
    int cheeseBurger = 0;
    SpinLock spinLock = SpinLock();
    SemLock semLock = SemLock(1);
    // int finished = 0x00;
    // void* args[4] = {&cheeseBurger, &spinLock, &semLock, &finished};
    // Scheduler::executeThread(secondThread, args, 10);
    // Scheduler::executeThread(thirdThread, args, 10);
    // while(finished != 0x11) {}
    int count = 0;
    SemLock remainLock = SemLock(0);
    SemLock emptyLock = SemLock(5);
    SemLock mutexLock = SemLock(1);
    List providerList = List();
    List consumerList = List();
    void* args_2[6] = {&count, &remainLock, &emptyLock, &providerList, &consumerList, &mutexLock};
    Scheduler::executeThread(provider, args_2, 10);
    Scheduler::executeThread(consumer, args_2, 10);
    // Scheduler::executeThread(provider, args_2, 2);
    // Scheduler::executeThread(consumer, args_2, 6);
    // Scheduler::executeThread(provider, args_2, 5);
    // Scheduler::executeThread(consumer, args_2, 8);
    // Scheduler::executeThread(consumer, args_2, 10);
    // Scheduler::executeThread(provider, args_2, 7);
    while(true){
    }
}

void secondThread(void** args) {
    int* cheeseBurger = (int*)args[0];
    SpinLock* spinLock = (SpinLock*)args[1];
    SemLock* semLock = (SemLock*)args[2];
    int* finishFlag = (int*)args[3];
    // spinLock.lock();
    semLock->acquire();
    printf("There are %d cheese burgers\n", (*cheeseBurger));
    for(int i = 0; i < 200000000; i++){}
    (*cheeseBurger) += 10;
    printf("made 10 cheese burgers\n");
    printf("now there are %d cheese burgers\n", (*cheeseBurger));
    for(int i = 0; i < 200000000; i++){}
    if((*cheeseBurger) == 0) {
        printf("What the ? where are my cheese burgers!");
    }
    else {
        printf("Safe and sound\n");
    }
    (*finishFlag) |= 0x01;
    semLock->release();
    // spinLock.release();
}

void thirdThread(void** args) {
    int* cheeseBurger = (int*)args[0];
    SpinLock* spinLock = (SpinLock*)args[1];
    SemLock* semLock = (SemLock*)args[2];
    int* finishFlag = (int*)args[3];
    // spinLock.lock();
    semLock->acquire();
    for(int i = 0; i < 200000000; i++){}
    printf("Look what I found! %d burgers!\n", (*cheeseBurger));
    for(int i = 0; i < 100000000; i++){}
    if((*cheeseBurger) >= 10) {
        (*cheeseBurger) -= 10;
    }
    printf("Now there are %d cheese burgers\n", (*cheeseBurger));
    (*finishFlag) |= 0x10;
    semLock->release();
    // spinLock.lock();
}

void provider(void** args) {
    int* count = (int*)args[0];
    SemLock* remainSem = (SemLock*)args[1];
    SemLock* emptySem = (SemLock*)args[2];
    List* providerList = (List*)args[3];
    List* consumerList = (List*)args[4];
    SemLock* mutexLock = (SemLock*)args[5];
    while(true) {
        if(*count == 5) {
            // printf("provider: Empty, going to sleep\n");
            PCB* currentThread = Scheduler::currentRunningThread();
            currentThread->status = JobStatus::BLOCKED;
            providerList->pushBack(&currentThread->scheduleListNode);
            // printf("provider: Slept\n");
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

        // emptySem->acquire();
        // mutexLock->acquire();
        // int before = *count;
        // (*count)++;
        // int after = *count;
        // printf("provider: %d -> %d\n", before, after);
        // mutexLock->release();
        // remainSem->release();
        // for(int i = 0; i < 200000000; i++) {}
    }
}

void consumer(void** args) {
    int* count = (int*)args[0];
    SemLock* remainSem = (SemLock*)args[1];
    SemLock* emptySem = (SemLock*)args[2];
    List* providerList = (List*)args[3];
    List* consumerList = (List*)args[4];
    SemLock* mutexLock = (SemLock*)args[5];
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

        // remainSem->acquire();
        // for(int i = 0; i < 100000000; i++) {}
        // mutexLock->acquire();
        // int before = *count;
        // (*count)--;
        // int after = *count;
        // printf("consumer: %d -> %d\n", before, after);
        // mutexLock->release();
        // emptySem->release();
    }
}

extern "C" void kernel() {
    clearScreen();
    InterruptManager::initialize((uint32*)IDT_START_ADDR);
    Scheduler::initialize();
    InterruptManager::set8259A();
    InterruptManager::setTimeInterruptHandler(asm_time_interrupt_handler);
    InterruptManager::enableTimeInterrupt();
    asm_enable_interrupt();
    Scheduler::executeThread(firstThread, nullptr, 1);
    while(true) {
        //Halt
    }
}