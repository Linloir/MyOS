/*** 
 * Author       : Linloir
 * Date         : 2022-06-12 21:47:00
 * LastEditTime : 2022-06-12 21:52:38
 * Description  : 
 */
#ifndef _EFLAGS_H_
#define _EFLAGS_H_

enum class EFlags {
    CARRY                       = 1,
    PARITY                      = 1 << 2,
    AUXILIARY                   = 1 << 4,
    ZERO                        = 1 << 6,
    SIGN                        = 1 << 7,
    TRAP                        = 1 << 8,
    INTERRUPT_ENABLED           = 1 << 9,
    DIRECTION                   = 1 << 10,
    OVERFLOW                    = 1 << 11,
    IO_PRIVILEGE_BIT_1         = 1 << 12,
    IO_PRIVILEGE_BIT_2         = 1 << 13,
    NESTED_TASK                 = 1 << 14,
    RESUME                      = 1 << 16,
    VIRTUAL_8086                = 1 << 17,
    ALIGNMENT_CHECK             = 1 << 18,
    VIRTUAL_INTERRUPT           = 1 << 19,
    VIRTUAL_INTERRUPT_PENDING   = 1 << 20,
    CPUID_AVAILABLE             = 1 << 21
};

#endif