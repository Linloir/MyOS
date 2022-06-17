/*** 
 * Author       : Linloir
 * Date         : 2022-06-17 11:07:55
 * LastEditTime : 2022-06-17 11:07:55
 * Description  : 
 */

#include "str.h"
#include "algorithm.h"

void itos(char *numStr, uint32 num, uint32 mod) {
    // 只能转换2~26进制的整数
    if (mod < 2 || mod > 26 || num < 0) {
        return;
    }

    uint32 length, temp;

    // 进制转换
    length = 0;
    while(num) {
        temp = num % mod;
        num /= mod;
        numStr[length] = temp > 9 ? temp - 10 + 'A' : temp + '0';
        ++length;
    }

    // 特别处理num=0的情况
    if(!length) {
        numStr[0] = '0';
        numStr[1] = '\0';
        ++length;
    }

    // 将字符串倒转，使得numStr[0]保存的是num的高位数字
    for(int i = 0, j = length - 1; i < j; ++i, --j) {
        swap(numStr[i], numStr[j]);
    }
    
    numStr[length] = '\0';
}