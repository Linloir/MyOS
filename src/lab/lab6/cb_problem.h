#ifndef _CB_PROBLEM_H_
#define _CB_PROBLEM_H_

void _non_lock_cheeseBurger_problem();
void _spin_lock_cheeseBurger_problem();
void _sem_lock_cheeseBurger_problem();

void _non_lock_mom(void** args);
void _non_lock_son(void** args);

void _spin_lock_mom(void** args);
void _spin_lock_son(void** args);

void _sem_lock_mom(void** args);
void _sem_lock_son(void** args);

#endif