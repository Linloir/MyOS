/*** 
 * Author       : Linloir
 * Date         : 2022-06-14 21:41:53
 * LastEditTime : 2022-06-15 10:04:13
 * Description  : 
 */

#include "systemservices.h"

uint32 syscallTest(uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4) {
    printf("%d, %d, %d, %d, %d\n", p0, p1, p2, p3, p4);
    return p0 + p1 + p2 + p3 + p4;
}