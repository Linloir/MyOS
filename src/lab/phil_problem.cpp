#include "phil_problem.h"
#include "std_utils.h"
#include "proc.h"
#include "lock.h"

void _non_lock_phil_problem() {
    bool states[5] = {false, false, false, false, false};
    int id[5] = {0, 1, 2, 3, 4};
    void* args0[2] = {&id[0], states};
    void* args1[2] = {&id[1], states};
    void* args2[2] = {&id[2], states};
    void* args3[2] = {&id[3], states};
    void* args4[2] = {&id[4], states};
    Scheduler::executeThread(_non_lock_philosopher, args0, 10);
    Scheduler::executeThread(_non_lock_philosopher, args1, 10);
    Scheduler::executeThread(_non_lock_philosopher, args2, 10);
    Scheduler::executeThread(_non_lock_philosopher, args3, 10);
    Scheduler::executeThread(_non_lock_philosopher, args4, 10);
    while(true){}
}

void _dead_lock_phil_problem() {
    SemLock locks[5] = {SemLock(1), SemLock(1), SemLock(1), SemLock(1), SemLock(1)};
    int id[5] = {0, 1, 2, 3, 4};
    void* args0[2] = {&id[0], locks};
    void* args1[2] = {&id[1], locks};
    void* args2[2] = {&id[2], locks};
    void* args3[2] = {&id[3], locks};
    void* args4[2] = {&id[4], locks};
    Scheduler::executeThread(_dead_lock_philosopher, args0, 10);
    Scheduler::executeThread(_dead_lock_philosopher, args1, 10);
    Scheduler::executeThread(_dead_lock_philosopher, args2, 10);
    Scheduler::executeThread(_dead_lock_philosopher, args3, 10);
    Scheduler::executeThread(_dead_lock_philosopher, args4, 10);
    while(true){}
}

void _tri_lock_phil_problem() {
    SemLock locks[5] = {SemLock(1), SemLock(1), SemLock(1), SemLock(1), SemLock(1)};
    int id[5] = {0, 1, 2, 3, 4};
    SemLock mutex = SemLock(1);
    void* args0[3] = {&id[0], locks, &mutex};
    void* args1[3] = {&id[1], locks, &mutex};
    void* args2[3] = {&id[2], locks, &mutex};
    void* args3[3] = {&id[3], locks, &mutex};
    void* args4[3] = {&id[4], locks, &mutex};
    Scheduler::executeThread(_tri_lock_philosopher, args0, 10);
    Scheduler::executeThread(_tri_lock_philosopher, args1, 10);
    Scheduler::executeThread(_tri_lock_philosopher, args2, 10);
    Scheduler::executeThread(_tri_lock_philosopher, args3, 10);
    Scheduler::executeThread(_tri_lock_philosopher, args4, 10);
    while(true){}
}

void _bi_lock_phil_problem() {
    SemLock locks[5] = {SemLock(1), SemLock(1), SemLock(1), SemLock(1), SemLock(1)};
    int id[5] = {0, 1, 2, 3, 4};
    void* args0[2] = {&id[0], locks};
    void* args1[2] = {&id[1], locks};
    void* args2[2] = {&id[2], locks};
    void* args3[2] = {&id[3], locks};
    void* args4[2] = {&id[4], locks};
    Scheduler::executeThread(_bi_lock_philosopher, args0, 10);
    Scheduler::executeThread(_bi_lock_philosopher, args1, 10);
    Scheduler::executeThread(_bi_lock_philosopher, args2, 10);
    Scheduler::executeThread(_bi_lock_philosopher, args3, 10);
    Scheduler::executeThread(_bi_lock_philosopher, args4, 10);
    while(true){}
}

void _non_lock_philosopher(void** args) {
    int id = *(int*)args[0];
    bool* states = (bool*)args[1];
    
    int left = id;
    int right = id == 0 ? 4 : id - 1;

    while(true) {
        if(!states[left] && !states[right]) {
            states[left] = true;
            states[right] = true;
            printf("%d: Eating\n", id);
            for(int i = 0; i < 200000000; i++){}
            states[left] = false;
            states[right] = false;
            printf("%d: Thinking\n", id);
            for(int i = 0; i < 200000000; i++){}
        }
    }
}

void _dead_lock_philosopher(void** args) {
    int id = *(int*)args[0];
    SemLock* locks = (SemLock*)args[1];
    
    int left = id;
    int right = id == 0 ? 4 : id - 1;

    while(true) {
        locks[left].acquire();
        printf("%d: Picked Left\n", id);
        for(int i = 0; i < 200000000; i++){}
        locks[right].acquire();
        printf("%d: Picked Right\n", id);
        printf("%d: Eating\n", id);
        for(int i = 0; i < 200000000; i++){}
        printf("%d: Thinking\n", id);
        locks[left].release();
        locks[right].release();
        for(int i = 0; i < 200000000; i++){}
    }
}

void _tri_lock_philosopher(void** args) {
    int id = *(int*)args[0];
    SemLock* locks = (SemLock*)args[1];
    SemLock* mutex = (SemLock*)args[2];
    
    int left = id;
    int right = id == 0 ? 4 : id - 1;

    while(true) {
        mutex->acquire();
        locks[left].acquire();
        printf("%d: Picked Left\n", id);
        for(int i = 0; i < 200000000; i++){}
        locks[right].acquire();
        printf("%d: Picked Right\n", id);
        printf("%d: Eating\n", id);
        for(int i = 0; i < 200000000; i++){}
        printf("%d: Thinking\n", id);
        locks[left].release();
        locks[right].release();
        mutex->release();
        for(int i = 0; i < 200000000; i++){}
    }
}

void _bi_lock_philosopher(void** args) {
    int id = *(int*)args[0];
    SemLock* locks = (SemLock*)args[1];
    
    int left = id;
    int right = id == 0 ? 4 : id - 1;

    while(true) {
        if(id & 1) {
            locks[right].acquire();
            printf("%d: Picked Right\n", id);
            for(int i = 0; i < 200000000; i++){}
            locks[left].acquire();
            printf("%d: Picked Left\n", id);
        }
        else {
            locks[left].acquire();
            printf("%d: Picked Left\n", id);
            for(int i = 0; i < 200000000; i++){}
            locks[right].acquire();
            printf("%d: Picked Right\n", id);
        }
        printf(">[%d: Eating]\n", id);
        for(int i = 0; i < 200000000; i++){}
        printf(">[%d: Thinking]\n", id);
        locks[left].release();
        locks[right].release();
        for(int i = 0; i < 200000000; i++){}
    }
}