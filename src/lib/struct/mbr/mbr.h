/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 21:44:49
 * LastEditTime : 2022-07-25 01:13:51
 * Description  : Master Boot Record structure
 */

#include "types.h"
#include "partition.h"

#ifndef _MBR_H_
#define _MBR_H_

class MBR {
    public:
        byte bootstrap[446];    //Bootstrap code
        Partition partition_1;  //Partition entry #1
        Partition partition_2;  //Partition entry #2
        Partition partition_3;  //Partition entry #3
        Partition partition_4;  //Partition entry #4
        byte signature[2];      //Bootable signature
};

#endif