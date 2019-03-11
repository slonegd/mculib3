#pragma once

#include <functional>
#include "timers.h"
#include "table_crc.h"
#include "uart.h"
#include "interrupt.h"
#include <cstring>

#if defined(USE_MOCK_UART)
using UART_ = mock::UART;
#else
using UART_ = ::UART;
#endif


template <class InRegs_t, class OutRegs_t>
class Modbus_slave : TickSubscriber
{
   enum class Function   : uint8_t {read_03 = 0x03, write_16 = 0x10};
   enum class Error_code : uint8_t {wrong_func = 0x01, wrong_reg = 0x02, wrong_value = 0x03};

   UART_& uart;
   Interrupt& interrupt_usart;
   Interrupt& interrupt_DMA_channel; 

   int time {0}; // выдержка времени для модбаса 
   int modbus_time {0};

   const uint8_t address;
   uint8_t func;
   uint16_t first_reg{0};
   uint16_t last_reg {0};
   uint16_t qty_reg  {0};
   uint8_t  qty_byte {0};
   uint16_t data;
   uint16_t crc{0};
   size_t last_message_size;


   uint16_t crc16 (uint8_t* data, uint8_t length);
   uint8_t  set_high_bit (uint8_t);

   bool check_CRC  ();
   bool check_value();
   bool check_reg  (uint16_t qty_reg_device);

   void answer_error (Error_code);
   void answer_03();
   template <class function> void answer_16 (function reaction);

   void uartInterrupt()
   {
      if (uart.is_IDLE()) 
         tick_subscribe();
   }
   void dmaInterrupt()
   {
      if (uart.is_tx_complete())
         uart.receive();
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
   } dma_ {*this};

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

   bool signed_[InRegQty] {};
   
   Modbus_slave (
        uint8_t address
      , UART_& uart
      , Interrupt& interrupt_usart
      , Interrupt& interrupt_DMA_channel
   ) : uart                  {uart}
     , interrupt_usart       {interrupt_usart}
     , interrupt_DMA_channel {interrupt_DMA_channel}
     , address               {address}
     , arInRegs    {}
     , outRegs     {}
     , arInRegsMin {}
     , arInRegsMax {}
   {}

   template <mcu::Periph usart, class TXpin,  class RXpin, class RTSpin> 
   static auto& make (uint8_t address, UART_::Settings set)
   {
      auto interrupt_usart = usart == mcu::Periph::USART1 ? &interrupt_usart1 :
                             usart == mcu::Periph::USART2 ? &interrupt_usart2 :
                             usart == mcu::Periph::USART3 ? &interrupt_usart3 :
                             nullptr;
    #if defined(STM32F0)
      auto interrupt_dma   = usart == Periph::USART1 ? &interrupt_DMA_channel2 : //тут может быть и 4 ???
                             usart == Periph::USART2 ? &interrupt_DMA_channel4 : 
                             usart == Periph::USART3 ? &interrupt_DMA_channel2 :
                             nullptr;
    #elif defined(STM32F1)
  	  auto interrupt_dma   = usart == Periph::USART1 ? &interrupt_DMA_channel4 :
                             usart == Periph::USART2 ? &interrupt_DMA_channel7 : 
                             usart == Periph::USART3 ? &interrupt_DMA_channel2 :
                             nullptr;
    #elif defined(STM32F4)
      auto interrupt_dma   = usart == Periph::USART1 ? &interrupt_DMA_channel7 :
                             usart == Periph::USART2 ? &interrupt_DMA_channel6 : 
                             usart == Periph::USART3 ? &interrupt_DMA_channel3 :
								             usart == Periph::USART4 ? &interrupt_DMA_channel4 :
								             usart == Periph::USART5 ? &interrupt_DMA_channel7 :
								             usart == Periph::USART6 ? &interrupt_DMA_channel6 : //тут может быть и 7 ???
                             nullptr;
#endif

      auto& uart_ref = UART_::make<usart, TXpin, RXpin, RTSpin>();

      static Modbus_slave<InRegs_t, OutRegs_t> modbus {
           address
         , uart_ref
         , *interrupt_usart
         , *interrupt_dma
      };

      modbus.modbus_time = set.baudrate == UART_::Baudrate::BR9600  ? 4 :
                           set.baudrate == UART_::Baudrate::BR14400 ? 3 :
                           set.baudrate == UART_::Baudrate::BR19200 ? 2 :
                           set.baudrate == UART_::Baudrate::BR28800 ? 2 : 1;

      modbus.uart.init(set);
      return modbus;
   }


  //  template <class function>
   void operator() (std::function<void(size_t reg)> reaction);
   auto& buffer(){return uart.buffer;}

};

#define GET_ADR(struct, reg)     (offsetof(struct, reg) / 2)





































template <class InRegs_t, class OutRegs_t>
// template <class function>
inline void Modbus_slave<InRegs_t, OutRegs_t>::operator() (std::function<void(size_t reg)> reaction)
{
   if (uart.is_receiving()) {
      time = 0;
      tick_unsubscribe();
      return;
   }
   
   if (time < modbus_time) 
      return;

   time = 0;
   tick_unsubscribe();
   
   if (uart.buffer.size() < 8) {
      uart.receive();
      return;
   }

   if (uart.buffer.front() != address) {
      uart.receive();
      return;
   }
   if (not check_CRC()) {
      uart.receive();
      return;
   }
   uart.buffer.pop_front(); // adr
   func = uart.buffer.pop_front();
   if (func == static_cast<uint8_t>(Function::read_03)) // operator ==
      answer_03();
   else if (func == static_cast<uint8_t>(Function::write_16))
      answer_16(reaction);
   else 
      answer_error (Error_code::wrong_func);
}




template <class InReg, class OutRegs_t>
uint8_t Modbus_slave<InReg, OutRegs_t>::set_high_bit(uint8_t func)
{
   return (func | 0b10000000);
}

template <class InReg, class OutRegs_t>
bool Modbus_slave<InReg, OutRegs_t>::check_CRC()
{
   auto high = uart.buffer.pop_back();
	 auto low  = uart.buffer.pop_back();
   auto [low_, high_] = CRC16(uart.buffer.begin(), uart.buffer.end());
   return (high == high_) and (low == low_);
}

template <class InReg, class OutRegs_t>
bool Modbus_slave<InReg, OutRegs_t>::check_reg(uint16_t qty_reg_device)
{
   uart.buffer >> first_reg;
   uart.buffer >> qty_reg; 
   last_reg = first_reg + qty_reg - 1;
   qty_byte = qty_reg * 2;
   return (last_reg <= (qty_reg_device - 1));
}

template <class InReg, class OutRegs_t>
void Modbus_slave<InReg, OutRegs_t>::answer_error(Error_code code)
{
   uart.buffer.clear();
   
   if (code == Error_code::wrong_func)
      uart.buffer << address << set_high_bit(func) << static_cast<uint8_t>(code);
   else if (code == Error_code::wrong_reg)
      uart.buffer << address << func << static_cast<uint8_t>(code);
   else if (code == Error_code::wrong_value)
      uart.buffer << address << func << static_cast<uint8_t>(code);
      
   auto [low_, high_] = CRC16(uart.buffer.begin(), uart.buffer.end());
   uart.buffer << low_ << high_;
   uart.transmit();
}

template <class InReg, class OutRegs_t>
void Modbus_slave<InReg, OutRegs_t>::answer_03()
{
   if (not check_reg(OutRegQty)) {
      answer_error(Error_code::wrong_reg);
      return;
   }
   uart.buffer.clear();
   // определить оператор вместо статик каста
   uart.buffer << address << static_cast<uint8_t>(Function::read_03) << qty_byte;
   while(qty_reg--)
      uart.buffer << arOutRegs[first_reg++];
   auto [low_, high_] = CRC16(uart.buffer.begin(), uart.buffer.end());
   uart.buffer << low_ << high_;
   uart.transmit();
}

template <class InReg, class OutRegs_t>
template <class function>
void Modbus_slave<InReg, OutRegs_t>::answer_16(function reaction)
{
   if (not check_reg(InRegQty)) {
      answer_error(Error_code::wrong_reg);
      return;
   }

   uart.buffer >> qty_byte;

   if (not check_value()) {
      answer_error(Error_code::wrong_value);
      // uart.receive();
      return;
   }
   for (uint16_t i = 0; i < qty_reg; i++) {
      reaction (first_reg + i);
   }
   uart.buffer.clear();
   uart.buffer << address << func << first_reg << qty_reg;
   auto [low_, high_] = CRC16(uart.buffer.begin(), uart.buffer.end());
   uart.buffer << low_ << high_;
   uart.transmit();
}

template <class InRegs_t, class OutRegs_t>
bool Modbus_slave<InRegs_t, OutRegs_t>::check_value()
{
   for (uint16_t i = 0; i < qty_reg; i++) {
      uart.buffer >> data;
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


