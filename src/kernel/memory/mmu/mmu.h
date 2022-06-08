/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 14:03:21
 * LastEditTime : 2022-06-07 23:54:37
 * Description  : Transform address
 */

#include "os_type.h"

#ifndef _MMU_H_
#define _MMU_H_

#define OFFSET 0xB0100000

uint32 toVirtualAddress(uint32 addr);
uint32 toPhysicalAddress(uint32 addr);

// class Address {
//     protected:
//         uint32 address;
//     public:
//         Address(uint32 addr);
//         Address operator+(Address& other) {return Address(address + other.address);}
//         Address operator+(uint32 other) {return Address(address + other);}
//         Address operator-(Address& other) {return Address(address - other.address);}
//         Address operator-(uint32 other) {return Address(address - other);}
//         Address operator&(Address& other) {return Address(address & other.address);}
//         Address operator&(uint32 other) {return Address(address & other);}
//         Address operator|(Address& other) {return Address(address | other.address);}
//         Address operator|(uint32 other) {return Address(address | other);}
//         Address operator~() {return Address(~address);}
//         Address operator^(Address& other) {return Address(address ^ other.address);}
//         Address operator^(uint32 other) {return Address(address ^ other);}
//         Address operator<<(int shift) {return Address(address << shift);}
//         Address operator>>(int shift) {return Address(address >> shift);}
//         Address& operator+=(Address& other) {address += other.address; return *this;}
//         Address& operator-=(Address& other) {address -= other.address; return *this;}
//         Address& operator<<=(int shift) {address <<= shift; return *this;}
//         Address& operator>>=(int shift) {address >>= shift; return *this;}
//         uint32 value() {return address;}
// };

// class PhysicalAddress: public Address {
//     public:
//         PhysicalAddress(Address addr): Address(addr) {}
//         PhysicalAddress(uint32 addr): Address(addr) {}
//         PhysicalAddress(VirtualAddress addr): Address(addr - OFFSET) {}
//         VirtualAddress toVirtual() {return VirtualAddress(*this);}
//         Address addr() {return address;}

        
//         PhysicalAddress operator+(PhysicalAddress& other) {return PhysicalAddress(address + other.address);}
//         PhysicalAddress operator+(uint32 other) {return PhysicalAddress(address + other);}
//         PhysicalAddress operator-(PhysicalAddress& other) {return PhysicalAddress(address - other.address);}
//         PhysicalAddress operator-(uint32 other) {return PhysicalAddress(address - other);}
//         PhysicalAddress operator&(PhysicalAddress& other) {return PhysicalAddress(address & other.address);}
//         PhysicalAddress operator&(uint32 other) {return PhysicalAddress(address & other);}
//         PhysicalAddress operator|(PhysicalAddress& other) {return PhysicalAddress(address | other.address);}
//         PhysicalAddress operator|(uint32 other) {return PhysicalAddress(address | other);}
//         PhysicalAddress operator~() {return PhysicalAddress(~address);}
//         PhysicalAddress operator^(PhysicalAddress& other) {return PhysicalAddress(address ^ other.address);}
//         PhysicalAddress operator^(uint32 other) {return PhysicalAddress(address ^ other);}
//         PhysicalAddress operator<<(int shift) {return PhysicalAddress(address << shift);}
//         PhysicalAddress operator>>(int shift) {return PhysicalAddress(address >> shift);}
//         PhysicalAddress& operator+=(PhysicalAddress& other) {address += other.address; return *this;}
//         PhysicalAddress& operator-=(PhysicalAddress& other) {address -= other.address; return *this;}
//         PhysicalAddress& operator<<=(int shift) {address <<= shift; return *this;}
//         PhysicalAddress& operator>>=(int shift) {address >>= shift; return *this;}
// };

// class VirtualAddress: public Address {
//     public:
//         VirtualAddress(Address addr): Address(addr) {}
//         VirtualAddress(uint32 addr): Address(addr) {}
//         VirtualAddress(PhysicalAddress addr): Address(addr + OFFSET) {}
//         PhysicalAddress toPhysical() {return PhysicalAddress(*this);}
//         Address addr() {return address;}

//         VirtualAddress operator+(VirtualAddress& other) {return VirtualAddress(address + other.address);}
//         VirtualAddress operator+(uint32 other) {return VirtualAddress(address + other);}
//         VirtualAddress operator-(VirtualAddress& other) {return VirtualAddress(address - other.address);}
//         VirtualAddress operator-(uint32 other) {return VirtualAddress(address - other);}
//         VirtualAddress operator&(VirtualAddress& other) {return VirtualAddress(address & other.address);}
//         VirtualAddress operator&(uint32 other) {return VirtualAddress(address & other);}
//         VirtualAddress operator|(VirtualAddress& other) {return VirtualAddress(address | other.address);}
//         VirtualAddress operator|(uint32 other) {return VirtualAddress(address | other);}
//         VirtualAddress operator~() {return VirtualAddress(~address);}
//         VirtualAddress operator^(VirtualAddress& other) {return VirtualAddress(address ^ other.address);}
//         VirtualAddress operator^(uint32 other) {return VirtualAddress(address ^ other);}
//         VirtualAddress operator<<(int shift) {return VirtualAddress(address << shift);}
//         VirtualAddress operator>>(int shift) {return VirtualAddress(address >> shift);}
//         VirtualAddress& operator+=(VirtualAddress& other) {address += other.address; return *this;}
//         VirtualAddress& operator-=(VirtualAddress& other) {address -= other.address; return *this;}
//         VirtualAddress& operator<<=(int shift) {address <<= shift; return *this;}
//         VirtualAddress& operator>>=(int shift) {address >>= shift; return *this;}
// };

#endif