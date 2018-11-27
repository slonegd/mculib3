#pragma once

#include "timers.h"
#include "table_crc.h"
#if not defined (TEST)
   #include "uart.h"
   #include "interrupt.h"
   #include <cstring>
#endif

namespace mcu {

template <class InRegs_t, class OutRegs_t>
class Modbus_slave : TickSubscriber
{
   size_t time {0}; // выдержка времени для модбаса 
   
   enum class Function   : uint8_t {read_03 = 0x03, write_16 = 0x10};
   enum class Error_code : uint8_t {wrong_func = 0x01, wrong_reg = 0x02, wrong_value = 0x03};

   UART uart;
   Interrupt& interrupt_usart;
   Interrupt& interrupt_DMA_channel;

   const uint8_t address;
   uint8_t func;
   uint16_t first_reg{0};
   uint16_t last_reg {0};
   uint16_t qty_reg  {0};
   uint8_t  qty_byte {0};
   uint16_t  data;
   uint16_t crc{0};


   uint16_t crc16(uint8_t* data, uint8_t length);
   uint8_t  set_high_bit(uint8_t func);

   bool check_CRC  ();
   bool check_value();
   bool check_reg  (uint16_t qty_reg_device);

   void answer_error(Error_code);
   void answer_03();
   template <class function> void answer_16(function reaction);

   void uartInterrupt()
   {
      if (uart.is_IDLE())
         subscribe();
   }
   void dmaInterrupt()
   {
      uart.start_receive();
   }

   void notify() override 
   {
      time++;
   }

   using Parent = Modbus_slave;

   struct uart_interrupt : Interrupting
   {
      Parent& parent;
      uart_interrupt (Parent& parent) : parent(parent) {
         parent.interrupt_usart.subscribe (this);
      }
      void interrupt() override {parent.uartInterrupt();} 
   } uart_ {*this};

   struct dma_interrupt : Interrupting
   {
      Parent& parent;
      dma_interrupt (Parent& parent) : parent(parent) {
         parent.interrupt_DMA_channel.subscribe (this);
      }
      void interrupt() override {parent.dmaInterrupt();} 
   } dma_ {*this};;

public:

   static constexpr uint16_t InRegQty  = sizeof (InRegs_t) / 2;
   static constexpr uint16_t OutRegQty = sizeof(OutRegs_t) / 2;

   union {
      InRegs_t inRegs;
      uint16_t arInRegs[InRegQty];
   };
   union {
      OutRegs_t outRegs;
      uint16_t arOutRegs[OutRegQty];
   };
   union {
      InRegs_t inRegsMin;
      uint16_t arInRegsMin[InRegQty];
   };
   union {
      InRegs_t inRegsMax;
      uint16_t arInRegsMax[InRegQty];
   };

   bool signed_[InRegQty] {false};
   
   Modbus_slave (uint8_t address, UART uart, Interrupt& interrupt_usart, Interrupt& interrupt_DMA_channel) 
      : address     {address}
      , arInRegs    {}
      , outRegs     {}
      , arInRegsMin {}
      , arInRegsMax {}
      , uart {uart} 
      , interrupt_usart {interrupt_usart}
      , interrupt_DMA_channel {interrupt_DMA_channel}
      {}

   template <uint8_t address, Periph usart, class TXpin,  class RXpin, class RTSpin, class LEDpin> 
   static auto make (UART::Settings set)
   {
      auto interrupt_usart = usart == Periph::USART1 ? &interrupt_usart1 :
                             usart == Periph::USART2 ? &interrupt_usart2 :
                             usart == Periph::USART3 ? &interrupt_usart3 :
                             nullptr;
      auto interrupt_dma   = usart == Periph::USART1 ? &interrupt_DMA_channel4 :
                             usart == Periph::USART2 ? &interrupt_DMA_channel7 : 
                             usart == Periph::USART3 ? &interrupt_DMA_channel2 :
                             nullptr;

      Modbus_slave<InRegs_t, OutRegs_t> modbus {address, UART::make<usart, TXpin, RXpin, RTSpin, LEDpin>(), *interrupt_usart, *interrupt_dma};

      modbus.uart.init(set);
      // modbus.uart.modbus_time(set.baudrate);
      return modbus;
   }


   template <class function> void operator() (function reaction);


};







































template <class InRegs_t, class OutRegs_t>
template <class function>
inline void Modbus_slave<InRegs_t, OutRegs_t>::operator() (function reaction)
{
   if (time < 4)
      return;

   time = 0;
   unsubscribe();
   
   if (uart.buffer_end() < 8) {
      uart.start_receive();
      return;
   }

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




template <class InReg, class OutRegs_t>
uint8_t Modbus_slave<InReg, OutRegs_t>::set_high_bit(uint8_t func)
{
   return (func | 0b10000000);
}

template <class InReg, class OutRegs_t>
bool Modbus_slave<InReg, OutRegs_t>::check_CRC()
{
   crc = uart.pop_back();
   uint16_t crc_buffer = CRC16(uart.buffer_pointer(), uart.buffer_end());
   return crc == crc_buffer;
}

template <class InReg, class OutRegs_t>
bool Modbus_slave<InReg, OutRegs_t>::check_reg(uint16_t qty_reg_device)
{
   uart >> first_reg;
   uart >> qty_reg; 
   last_reg = first_reg + qty_reg - 1;
   qty_byte = qty_reg * 2;
   return (last_reg <= (qty_reg_device - 1));
}

template <class InReg, class OutRegs_t>
void Modbus_slave<InReg, OutRegs_t>::answer_error(Error_code code)
{
   uart.buffer_clean();
   
   if (code == Error_code::wrong_func)
      uart << address << set_high_bit(func) << static_cast<uint8_t>(code);
   else if (code == Error_code::wrong_reg)
      uart << address << func << static_cast<uint8_t>(code);
   else if (code == Error_code::wrong_value)
      uart << address << func << static_cast<uint8_t>(code);
      
   crc = CRC16(uart.buffer_pointer(), uart.buffer_end());
   uart << crc;
   uart.start_transmit();
}

template <class InReg, class OutRegs_t>
void Modbus_slave<InReg, OutRegs_t>::answer_03()
{
   if (not check_reg(OutRegQty)) {
      answer_error(Error_code::wrong_reg);
      return;
   }
   uart << address << static_cast<uint8_t>(Function::read_03) << qty_byte;
   while(qty_reg--)
      uart << arOutRegs[first_reg++];
   crc = CRC16(uart.buffer_pointer(), uart.buffer_end());
   uart << crc;
   uart.start_transmit();
}

template <class InReg, class OutRegs_t>
template <class function>
void Modbus_slave<InReg, OutRegs_t>::answer_16(function reaction)
{
   if (not check_reg(InRegQty)) {
      answer_error(Error_code::wrong_reg);
      return;
   }

   uart >> qty_byte;

   if (not check_value()) {
      answer_error(Error_code::wrong_value);
      uart.start_receive();
      return;
   }
   for (uint16_t i = 0; i < qty_reg; i++) {
      reaction (first_reg + i);
   }
   
   uart << address << func << first_reg << qty_reg;
   crc = CRC16(uart.buffer_pointer(), uart.buffer_end());
   uart << crc;
   uart.start_transmit();
}

template <class InRegs_t, class OutRegs_t>
bool Modbus_slave<InRegs_t, OutRegs_t>::check_value()
{
   for (uint16_t i = 0; i < qty_reg; i++) {
      uart >> data;
      if (signed_[first_reg + i]) {
         if ((int16_t(data) < int16_t(arInRegsMin[first_reg + i]) and int16_t(arInRegsMin[first_reg + i]) != 0) or
             (int16_t(data) > int16_t(arInRegsMax[first_reg + i]) and int16_t(arInRegsMax[first_reg + i]) != 0))
            return false;
         arInRegs[first_reg + i] = int16_t(data);
      } else {
         if ((data < arInRegsMin[first_reg + i] and arInRegsMin[first_reg + i] != 0) or
             (data > arInRegsMax[first_reg + i] and arInRegsMax[first_reg + i] != 0))
            return false;
         arInRegs[first_reg + i] = data;
      }
   }

   return true;
}


} // namespace mcu