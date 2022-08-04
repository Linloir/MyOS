/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 23:11:32
 * LastEditTime : 2022-08-04 15:43:32
 * Description  : 
 */

#include "types.h"

#ifndef _PORT_H_
#define _PORT_H_

template<typename T>
class Port {
    public:
        Port(uint16 port): _port(port) {}
    private:
        const uint16 _port;
    public:
        T read() const;
        void write(T data) const;
};

template<typename T>
class PortWriteOnly {
    public:
        PortWriteOnly(uint16 port) : _port(port) {}
    private:
        const Port<T> _port;
    public:
        void write(T data) const { 
            _port.write(data);
        }
};

template<typename T>
class PortReadOnly {
    public:
        PortReadOnly(uint16 port) : _port(port) {}
    private:
        const Port<T> _port;
    public:
        T read() const {
            return _port.read();
        }
};

template<typename T>
T Port<T>::read() const {
    T dat;
    if(sizeof(dat) == 1) {
        uint8 _dat = static_cast<uint8>(dat);
        asm volatile(
            "movw %[p], %%dx \n\t"
            "inb  %%dx, %%al \n\t"
            "movb %%al, %[o] \n\t"
            : [o] "=r"(_dat)
            : [p] "r"(_port)
            :
        );
        dat = _dat;
    }
    else if(sizeof(dat) == 2) {
        uint16 _dat = static_cast<uint16>(dat);
        asm volatile(
            "movw %[p], %%dx \n\t"
            "inw  %%dx, %%ax \n\t"
            "movw %%ax, %[o] \n\t"
            : [o] "=r"(_dat)
            : [p] "r"(_port)
            :
        );
        dat = _dat;
    }
    else {
        uint32 _dat = static_cast<uint32>(dat);
        asm volatile(
            "movw %[p], %%dx \n\t"
            "inl  %%dx, %%eax \n\t"
            "movl %%eax, %[o] \n\t"
            : [o] "=r"(_dat)
            : [p] "r"(_port)
            :
        );
        dat = _dat;
    }
    return dat;
}

template<typename T>
void Port<T>::write(T data) const {
    if(sizeof(data) == 1) {
        asm volatile(
            "movb %[i], %%al \n\t"
            "movw %[p], %%dx \n\t"
            "outb %%al, %%dx \n\t"
            : 
            : [i] "r"(static_cast<uint8>(data)), [p] "r"(_port)
        );
    }
    else if(sizeof(data) == 2) {
        asm volatile(
            "movw %[i], %%ax \n\t"
            "movw %[p], %%dx \n\t"
            "outw %%ax, %%dx \n\t"
            : 
            : [i] "r"(static_cast<uint16>(data)), [p] "r"(_port)
        );
    }
    else {
        asm volatile(
            "movl %[i], %%eax \n\t"
            "movw %[p], %%dx \n\t"
            "outl %%eax, %%dx \n\t"
            : 
            : [i] "r"(static_cast<uint32>(data)), [p] "r"(_port)
        );
    }
}

#endif