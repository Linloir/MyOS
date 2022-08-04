/*** 
 * Author       : Linloir
 * Date         : 2022-07-28 13:14:48
 * LastEditTime : 2022-07-28 13:47:15
 * Description  : Algorithm for strings
 */

#include "str.h"

int strcmp(const char* str1, const char* str2) {
    int p1 = 0, p2 = 0;
    while(str1[p1] != '\0' || str2[p2] != '\0') {
        if(str1[p1] != str2[p2]) {
            return str1[p1] - str2[p2];
        }
        if(str1[p1] != '\0') p1++;
        if(str2[p2] != '\0') p2++;
    }
    return 0;
}

int strncmp(const char* str1, const char* str2, uint32 cnt) {
    int p1 = 0, p2 = 0;
    while(p1 < cnt && p2 < cnt && !(str1[p1] == '\0' && str2[p2] == '\0')) {
        if(str1[p1] != str2[p2]) {
            return str1[p1] - str2[p2];
        }
        if(str1[p1] != '\0') p1++;
        if(str2[p2] != '\0') p2++;
    }
    return 0;
}

char* strcpy(char* dest, const char* src) {
    int p = 0;
    do {
        dest[p] = src[p];
    } while(src[p++] != '\0');
    return dest;
}

char* strcat(char* dest, const char* src, uint32 cnt) {
    int p1 = 0, p2 = 0;
    while(dest[p1] != '\0') p1++;
    do {
        dest[p1++] = src[p2];
    } while(src[p2++] != '\0' && p2 < cnt);
    return dest;
}

char* strcat(char* dest, const char* src) {
    int p1 = 0, p2 = 0;
    while(dest[p1] != '\0') p1++;
    do {
        dest[p1++] = src[p2];
    } while(src[p2++] != '\0');
    return dest;
}

int strlen(const char* str) {
    int p = 0;
    while(str[p] != '\0') p++;
    return p;
}