#pragma once

#if not defined (TEST)
   #include "uart.h"
   #include "timers.h"
   #include "interrupt.h"
   #include <cstring>
#endif

namespace mcu {

template <class InReg, class OutReg>
class Modbus_slave
{
   enum class Function   : uint8_t {read_03 = 0x03, write_16 = 0x10};
   enum class Error_code : uint8_t {wrong_func = 0x01, wrong_reg = 0x02, wrong_value = 0x03};

   UART uart;
   Interrupt& interrupt_usart;

   uint8_t address;
   uint8_t func;
   uint16_t first_reg{0};
   uint16_t last_reg {0};
   uint16_t qty_reg  {0};
   uint8_t  qty_byte {0};
   uint8_t  data;
   uint16_t crc{0};

   uint16_t crc16(uint8_t* data, uint8_t length);
   uint8_t  set_high_bit(uint8_t func);

   bool check_CRC  ();
   bool check_reg  (uint16_t qty_reg_device);
   bool check_value();

   void answer_error(Error_code);
   void answer_03();
   template <class function> void answer_16(function reaction);

public:

   static constexpr uint16_t InRegQty  = sizeof (InReg) / 2;
   static constexpr uint16_t OutRegQty = sizeof(OutReg) / 2;

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
   
   Modbus_slave (UART uart, Interrupt& interrupt_usart) 
      : uart {uart} 
      , interrupt_usart {interrupt_usart}
      {}

   template <Periph usart, class TXpin,  class RXpin, class RTSpin, class LEDpin> 
   static auto make (/*const UART::Settings& set*/)
   {
      auto interrupt_tmp = usart == Periph::USART1 ? &interrupt_usart1 :
                           usart == Periph::USART2 ? &interrupt_usart2 :
                           usart == Periph::USART3 ? &interrupt_usart3 :
                           nullptr;

      Modbus_slave<InReg, OutReg> modbus {UART::make<usart, TXpin, RXpin, RTSpin, LEDpin>(), *interrupt_tmp};

      // modbus.uart.init(set);
      return modbus;
   }


   template <class function> void operator() (function reaction);


};







































template <class InReg, class OutReg>
template <class function>
void Modbus_slave<InReg, OutReg>::operator() (function reaction)
{
   uint16_t qty_transactions;
   uint8_t adr;
   uart >> adr;
   if (adr != address) {
      uart.start_receive();
      return;
   }
   if (not check_CRC()) {
      uart.start_receive();
      return;
   }
   uart >> func;
   if(func == static_cast<uint8_t>(Function::read_03))
      answer_03();
   else if (func == static_cast<uint8_t>(Function::write_16))
      answer_16(reaction);
   else 
      answer_error(Error_code::wrong_func);
}






template <class InReg, class OutReg>
uint16_t Modbus_slave<InReg, OutReg>::crc16(uint8_t* data, uint8_t length)
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

template <class InReg, class OutReg>
uint8_t Modbus_slave<InReg, OutReg>::set_high_bit(uint8_t func)
{
   return (func | 0b10000000);
}

template <class InReg, class OutReg>
bool Modbus_slave<InReg, OutReg>::check_CRC()
{
   return uart.buffer_CRC() == crc16(uart.buffer_pointer(), uart.buffer_end());
}

template <class InReg, class OutReg>
bool Modbus_slave<InReg, OutReg>::check_reg(uint16_t qty_reg_device)
{
   first_reg; uart >> first_reg;
   qty_reg;   uart >> qty_reg; 
   last_reg = first_reg + qty_reg;
   qty_byte = qty_reg * 2;
   return (last_reg > (qty_reg_device - 1));
}

template <class InReg, class OutReg>
void Modbus_slave<InReg, OutReg>::answer_error(Error_code code)
{
   if (code == Error_code::wrong_func)
      uart << address << set_high_bit(func) << static_cast<uint8_t>(code);
   else
      uart << address << func << static_cast<uint8_t>(code);
   crc = crc16 (uart.buffer_pointer(), uart.buffer_end());
   uart << crc;
   uart.start_transmit();
}

template <class InReg, class OutReg>
void Modbus_slave<InReg, OutReg>::answer_03()
{
   if (not check_reg(OutRegQty)) {
      answer_error(Error_code::wrong_reg);
      return;
   }
   uart << address << static_cast<uint8_t>(Function::read) << qty_byte;
   while(qty_reg--)
      uart << arOutRegs[first_reg++];
   crc = crc16 (uart.buffer_pointer(), uart.buffer_end());
   uart << crc;
   uart.start_transmit();
}

template <class InReg, class OutReg>
template <class function>
void Modbus_slave<InReg, OutReg>::answer_16(function reaction)
{
   if (not check_reg(InRegQty)) {
      answer_error(Error_code::wrong_reg);
      return;
   }
   if (not check_value()) {
      answer_error(Error_code::wrong_value);
      uart.start_receive();
      return;
   }
   
   while(qty_reg--) {
      uart >> arInRegs[first_reg++];
      reaction (first_reg++);
   }
   uart << address << func << first_reg << qty_reg;
   crc = crc16 (uart.buffer_pointer(), uart.buffer_end());
   uart << crc;
   uart.start_transmit();
}

template <class InReg, class OutReg>
bool Modbus_slave<InReg, OutReg>::check_value()
{
   while(qty_reg--) {
      uart >> data;
      if (data < arInRegsMin[first_reg++] and data > arInRegsMax[first_reg++])
         return false;
   }

   return true;
}


} // namespace mcu