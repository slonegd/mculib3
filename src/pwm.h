#pragma once

#include "tim.h"
#include "pin.h"
#include "literals.h"

namespace mcu {

template<class Parent, class T>
struct Property {
   T value {};
   Parent& parent;
   Property(Parent& p) : parent{p} {}
};

class PWM
{
   TIM& tim;
   Pin  pin;
   uint16_t        const max_fill;
   TIM::EnableMask const enable_mask;
   __IO uint32_t& compare_value;
   constexpr static uint32_t min_frequency {F_CPU / 0xFFFF};
public:
   /// отдельные классы параметров, так как изменения влияют на другие параметры
   /// частота в Гц
   struct Frequency : private Property<PWM, uint16_t> {
      Frequency  (PWM& parent) : Property{parent} {}
      operator uint16_t() { return value; }
      inline Frequency& operator=  (uint16_t);
      inline Frequency& operator+= (int v) { return operator= (value + v); }
      /// задаёт значениe, ничего не пересчитывая
      inline void  update     (uint16_t v) { value = v; }
   } frequency {*this};

   /// коэффициент заполения в от 0 до 1000
   struct FillRatio : private Property<PWM, uint16_t> {
      FillRatio  (PWM& parent) : Property{parent} {}
      operator uint16_t() { return value; }
      inline FillRatio& operator=  (uint16_t);
      inline FillRatio& operator+= (int v) { return operator= (value + v); }
      inline void update();
   } fill_ratio {*this};

   /// значение при котором счётчик сбрасывается 
   struct CountTo : private Property<PWM, uint16_t> {
      CountTo  (PWM& parent) : Property{parent} {}
      operator uint16_t() { return value; }
      inline CountTo& operator=  (uint16_t);
      inline CountTo& operator+= (int v) { return operator= (value + v); }
      /// задаёт значениe, ничего не пересчитывая
      void update (uint16_t);
   } count_to {*this};


   PWM (
      TIM&            tim,
      Pin             pin,
      uint16_t        max_fill,
      TIM::EnableMask enable_mask,
      __IO uint32_t&  compare_value
   )  : tim           {tim}
      , pin           {pin}
      , max_fill      {max_fill}
      , enable_mask   {enable_mask}
      , compare_value {compare_value}
   { }

   template<Periph tim_, class Pin_>
   static PWM make (int16_t max_fill = 500_from1000)
   {
      constexpr auto channel = TIM::channel<tim_, Pin_>();
      constexpr auto pin_mode = TIM::pin_mode<tim_, Pin_>();
      static_assert (
         channel != TIM::Channel::error,
         "Вывод контроллера не поддерживает функцию ШИМ с этим таймером"
      );
      PWM pwm {
         make_reference<tim_>(),
         Pin::make<Pin_, pin_mode>(),
         max_fill,
         TIM::enable_mask<channel>(),
         tim.template get_compare_reference<channel>()
      };
      make_reference<Periph::RCC>().clock_enable<tim_>();
      pwm.tim.template set<channel> (TIM::CompareMode::PWMmode)
             .template preload_enable<channel>()
             .auto_reload_enable()
             .main_output_enable()
             .counter_enable();
      pwm.frequency = pwm.min_frequency;
      return pwm;
   }

   /// следующие управляют выходом ШИМ
   void out_enable()  { tim.compare_enable (enable_mask); }
   void out_disable() { tim.compare_disable(enable_mask); }


};
















///////////////////////////////////////////////////////////////////////////////
// ЧАСТОТА
///////////////////////////////////////////////////////////////////////////////
PWM::Frequency& PWM::Frequency::operator= (uint16_t v)
{
   if ( (v != value) and (v >= parent.min_frequency) ) {
      value = v;
      parent.count_to.update (F_CPU / value - 1);
   }
   return *this;
}


///////////////////////////////////////////////////////////////////////////////
// Коэффициент заполнения
///////////////////////////////////////////////////////////////////////////////
PWM::FillRatio& PWM::FillRatio::operator= (uint16_t v)
{
   if ( (v != value) and (v > 0) and (v <= parent.max_fill) ) {
      value = v;
      update();
   }
   return *this;
}

void PWM::FillRatio::update()
{
   parent.compare_value = parent.count_to * value / 1000;
}


///////////////////////////////////////////////////////////////////////////////
// СБРОС СЧЁТЧИКА
///////////////////////////////////////////////////////////////////////////////
PWM::CountTo& PWM::CountTo::operator= (uint16_t v)
{
   if ( (v != value) and (v >= 0) and (v <= 0xFFFF) ) {
      value = v;
      update (value);
      parent.frequency.update (F_CPU / (value + 1));
   }
   return *this;
}

void PWM::CountTo::update (uint16_t v)
{
   value = v;
   parent.tim.set_auto_reload (v);
   parent.fill_ratio.update();
}

} // namespace mcu {


