#pragma once

#include <iomanip>

#define USE_MOCK_UART
#include "mock_dma.h"
#include "mock_periph_usart.h"
#if defined (STM32F1)
#include "mock_afio.h"
#endif
#include "mock_gpio.h"
#include "mock_interrupt.h"
// #include "mock_net_buffer.h"
#include "uart.h"
#include "process.h"

namespace mock {

uint16_t CNDTR {255};

// template<size_t buffer_size = 255>
class UART_sized : public ::UART_sized<> {

   Process& process {Process::make()};
   UART_sized() = default;
   UART_sized(const ::UART_sized<>& v) : ::UART_sized<>(v) {}
   auto& base() { return *static_cast<::UART_sized<>*>(this); }
   
public:
   template <
      mcu::Periph usart
      , class TXpin
      , class RXpin
      , class RTSpin
   > static auto& make()
   {
      static auto uart = UART_sized(::UART_sized<>::template make<usart,TXpin,RXpin,RTSpin>());
      uart.process.clear();
      uart.process << "Инициализация ";
      if (std::is_same_v<TXpin,mcu::PA9>) 
         uart.process << "PA9";
      uart.process << "(TX), ";
      if (std::is_same_v<RXpin,mcu::PA10>)
         uart.process << "PA10";
      uart.process << "(RX), ";
      if (std::is_same_v<RTSpin,mcu::PA11>)
         uart.process << "PA10";
      uart.process << "(RTS), ";
      uart.process << uart.uart_periph << std::endl;

      // uart.process << boost::typeindex::type_id_with_cvr<TXpin>().pretty_name();
      return uart;
      // while the farm,typeid(RX).name didn't work. Need to do it somehow
   }

   void init(const ::UART_sized<>::Settings& set)
   {
      process << "настройка параметров "<< uart_periph << std::endl;
      base().init(set);
   }

   void transmit() 
   {
      process << "Передача данных:";
      for (int i = 0; i < ::UART_sized<>::buffer.size(); i++)
         process << " " << std::setfill('0') 
                 << std::setw(2) << std::hex << std::uppercase
                 << static_cast<int>(::UART_sized<>::buffer[i]);
      process << std::endl;

      // auto[_1,_2,_3 ... ] = buffer; // Structured bindings

      // if (go_deeper) base().transmit();

   }

   void receive()
   {
      ::UART_sized<>::buffer.clear();
      process << "Очистка буфера, прием данных" << std::endl; // что получает? we don't know
   }

   bool is_tx_complete() {return true;}
   bool is_IDLE()        {return true;}



};

using UART = UART_sized;

} // namespace mock {