#ifndef _PHIL_PROBLEM_H_
#define _PHIL_PROBLEM_H_

#include "structure.h"

void _non_lock_phil_problem();
void _dead_lock_phil_problem();
void _tri_lock_phil_problem();
void _bi_lock_phil_problem();

// void sleep();
// void awake(List* awaitList);
void _non_lock_philosopher(void** args);
void _dead_lock_philosopher(void** args);
void _tri_lock_philosopher(void** args);
void _bi_lock_philosopher(void** args);

#endif