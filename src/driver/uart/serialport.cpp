/*** 
 * Author       : Linloir
 * Date         : 2022-08-04 15:29:01
 * LastEditTime : 2022-08-04 16:58:00
 * Description  : Serial port interface
 */

#include "serialport.h"

LineStatus::LineStatus() {
    _attr = 0;
}
LineStatus::LineStatus(uint8 flags) {
    _attr = flags;
}
LineStatus& LineStatus::operator=(uint8 flags) {
    _attr = flags;
    return *this;
}
LineStatus& LineStatus::operator=(const LineStatus& other) {
    _attr = other._attr;
    return *this;
}
//Judge whether two flags are equal
bool LineStatus::operator==(uint8 flags) {
    return _attr == flags;
}
//Combine two flags
LineStatus LineStatus::operator+(uint8 flags) {
    uint8 newAttr = _attr | flags;
    return LineStatus(newAttr);
}
LineStatus& LineStatus::operator+=(uint8 flags) {
    _attr |= flags;
    return *this;
}
//Remove flags
LineStatus LineStatus::operator-(uint8 flags) {
    uint8 newAttr = _attr & ~(flags);
    return LineStatus(newAttr);
}
LineStatus& LineStatus::operator-=(uint8 flags) {
    _attr &= ~(flags);
    return *this;
}
//Bitwise or
LineStatus LineStatus::operator|(uint8 flags) {
    uint8 newAttr = _attr | flags;
    return LineStatus(newAttr);
}
LineStatus& LineStatus::operator|=(uint8 flags) {
    _attr |= flags;
    return *this;
}
//Bitwise and
LineStatus LineStatus::operator&(uint8 flags) {
    uint8 newAttr = _attr & flags;
    return LineStatus(newAttr);
}
LineStatus& LineStatus::operator&=(uint8 flags) {
    _attr &= flags;
    return *this;
}
//Reverse flags
LineStatus LineStatus::operator~() {
    return LineStatus(_attr);
}
//Toggle flag bits
LineStatus& LineStatus::operator^=(uint8 flags) {
    _attr ^= flags;
    return *this;
}
//Judge whether flags are contained
bool LineStatus::contains(uint8 flags) {
    return (_attr & flags) == flags;
}

SerialPort::SerialPort(uint16 base):
_dataPort(base),
_intEnablePort(base + 1),
_fifoCtrlPort(base + 2),
_lineCtrlPort(base + 3),
_modemCtrlPort(base + 4),
_lineStatusPort(base + 5) {}

LineStatus SerialPort::_lineStatus() {
    return static_cast<LineStatus>(_lineStatusPort.read());
}

void SerialPort::init() {
    // Disable interrupts
    _intEnablePort.write(0x00);
    // Enable DLAB
    _lineCtrlPort.write(0x80);
    // Set maximum speed to 38400 bps by configuring DLL and DLM
    _dataPort.write(0x03);
    _intEnablePort.write(0x00);
    // Disable DLAB and set data word length to 8 bits
    _lineCtrlPort.write(0x03);
    // Enable FIFO, clear TX/RX queues and set interrupt watermark at 14 bytes
    _fifoCtrlPort.write(0x07);
    // Mark data terminal ready, signal request to send and enable auxilliary output #2
    // (used as interrupt line for CPU)
    _modemCtrlPort.write(0x0B);
    // Enable interrupts
    _intEnablePort.write(0x01);
}

void SerialPort::send(uint8 data) {
    //Wait until transmission buffer is empty
    while(!_lineStatus().contains(LineStatus::TRANSMISSION_BUFFER_EMPTY)) {}
    //Send data
    _dataPort.write(data);
}

uint8 SerialPort::receive() {
    //Wait until data is ready
    while(!_lineStatus().contains(LineStatus::DATA_READY)) {}
    //Receive data
    return _dataPort.read();
}