#pragma once

#define USE_MOCK_GPIO
#include "mock_rcc.h"
#if defined(STM32F1)
#include "periph_afio.h"
#endif
#include "periph_gpio.h"
#include <bitset>
#include "process.h"

namespace mock {

std::ostream& operator<< (std::ostream& s, mcu::PinMode v)
{
   return
      v == mcu::PinMode::Input        ? s << "Input"        :
      v == mcu::PinMode::Output       ? s << "Output"       :
   #if defined(STM32F0) or defined(STM32F4) or defined(STM32F7)
      v == mcu::PinMode::Alternate_0  ? s << "Alternate 0"  :
      v == mcu::PinMode::Alternate_1  ? s << "Alternate 1"  :
      v == mcu::PinMode::Alternate_2  ? s << "Alternate 2"  :
      v == mcu::PinMode::Alternate_3  ? s << "Alternate 3"  :
      v == mcu::PinMode::Alternate_4  ? s << "Alternate 4"  :
      v == mcu::PinMode::Alternate_5  ? s << "Alternate 5"  :
      v == mcu::PinMode::Alternate_6  ? s << "Alternate 6"  :
      v == mcu::PinMode::Alternate_7  ? s << "Alternate 7"  :
   #endif
   #if defined(STM32F4) or defined(STM32F7)
      v == mcu::PinMode::Alternate_8  ? s << "Alternate 8"  :
      v == mcu::PinMode::Alternate_9  ? s << "Alternate 9"  :
      v == mcu::PinMode::Alternate_10 ? s << "Alternate 10" :
      v == mcu::PinMode::Alternate_11 ? s << "Alternate 11" :
      v == mcu::PinMode::Alternate_12 ? s << "Alternate 12" :
      v == mcu::PinMode::Alternate_13 ? s << "Alternate 13" :
      v == mcu::PinMode::Alternate_14 ? s << "Alternate 14" :
      v == mcu::PinMode::Alternate_15 ? s << "Alternate 15" :
   #endif
   #if defined(STM32F1)
      v == mcu::PinMode::USART1_RX  ? s << "USART1_RX"  :
      v == mcu::PinMode::USART2_RX  ? s << "USART2_RX"  :
      v == mcu::PinMode::USART3_RX  ? s << "USART3_RX"  :
      v == mcu::PinMode::USART1_TX  ? s << "USART1_TX"  :
      v == mcu::PinMode::USART2_TX  ? s << "USART2_TX"  :
      v == mcu::PinMode::USART3_TX  ? s << "USART3_TX"  :
   #endif
      s;
}

class GPIO : public mcu::GPIO
{
   Process& process { Process::make() };
   GPIO() = default;
public:
   template<mcu::Periph port>
   static GPIO& make()
   {
      static GPIO gpio;
      return gpio;
   }
   friend std::ostream& operator<< (std::ostream&, const GPIO&);

   auto& base() { return *static_cast<mcu::GPIO*>(this); }

   GPIO& set (size_t n) {
      process << "установка вывода " << n << " порта " << *this << std::endl;
      static_cast<mcu::GPIO*>(this)->set(n);
      mock.bsrr_to_idr();
      return *this;
   }

   GPIO& clear (size_t n) {
      process << "сброс вывода " << n << " порта " << *this << std::endl;
      static_cast<mcu::GPIO*>(this)->clear(n);
      mock.bsrr_to_idr();
      return *this;
   }

   GPIO& toggle (size_t n) {
      process << "переключение вывода " << n << " порта " << *this
              << ", а именно " << (this->like_CMSIS().IDR & (1 << n) ? "сброс" : "установка") << std::endl;
      static_cast<mcu::GPIO*>(this)->toggle(n);
      mock.bsrr_to_idr();
      return *this;
   }

   template<class Pin_, mcu::PinMode mode>
   void init() {
      process << "инициализация вывода " << Pin_::n << " порта " << *this << " в режиме " << mode << std::endl;
      static_cast<mcu::GPIO*>(this)->init<Pin_,mode>();
   }

   void atomic_write (uint32_t v)
   {
      base().atomic_write(v);
      mock.bsrr_to_idr();
      std::bitset<32> num(v);
      if (process) *process << *this << " "
      << "pins set: ";
      for (size_t i = 0; i < num.size()/2; ++i) {      
         if (not num[i]) continue;
         if (process) *process << i << " ";  
      }
    
      if (process) *process << "pins clear: ";
      for (size_t i = num.size()/2; i < num.size(); ++i) {      
         if (not num[i]) continue;
         if (process) *process << i - 16 << " ";

      }
      if (process) *process << std::endl;
   }

   struct Mock {
      GPIO& parent;
      Mock (GPIO& parent) : parent{parent} {}
      void set (size_t n, bool v)
      {
         v ? parent.like_CMSIS().IDR |= (1 << n) : parent.like_CMSIS().IDR &= ~(1 << n);
      }
      void bsrr_to_idr()
      {
         auto& cmsis = parent.like_CMSIS();
         cmsis.IDR |= cmsis.BSRR & 0xFFFF;
         cmsis.IDR &= ~((cmsis.BSRR >> 16) & 0xFFFF);
      }
   } mock {*this};


};

std::ostream& operator<< (std::ostream& s, const GPIO& v)
{
   return
      &v == &GPIO::make<mcu::Periph::GPIOA>() ? s << "GPIOA" :
      &v == &GPIO::make<mcu::Periph::GPIOB>() ? s << "GPIOB" :
      &v == &GPIO::make<mcu::Periph::GPIOC>() ? s << "GPIOC" :
      &v == &GPIO::make<mcu::Periph::GPIOD>() ? s << "GPIOD" :
   #if defined(STM32F1)
      &v == &GPIO::make<mcu::Periph::GPIOE>() ? s << "GPIOE" :
   #endif
   #if defined(STM32F0) or defined(STM32F4) or defined(STM32F7)
      &v == &GPIO::make<mcu::Periph::GPIOF>() ? s << "GPIOF" :
   #endif
   #if defined(STM32F4) or defined(STM32F7)
      &v == &GPIO::make<mcu::Periph::GPIOE>() ? s << "GPIOE" :
      &v == &GPIO::make<mcu::Periph::GPIOG>() ? s << "GPIOG" :
      &v == &GPIO::make<mcu::Periph::GPIOH>() ? s << "GPIOH" :
      &v == &GPIO::make<mcu::Periph::GPIOI>() ? s << "GPIOI" :
   #endif
      s;
}

} // namespace mock {

namespace mcu {
   template<Periph p> std::enable_if_t<p == Periph::GPIOA, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOB, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOC, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOD, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
#if defined(STM32F1) or defined(STM32F4) or defined(STM32F7)
   template<Periph p> std::enable_if_t<p == Periph::GPIOE, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
#endif
#if defined(STM32F0) or defined(STM32F4) or defined(STM32F7)
   template<Periph p> std::enable_if_t<p == Periph::GPIOF, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
#endif
#if defined(STM32F4) or defined(STM32F7)
   template<Periph p> std::enable_if_t<p == Periph::GPIOG, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOH, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOI, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
#endif
}

namespace mock {
   auto& pa = REF(GPIOA);
   auto& pb = REF(GPIOB);
   auto& pc = REF(GPIOC);
   auto& pd = REF(GPIOD);
#if defined(STM32F1) or defined(STM32F4) or defined(STM32F7)
   auto& pe = REF(GPIOE);
#endif
#if defined(STM32F0) or defined(STM32F4) or defined(STM32F7)
   auto& pf = REF(GPIOF);
#endif
#if defined(STM32F4) or defined(STM32F7)
   auto& pg = REF(GPIOG);
   auto& ph = REF(GPIOH);
   auto& pi = REF(GPIOI);
#endif
}