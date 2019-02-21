#pragma once

#define USE_MOCK_TIM
#include "periph_tim.h"
#include "process.h"

namespace mock {

std::ostream& operator<< (std::ostream& s, mcu::TIM::SlaveMode v)
{
   return
      v == mcu::TIM::SlaveMode::SMDisabled    ? s << "слейв отключен"        :
      v == mcu::TIM::SlaveMode::Encoder1      ? s << "энкодера 1"            : 
      v == mcu::TIM::SlaveMode::Encoder2      ? s << "энкодера 2"            : 
      v == mcu::TIM::SlaveMode::Encoder3      ? s << "энкодера 3"            : 
      v == mcu::TIM::SlaveMode::Reset         ? s << "сброса"                : 
      v == mcu::TIM::SlaveMode::Gated         ? s << "стробированный"        : 
      v == mcu::TIM::SlaveMode::TriggerMode   ? s << "тригеррный "           : 
      v == mcu::TIM::SlaveMode::ExternalClock ? s << "внешнего тактирования" : s;
}

std::ostream& operator<< (std::ostream& s, mcu::TIM::Channel v)
{
   return
      v == mcu::TIM::Channel::_1  ? s << "канала №1 " :
      v == mcu::TIM::Channel::_2  ? s << "канала №2 " : 
      v == mcu::TIM::Channel::_3  ? s << "канала №3 " : 
      v == mcu::TIM::Channel::_4  ? s << "канала №4 " : s;
}

std::ostream& operator<< (std::ostream& s, mcu::TIM::Polarity v)
{
   return
      v == mcu::TIM::Polarity::rising   ? s << "по фронту"         :
      v == mcu::TIM::Polarity::falling  ? s << "по спаду"          : 
      v == mcu::TIM::Polarity::both     ? s << "по фронту и спаду" : s;
}

std::ostream& operator<< (std::ostream& s, mcu::TIM::CompareMode v)
{
   return
      v == mcu::TIM::CompareMode::Off             ? s << "отключен"           :
      v == mcu::TIM::CompareMode::ActiveOnMatch   ? s << "высокий уровень при совпадении"     : 
      v == mcu::TIM::CompareMode::InactiveOnMatch ? s << "низкий уровень при совпадении"      : 
      v == mcu::TIM::CompareMode::ToggleOnMatch   ? s << "переключения при совпадении"  :
      v == mcu::TIM::CompareMode::ForceInactive   ? s << "принудительно низкий уровень"       :
      v == mcu::TIM::CompareMode::ForceActive     ? s << "принудительно высокий уровень"      : 
      v == mcu::TIM::CompareMode::PWMmode         ? s << "ШИМ"                          : 
      v == mcu::TIM::CompareMode::InvertedPWMmode ? s << "ШИМ инвертированный"          : s;
}

std::ostream& operator<< (std::ostream& s, mcu::TIM::SelectionCompareMode v)
{
   return
      v == mcu::TIM::SelectionCompareMode::Output   ? s << "выход"           :
      v == mcu::TIM::SelectionCompareMode::Input    ? s << "вход"     : 
      v == mcu::TIM::SelectionCompareMode::InputALT ? s << "альтернативный вход"      : 
      v == mcu::TIM::SelectionCompareMode::InputTRC ? s << "тригеррный вход"          : s;
}

class TIM : public mcu::TIM
{
   Process& process { Process::make() };
   TIM() = default;
public:
   template<mcu::Periph p>
   static TIM& make()
   {
      static TIM tim;
      return tim;
   }
   friend std::ostream& operator<< (std::ostream&, const TIM&);

   auto& base() { return *static_cast<mcu::TIM*>(this); }

   TIM& counter_enable()
   {
      process << *this << ": Разрешение работы" << std::endl;
      base().counter_enable();
      return *this; 
   }

   TIM& counter_disable()
   {
      process << *this << ": Запрет работы" << std::endl;
      base().counter_disable();
      return *this; 
   }

   TIM& clear_counter()
   {
      process << *this << ": регистр CNT обнулен" << std::endl;
      base().clear_counter();
      return *this; 
   }

   TIM& set_counter (uint16_t v)
   {
      process << *this << ": CNT = " << v << std::endl;
      base().set_counter(v);
      return *this; 
   }

   TIM& ext_clock_enable ()
   {
      process << *this << ": Включение внешнего тактирования" << std::endl;
      base().ext_clock_enable();
      return *this; 
   }

   TIM& ext_clock_disable ()
   {
      process << *this << ": Отключение внешнего тактирования" << std::endl;
      base().ext_clock_disable();
      return *this; 
   }

   TIM& set_prescaller (uint16_t v)
   {
      process << *this << ": PSC = " << v << std::endl;
      base().set_prescaller(v);
      return *this; 
   }

   TIM& auto_reload_enable()
   {
      process << *this << ": Предзагрузка разрешена" << std::endl;
      base().auto_reload_enable();
      return *this; 
   }

   TIM& set (SlaveMode v) 
   {
      process << *this << ": Установлен режим слейва =  " << v << std::endl;
      base().set(v);
      return *this; 
   }

   TIM& set_auto_reload (uint16_t v)
   {
      process << *this << ": ARR = " << v << std::endl;
      base().set_auto_reload(v);
      return *this; 
   }

   TIM& compare_enable (uint32_t v)
   {
      process << *this;

      if (v == TIM::EnableMask::ch1)
         process << ": Разрешена работа первого канала";
      else if (v == TIM::EnableMask::ch2)
         process << ": Разрешена работа второго канала";
      else if (v == TIM::EnableMask::ch3)
         process << ": Разрешена работа третьего канала";
      else if (v == TIM::EnableMask::ch4)
         process << ": Разрешена работа четвертого канала";

      process << std::endl;

      base().compare_enable(v);
      return *this; 
   }

   TIM& compare_disable (uint32_t v)
   {
      process << *this;

      if (v == TIM::EnableMask::ch1)
         process << ": Работа первого канала запрещена";
      else if (v == TIM::EnableMask::ch2)
         process << ": Работа второго канала запрещена";
      else if (v == TIM::EnableMask::ch3)
         process << ": Работа третьего канала запрещена";
      else if (v == TIM::EnableMask::ch4)
         process << ": Работа четвертого канала запрещена";

      process << std::endl;

      base().compare_disable(v);
      return *this; 
   }

   template<TIM::Channel c> TIM& set (Polarity v)
   {
      process << *this << ": Полярность" << c << v << std::endl;
      base().set<c>(v);
      return *this; 
   }

   template<TIM::Channel c> TIM& set (CompareMode v)
   {
      process << *this << ": Режим сравнения " << c << v << std::endl;
      base().set<c>(v);
      return *this; 
   }

   template<TIM::Channel c> TIM& set (SelectionCompareMode v)
   {
      process << *this << ": Режим сравнения " << c << v << std::endl;
      base().set<c>(v);
      return *this; 
   }

   template<TIM::Channel c> TIM& preload_enable()
   {
      process << *this << ": Разрешение работы предзагрузочног регистра " << c << std::endl;
      base().preload_enable<c>();
      return *this; 
   }

   template<TIM::Channel c> TIM& preload_disable()
   {
      process << *this << ": Разрешение работы предзагрузочног регистра " << c << std::endl;
      base().preload_disable<c>();
      return *this; 
   }

   TIM& set_compare (Channel c, uint16_t v)
   {
      process << *this << ": Значение сравнения " << c << "= " << v << std::endl;
      base().set_compare(c, v);
      return *this; 
   }

};

std::ostream& operator<< (std::ostream& s, const TIM& v)
{
   return
      &v == &TIM::make<mcu::Periph::TIM3>() ? s << "TIM3" :
   #if defined(STM32F0)
      &v == &TIM::make<mcu::Periph::TIM1>() ? s << "TIM1" :
   #endif
   #if defined(STM32F4)
      &v == &TIM::make<mcu::Periph::TIM1>() ? s << "TIM2" :
   #endif
      s;
}

} //namespace mock

namespace mcu {
   SFINAE(TIM3,mock::TIM) make_reference() {return mock::TIM::make<mcu::Periph::TIM3>();}
#if defined(STM32F0)
   SFINAE(TIM1,mock::TIM) make_reference() {return mock::TIM::make<mcu::Periph::TIM1>();}
#endif
#if defined(STM32F4)
   SFINAE(TIM2,mock::TIM) make_reference() {return mock::TIM::make<mcu::Periph::TIM2>();}
#endif
}

namespace mock {
   auto& tim3 = REF(TIM3);
#if defined(STM32F0)
   auto& tim1 = REF(TIM1);
#endif
#if defined(STM32F4)
   auto& tim2 = REF(TIM2);
#endif
}