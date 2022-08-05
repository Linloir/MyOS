/*** 
 * Author       : Linloir
 * Date         : 2022-08-04 14:53:03
 * LastEditTime : 2022-08-04 18:04:44
 * Description  : Serial port interface
 */

#include "types.h"
#include "ports.h"

#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

class LineStatus {
    public:
        enum ATTRIBUTES {
            DATA_READY                  = 1,        //Set if there is data that can be read
            OVERRUN_ERR                 = 1 << 1,   //Set if there has been data lost
            PARITY_ERR                  = 1 << 2,   //Set if there was error in the transmission as detected by parity
            FRAMING_ERR                 = 1 << 3,   //Set if a stop bit was missing
            BREAK_IN_DATA               = 1 << 4,   //Set if there is a break in data input
            TRANSMISSION_BUFFER_EMPTY   = 1 << 5,   //Set if the transmission buffer is empty (i.e. data can be sent)
            TRANSMITTER_EMPTY           = 1 << 6,   //Set if the transmitter is not doing anything
            IMPENDING_ERR               = 1 << 7    //Set if there is an error with a word in the input buffer
        };
    private:
        uint8 _attr;
    public:
        LineStatus();
        LineStatus(uint8 flags);
    public:
        LineStatus& operator=(uint8 flags);
        LineStatus& operator=(const LineStatus& other);
        //Judge whether two flags are equal
        bool operator==(uint8 flags);
        //Combine two flags
        LineStatus operator+(uint8 flags);
        LineStatus& operator+=(uint8 flags);
        //Remove flags
        LineStatus operator-(uint8 flags);
        LineStatus& operator-=(uint8 flags);
        //Bitwise or
        LineStatus operator|(uint8 flags);
        LineStatus& operator|=(uint8 flags);
        //Bitwise and
        LineStatus operator&(uint8 flags);
        LineStatus& operator&=(uint8 flags);
        //Reverse flags
        LineStatus operator~();
        //Toggle flag bits
        LineStatus& operator^=(uint8 flags);
        //Judge whether flags are contained
        bool contains(uint8 flags);
};

class SerialPort {
    private:
        Port<uint8> _dataPort;
        PortWriteOnly<uint8> _intEnablePort;
        PortWriteOnly<uint8> _fifoCtrlPort;
        PortWriteOnly<uint8> _lineCtrlPort;
        PortWriteOnly<uint8> _modemCtrlPort;
        PortReadOnly<uint8> _lineStatusPort;
    public:
        SerialPort(uint16 base);
    private:
        LineStatus _lineStatus();
    public:
        void init();
        void send(uint8 data);
        void send(const char* str);
        uint8 receive();
};

#endif
