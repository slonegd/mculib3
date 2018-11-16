#pragma once

#include "uart.h"
#include "timers.h"
#include "interrupt.h"
#include <cstring>

namespace mcu {

class Modbus_slave
{

   UART uart;
   Interrupt& interrupt_usart;

public:

   Modbus_slave (UART uart, Interrupt& interrupt_usart) 
      : uart {uart} 
      , interrupt_usart {interrupt_usart}
      {}

   template <Periph usart, class TXpin,  class RXpin, class RTSpin, 
                           class LEDpin, class InReg, class OutReg> 
   static auto make (/*const UART::Settings& set*/)
   {

   static constexpr uint16_t InRegQty  = sizeof (InReg) / 2;
   static constexpr uint16_t OutRegQty = sizeof(OutReg) / 2;

   uint8_t address;
   union {
      InReg inRegs;
      uint16_t arInRegs[InRegQty];
   };
   union {
      OutReg outRegs;
      uint16_t arOutRegs[OutRegQty];
   };
   union {
      InReg inRegsMin;
      uint16_t arInRegsMin[InRegQty];
   };
   union {
      InReg inRegsMax;
      uint16_t arInRegsMax[InRegQty];
   };

   auto interrupt_tmp = usart == Periph::USART1 ? &interrupt_usart1 :
                        usart == Periph::USART2 ? &interrupt_usart2 :
                        usart == Periph::USART3 ? &interrupt_usart3 :
                        nullptr;
                                  

   Modbus_slave modbus {UART::make<usart, TXpin, RXpin, RTSpin, LEDpin>(), *interrupt_tmp};

   // modbus.uart.init(set);
      return modbus;
   }

   uint16_t crc16(uint8_t* data, uint8_t length);
   bool check_CRC();
   void answer_03();
   void answer_16();
   void operator() (function reaction);


};









































void Modbus_slave::operator() (function reaction)
{
   uint8_t adr{0};
   uart >> adr;
   if (adr != address) {
      uart.start_receive();
      return;
   }
   check_CRC();
   uint8_t function;
   uart >> function
   if(function == 3) {
      uint16_t first_reg{0}; uart >> first_reg;
      uint16_t qty_reg  {0}; uart >> qty_reg; 
      uint16_t last_reg = first_reg + last_reg;
      uint32_t qty_byte = qty_reg * 2;
      if (last_reg > (OutRegQty - 1)) {
         uart << address << function << qty_byte << data << crc;
      }
      return;
   } else if (function == 16) {

   }

}

uint16_t Modbus_slave::crc16(uint8_t* data, uint8_t length))
{
   int j;
   uint16_t reg_crc = 0xFFFF;
   while(length--)	{
      reg_crc ^= *data++;
      for(j = 0; j < 8; j++) {
         if (reg_crc & 0x01) {
            reg_crc=(reg_crc>>1) ^ 0xA001; // LSB(b0)=1
         } else {
            reg_crc=reg_crc>>1;
         }	
      }
   }
   return reg_crc;
}
bool Modbus_slave::check_CRC()
{
   crc = crc16 (uart.buffer, byteQty - 2);
   return (static_cast<uint8_t>(crc)      == uart.buffer[byteQty-2]
       and static_cast<uint8_t>(crc >> 8) == uart.buffer[byteQty-1])
}

void Modbus_slave::answer_03()
{
   uint8_t function = 3;
   
}


} // namespace mcu