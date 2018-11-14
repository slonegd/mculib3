#pragma once

#include "uart.h"
#include "timers.h"
#include <cstring>

namespace mcu {

class Modbus_slave
{

   UART& uart;

public:

   Modbus_slave (UART& uart) : uart {uart} {}

   template <mcu::Periph usart, class TXpin, class RXpin, class InReg, class OutReg> static auto make (const UART::Settings& set)
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

   Modbus_slave modbus {UART::make<usart, TXpin, RXpin>};

   modbus.uart.init(set);
   
   }


};

} // namespace mcu