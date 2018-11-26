#pragma once

#include <type_traits>
#include "mock_pin.h"
#include "test_periph.h"
#include "test_NVIC_Enable.h"

uint8_t buffer_[2];
constexpr int qty = 8;

namespace mcu {
    
class USART
{
public:

   enum Parity         { even = 0b0, odd     };
   enum WakeMethod     { idle = 0b0, address };
   enum DataBits       { _8   = 0b0, _9      };
   enum BreakDetection { _10bit = 0b0, _11bit};
   enum StopBits       { _1 = 0b00, _0_5, _2, _1_5 };

   enum Baudrate {
      BR9600   = 0b000,
      BR14400  = 0b001,
      BR19200  = 0b010,
      BR28800  = 0b011,
      BR38400  = 0b100,
      BR57600  = 0b101,
      BR76800  = 0b110,
      BR115200 = 0b111
   };

   static size_t clock_;

   USART& set (Parity v)  {result << (v == Parity::even ? "Задана проверка на четность" : 
                                                          "Задана проверка на нечетность") 
                                                          << '\n'; return *this;}
   USART& set (DataBits v){result << (v == DataBits::_8 ? "Установлен размер пакета 8 бит" :
                                                          "Установлен размер пакета 9 бит") 
                                                          << '\n'; return *this;}
   USART& set (StopBits v)
   { 
      if (v == StopBits::_1) {
         result << "УСтановлен один стоповый бит" << '\n';
      } else if (v == StopBits::_2) {
         result << "УСтановлено два стоповых бит" << '\n';
      }
      return *this;
   }
   USART& set (Baudrate v, size_t)
   {
      if (v == Baudrate::BR9600) {
         result << "Установлена скорость 9600 бит/с" << '\n';
      } else if (v == Baudrate::BR14400) {
         result << "Установлена скорость 14400 бит/с" << '\n';
      }
      return *this;
   }

   USART& rx_enable (){result << "USART Receiver enable"     << '\n'; return *this;}
   USART& rx_disable(){result << "USART Receiver disable"    << '\n'; return *this;}
   USART& tx_enable (){result << "USART Transmitter enable"  << '\n'; return *this;}
   USART& tx_disable(){result << "USART Transmitter disable" << '\n'; return *this;}

   USART& DMA_tx_enable (){result << "DMA enable transmitter" << '\n'; return *this;}
   USART& DMA_rx_enable (){result << "DMA enable receiver"    << '\n'; return *this;}

   USART& enable_IDLE_interrupt(){result << "USART IDLE interrupt enable" << '\n'; return *this;}
   USART& disable_tx_complete_interrupt  (){return *this;}
   USART& enable(){result << "USART enable" << '\n'; return *this;}
   USART& clear_interrupt_flags(){ result << "Очищаем флаги прерываний uart" << '\n'; return *this;}

   bool    is_IDLE_interrupt              (){return true;}
   bool    is_tx_complete                 (){return true;}
   bool    is_tx_complete_interrupt_enable(){return true;}

   size_t transmit_data_adr(){result << "Передаем адрес регистра данных" << '\n'; return 0xff;}

   IRQn_Type IRQn(Periph usart){result << "Определили номер прерывания USART" << '\n'; return USART1_IRQn;}
   template<Periph usart, Periph rcc = Periph::TEST_RCC> USART& clock_enable(){result << "Включаем тактирование переферии usart" << '\n'; return *this;}
   template<Periph usart, Periph rcc = Periph::TEST_RCC> static size_t clock(){result << "Передаем значение clock" << '\n'; return clock_;}

   template<Periph usart, class Pin> static constexpr Periph stream()
   {
      if constexpr (usart == Periph::TEST_USART) {
         if      constexpr (std::is_same_v<Pin, TX>) {/*result << "Определили поток №1" << '\n';*/ return Periph::TEST_DMA_stream1;}
         else if constexpr (std::is_same_v<Pin, RX>) {/*result << "Определили поток №2" << '\n';*/ return Periph::TEST_DMA_stream2;}
      }
   }
   template<Periph usart, class Pin> static constexpr PinMode pin_mode()
   {
      if constexpr (usart == Periph::TEST_USART) {
      	if      constexpr (std::is_same_v<Pin, TX>) return PinMode::USART1_TX; 
      	else if constexpr (std::is_same_v<Pin, RX>) return PinMode::USART1_RX;
      	else return PinMode::Input;
   	}
   }
   template<Periph usart, class TXpin, class RXpin> static void pin_static_assert(){
      result << "Проверяем назначение пинов" << '\n';
      }


}mock_usart;

size_t USART::clock_{1000};

template <Periph p> std::enable_if_t<p == Periph::TEST_USART, USART&> make_reference()
{
   result << "Создали ссылку на переферию usart" << '\n';
   return mock_usart;}

} //namespace mcu