/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:21
 * LastEditTime : 2022-06-14 14:09:51
 * Description  : 
 */
#ifndef _OS_CONSTANT_H_
#define _OS_CONSTANT_H_

#define CODE_SELECTOR 0x08
#define DATA_SELECTOR 0x10
#define STACK_SELECTOR 0x18

#define USER_CODE_SELECTOR 0x23
#define USER_DATA_SELECTOR 0x2B
#define USER_STACK_SELECTOR 0x33

#define KERNEL_DATA_START 0xC0000000
#define KERNEL_DATA_END 0xC0100000
#define KERNEL_STACK_START 0xFFE00000
#define KERNEL_STACK_END 0xFFFFFFFC

#define ESP_0_ADDR 0xC0000000
#define USER_STACK_ADDR 0xBFF00000

#define PCB_MAX_SIZE 4096
#define MAX_PROCESS_COUNT 16

#define PAGE_SIZE 4096

#define MEMORY_INFO_ADDR 0x0

#endif