class ProcessState;

__attribute__((naked)) void timeInterruptHandler()
{
    asm volatile("subl $4, %%esp\n\t"
                 "push %%ebp\n\t"
                 "movl %%esp, %%ebp\n\t"
                 "addl $8, %%ebp\n\t"
                 "push %%eax\n\t"
                 "movl %[esp0], %%eax\n\t"
                 "subl $20, %%eax\n\t"
                 "push %%ebx\n\t"
                 "movl 0(%%ebp), %%ebx\n\t"
                 "movl %%ebx, 0(%%eax)\n\t"
                 "movl 4(%%ebp), %%ebx\n\t"
                 "movl %%ebx, 4(%%eax)\n\t"
                 "movl 8(%%ebp), %%ebx\n\t"
                 "movl %%ebx, 8(%%eax)\n\t"
                 "subl $8, %%ebp\n\t"
                 "movl %%ebp, -4(%%ebp)\n\t"
                 "pop %%ebx\n\t"
                 "pop %%eax\n\t"
                 "pop %%ebp\n\t"
                 "movl %%eax, %%esp\n\t"
                 :
                 : [esp0] "i"(0xFFFFFFFC));
    asm volatile("push %%gs\n\t"
                 "push %%fs\n\t"
                 "push %%es\n\t"
                 "push %%ds\n\t"
                 "push %%esi\n\t"
                 "push %%edi\n\t"
                 "push %%ebp\n\t"
                 "push %%edx\n\t"
                 "push %%ecx\n\t"
                 "push %%ebx\n\t"
                 "push %%eax\n\t"
                 "movl %%esp, %%eax\n\t"
                 "push %%eax\n\t"
                 "call %P0\n\t"
                 "addl $4, %%esp\n\t"
                 "pop %%eax\n\t"
                 "pop %%ebx\n\t"
                 "pop %%ecx\n\t"
                 "pop %%edx\n\t"
                 "pop %%ebp\n\t"
                 "pop %%edi\n\t"
                 "pop %%esi\n\t"
                 "pop %%ds\n\t"
                 "pop %%es\n\t"
                 "pop %%fs\n\t"
                 "pop %%gs\n\t"
                 :
                 : "i"(__timeInterruptHandler_asm__));
    asm volatile("pop %%esp\n\t"
                 "push %%ebp\n\t"
                 "movl %%esp, %%ebp\n\t"
                 "addl $4, %%ebp\n\t"
                 "push %%eax\n\t"
                 "movl %[esp0], %%eax\n\t"
                 "subl $20, %%eax\n\t"
                 "push %%ebx\n\t"
                 "movl 0(%%eax), %%ebx\n\t"
                 "movl %%ebx, 0(%%ebp)\n\t"
                 "movl 4(%%eax), %%ebx\n\t"
                 "movl %%ebx, 4(%%ebp)\n\t"
                 "movl 8(%%eax), %%ebx\n\t"
                 "movl %%ebx, 8(%%ebp)\n\t"
                 "pop %%ebx\n\t"
                 "pop %%eax\n\t"
                 "pop %%ebp\n\t"
                 :
                 : [esp0] "i"(0xFFFFFFFC));
    asm volatile("iret\n\t");
}
void __timeInterruptHandler_asm__(ProcessState *preservedState)
{
}