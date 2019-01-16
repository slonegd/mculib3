#pragma once

#define USE_MOCK_USART
#include "periph_usart.h"
#include <iostream>

namespace mock {

std::ostream& operator<< (std::ostream& s, mcu::USART::Parity v)
{
   return
      v == mcu::USART::Parity::even  ? s << "четность"   :
      v == mcu::USART::Parity::odd   ? s << "нечетность" : s;
}

std::ostream& operator<< (std::ostream& s, mcu::USART::DataBits v)
{
   return
      v == mcu::USART::DataBits::_8 ? s << "8"   :
      v == mcu::USART::DataBits::_9 ? s << "9" : s;
}

std::ostream& operator<< (std::ostream& s, mcu::USART::StopBits v)
{
   return
      v == mcu::USART::StopBits::_0_5 ? s << "0.5" :
      v == mcu::USART::StopBits::_1   ? s << "1"   :
      v == mcu::USART::StopBits::_1_5 ? s << "1.5" :
      v == mcu::USART::StopBits::_2   ? s << "2"   : s;
}

std::ostream& operator<< (std::ostream& s, mcu::USART::Baudrate v)
{
   return
      v == mcu::USART::Baudrate::BR115200 ? s << 115200 :
      v == mcu::USART::Baudrate::BR14400  ? s << 14400  :
      v == mcu::USART::Baudrate::BR19200  ? s << 19200  :
      v == mcu::USART::Baudrate::BR28800  ? s << 28800  :
      v == mcu::USART::Baudrate::BR38400  ? s << 38400  :
      v == mcu::USART::Baudrate::BR57600  ? s << 57600  :
      v == mcu::USART::Baudrate::BR76800  ? s << 76800  :
      v == mcu::USART::Baudrate::BR9600   ? s << 9600   : s;
}


class USART : public mcu::USART
{
   std::ostream* process {nullptr};
   USART() = default;
public:
   template<mcu::Periph p>
   static USART& make()
   {
      static USART usart;
      return usart;
   }
   void set_stream (std::ostream& s) { process = &s; }
   friend std::ostream& operator<< (std::ostream& s, const USART& v);

   auto& base() { return *static_cast<mcu::USART*>(this); }

   USART& set (Parity v)
   {
      if (process) *process << *this << ": Задана проверка на " << v << std::endl;
      base().set(v);
      return *this;
   }

   USART& set (DataBits v)
   {
      if (process) *process << *this << ": Установлен размер пакета " << v << " бит" << std::endl;
      base().set(v);
      return *this;
   }

   USART& set (StopBits v)
   {
      if (process) *process << *this << ": Установлено количество стоп битов: " << v << std::endl;
      base().set(v);
      return *this;
   }

   USART& set (Baudrate v, mcu::Periph p)
   {
      if (process) *process << *this << ": Установлена скорость " << v << " бит/с" << std::endl;
      base().set(v, p);
      return *this;
   }

   USART& enable()
   {
      if (process) *process << *this << ": Разрешение работы" << std::endl;
      base().enable();
      return *this;
   }

   USART& disable()
   {
      if (process) *process << *this << ": Запрет работы" << std::endl;
      base().disable();
      return *this;
   }

   USART& rx_enable()
   {
      if (process) *process << *this << ": Разрешение приёма" << std::endl;
      base().rx_enable();
      return *this;
   }

   USART& rx_disable()
   {
      if (process) *process << *this << ": Запрет приёма" << std::endl;
      base().rx_disable();
      return *this;
   }

   USART& tx_enable()
   {
      if (process) *process << *this << ": Разрешение отправки" << std::endl;
      base().tx_enable();
      return *this;
   }

   USART& tx_disable()
   {
      if (process) *process << *this << ": Запрет отправки" << std::endl;
      base().tx_disable();
      return *this;
   }

   USART& rts_enable()
   {
      if (process) *process << *this << ": Разрешение аппаратного rts" << std::endl;
      base().rts_enable();
      return *this;
   }

   USART& rts_disable()
   {
      if (process) *process << *this << ": Запрет аппаратного rts" << std::endl;
      base().rts_disable();
      return *this;
   }

   USART& DMA_tx_enable()
   {
      if (process) *process << *this << ": Разрешение DMA на отправку" << std::endl;
      base().DMA_tx_enable();
      return *this;
   }

   USART& DMA_rx_enable()
   {
      if (process) *process << *this << ": Разрешение DMA на приём" << std::endl;
      base().DMA_rx_enable();
      return *this;
   }

   USART& parity_enable()
   {
      if (process) *process << *this << ": Включение проверки на чётность" << std::endl;
      base().parity_enable();
      return *this;
   }

   USART& parity_disable()
   {
      if (process) *process << *this << ": Отключение проверки на чётность" << std::endl;
      base().parity_disable();
      return *this;
   }

   USART& enable_IDLE_interrupt()
   {
      if (process) *process << *this << ": Разрешение прерывания по простою" << std::endl;
      base().enable_IDLE_interrupt();
      return *this;
   }

   USART& enable_tx_complete_interrupt()
   {
      if (process) *process << *this << ": Разрешение прерывания по окончанию отправки" << std::endl;
      base().enable_tx_complete_interrupt();
      return *this;
   }

   USART& disable_tx_complete_interrupt()
   {
      if (process) *process << *this << ": Запрет прерывания по окончанию отправки" << std::endl;
      base().enable_tx_complete_interrupt();
      return *this;
   }

   USART& clear_interrupt_flags()
   {
      if (process) *process << *this << ": Очищение флагов прерываний" << std::endl;
      base().clear_interrupt_flags();
      return *this;
   }

};

std::ostream& operator<< (std::ostream& s, const USART& v)
{
   return
      &v == &USART::make<mcu::Periph::USART1>() ? s << "USART1" :
   #if defined(STM32F1) or defined(STM32F4)
      &v == &USART::make<mcu::Periph::USART2>() ? s << "USART2" :
      &v == &USART::make<mcu::Periph::USART3>() ? s << "USART3" :
   #endif
   #if defined(STM32F4)
      &v == &USART::make<mcu::Periph::USART4>() ? s << "USART4" :
      &v == &USART::make<mcu::Periph::USART5>() ? s << "USART5" :
      &v == &USART::make<mcu::Periph::USART6>() ? s << "USART6" :
   #endif
      s;
}

} // namespace mock {

namespace mcu {
   SFINAE(USART1,mock::USART) make_reference() {return mock::USART::make<mcu::Periph::USART1>();}
#if defined(STM32F1) or defined(STM32F4)
   SFINAE(USART2,mock::USART) make_reference() {return mock::USART::make<mcu::Periph::USART2>();}
   SFINAE(USART3,mock::USART) make_reference() {return mock::USART::make<mcu::Periph::USART3>();}
#endif
#if defined(STM32F4)
   SFINAE(USART4,mock::USART) make_reference() {return mock::USART::make<mcu::Periph::USART4>();}
   SFINAE(USART5,mock::USART) make_reference() {return mock::USART::make<mcu::Periph::USART5>();}
   SFINAE(USART6,mock::USART) make_reference() {return mock::USART::make<mcu::Periph::USART6>();}
#endif
}