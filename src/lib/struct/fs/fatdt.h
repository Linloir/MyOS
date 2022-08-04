/*** 
 * Author       : Linloir
 * Date         : 2022-07-29 18:06:23
 * LastEditTime : 2022-07-29 18:18:38
 * Description  : Date Time field for FAT fs
 */

#include "types.h"

#ifndef _FAT_DT_H_
#define _FAT_DT_H_

class FATDate {
    private:
        uint16 _date;
    public:
        FATDate(uint16 date);
    public:
        uint16 year();
        void setYear(uint16 year);
        uint16 month();
        void setMonth(uint16 month);
        uint16 day();
        void setDay(uint16 day);
};

class FATTime {
    private:
        uint8 _timeTenth;
        uint16 _time;
    public:
        FATTime(uint8 timeTenth, uint16 time);
    public:
        uint16 hour();
        void setHour(uint16 hour);
        uint16 minute();
        void setMinute(uint16 minute);
        uint16 second();
        void setSecond(uint16 second);
        uint16 millisecond();
        void setMillisecond(uint16 millisecond);
};

#endif
