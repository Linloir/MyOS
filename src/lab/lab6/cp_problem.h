#ifndef _CP_PROBLEM_H_
#define _CP_PROBLEM_H_

void _non_lock_cp_problem();
void cp_problem();

void _non_lock_consumer(void** args);
void _non_lock_provider(void** args);

void consumer(void** args);
void provider(void** args);

#endif