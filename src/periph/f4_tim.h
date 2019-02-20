#pragma once

#include "periph.h"
#include "f4_bits_tim.h"
#include "periph_rcc.h"
#include "pin.h"
#include <type_traits>

namespace mcu {

class TIM {
   __IO TIM_bits::CR1  CR1;  // control register 1,                     offset: 0x00
   __IO TIM_bits::CR2  CR2;  // control register 2,                     offset: 0x04
   __IO TIM_bits::SMCR SMCR; // slave Mode Control register,            offset: 0x08
   __IO TIM_bits::DIER DIER; // DMA/interrupt enable register,          offset: 0x0C
   __IO uint32_t       SR;   // status register,                        offset: 0x10
   __IO uint32_t       EGR;  // event generation register,              offset: 0x14
   __IO TIM_bits::CCMR CCMR; // capture/compare mode register,          offset: 0x18
   __IO TIM_bits::CCER CCER; // capture/compare enable register,        offset: 0x20
   __IO uint32_t       CNT;  // counter register,                       offset: 0x24
   __IO uint32_t       PSC;  // prescaler register,                     offset: 0x28
   __IO uint32_t       ARR;  // auto-reload register,                   offset: 0x2C
   __IO uint32_t       RCR;  // repetition counter register,            offset: 0x30
   __IO uint32_t       CCR1; // capture/compare registers 1-4,          offset: 0x34
   __IO uint32_t       CCR2; // capture/compare register 2,             offset: 0x38
   __IO uint32_t       CCR3; // capture/compare register 3,             offset: 0x3C
   __IO uint32_t       CCR4; // capture/compare register 4,             offset: 0x40
   __IO uint32_t       BDTR; // break and dead-time register,           offset: 0x44
   __IO TIM_bits::DCR  DCR;  // DMA control register,                   offset: 0x48
   __IO uint32_t       DMAR; // DMA address for full transfer register, offset: 0x4C
   __IO uint32_t       OR;   // option register,                        offset: 0x50
public:

   template<Periph, class RCC = RCC> static TIM& make_reference();

   using CMSIS_type           = TIM_TypeDef;
   using CompareMode          = TIM_bits::Output_t::CompareMode;
   // using OnePulseMode         = TIM_bits::CR1::OnePulseMode;
   // using Direction            = TIM_bits::CR1::Direction;
   using SlaveMode            = TIM_bits::SMCR::SlaveMode;
   // using Trigger              = TIM_bits::SMCR::Trigger;
   // using ExtTriggerPolarity   = TIM_bits::SMCR::ExtTriggerPolarity;
   using SelectionCompareMode = TIM_bits::SelectionCompareMode;
   using Polarity             = TIM_bits::CCER::Polarity;
   enum Channel {_1 = 1, _2, _3, _4, error};
   enum EnableMask { 
      ch1 = TIM_CCER_CC1E_Msk,
      ch2 = TIM_CCER_CC2E_Msk,
      ch3 = TIM_CCER_CC3E_Msk,
      ch4 = TIM_CCER_CC4E_Msk
   };

   TIM&     counter_enable()                { CR1.CEN = true;     return *this; }
   TIM&     counter_disable()               { CR1.CEN = false;    return *this; }
   TIM&     clear_counter()                 { CNT = 0;            return *this; }
   TIM&     set_counter (uint16_t v)        { CNT = v;            return *this; }
   TIM&     set_compare (uint16_t v)        { CCR1 = v;           return *this; }
   TIM&     ext_clock_enable()              { SMCR.ECE = true;    return *this; }
   TIM&     ext_clock_disable()             { SMCR.ECE = false;   return *this; }
   TIM&     set_prescaller (uint16_t v)     { PSC = v;            return *this; }
   TIM&     auto_reload_enable()            { CR1.ARPE = true;    return *this; }
   TIM&     set (SlaveMode v)               { SMCR.SMS = v;       return *this; }
   TIM&     set_auto_reload  (uint16_t v)   { ARR = v;            return *this; }
   TIM&     compare_enable   (uint32_t v)   { *reinterpret_cast<__IO uint32_t*>(&CCER) |=  v; return *this; }
   TIM&     compare_disable  (uint32_t v)   { *reinterpret_cast<__IO uint32_t*>(&CCER) &= ~v; return *this; }
   TIM&     interrupt_enable (uint32_t v)   { *reinterpret_cast<__IO uint32_t*>(&DIER) |=  v; return *this; }
   TIM&     interrupt_disable(uint32_t v)   { *reinterpret_cast<__IO uint32_t*>(&DIER) &= ~v; return *this; }

   bool     is_count()                      { return CR1.CEN; }
   uint16_t get_counter()                   { return CNT;     }

   template<Channel> TIM& set (Polarity);
   template<Channel> TIM& set (CompareMode);
   template<Channel> TIM& set (SelectionCompareMode);
   template<Channel> TIM& preload_enable ();
   template<Channel> TIM& preload_disable();
   template<Channel> TIM& compare_enable ();
   template<Channel> TIM& compare_disable();
   template<Channel> TIM& compareToggle  ();
   TIM& set_compare (Channel, uint16_t);
   template<Channel>  __IO uint32_t& get_compare_reference();

   template<Periph, class Pin_> static constexpr PinMode pin_mode();
   template<Periph, class Pin_> static constexpr Channel channel();
   template<Channel>            static constexpr EnableMask    enable_mask();
};

template<Periph p> std::enable_if_t<p==Periph::TIM1 , TIM&> make_reference() { return *reinterpret_cast<TIM*>(TIM1_BASE);  }
template<Periph p> std::enable_if_t<p==Periph::TIM2 , TIM&> make_reference() { return *reinterpret_cast<TIM*>(TIM2_BASE);  }
template<Periph p> std::enable_if_t<p==Periph::TIM3 , TIM&> make_reference() { return *reinterpret_cast<TIM*>(TIM3_BASE);  }
template<Periph p> std::enable_if_t<p==Periph::TIM14, TIM&> make_reference() { return *reinterpret_cast<TIM*>(TIM14_BASE); }

template<Periph p, class Pin_> constexpr PinMode TIM::pin_mode()
{
   if      constexpr (p == Periph::TIM2) {
      if      constexpr (std::is_same_v<Pin_,PA0 >) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PA1 >) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PA2 >) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PA3 >) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PA5 >) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PA15>) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PB3 >) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PB10>) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PB11>) return PinMode::Alternate_1;
      else return PinMode::Input;

   } else if constexpr (p == Periph::TIM3) {
      if      constexpr (std::is_same_v<Pin_,PA6>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PA7>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PB0>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PB1>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PB4>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PB5>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PC6>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PC7>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PC8>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PC9>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PD2>) return PinMode::Alternate_2;
      else return PinMode::Input;

   } else {
      return PinMode::Input;
   }
}

template<TIM::Channel c> constexpr TIM::EnableMask TIM::enable_mask()
{
   if      constexpr (c == Channel::_1) return TIM::EnableMask::ch1;
   else if constexpr (c == Channel::_2) return TIM::EnableMask::ch2;
   else if constexpr (c == Channel::_3) return TIM::EnableMask::ch3;
   else if constexpr (c == Channel::_4) return TIM::EnableMask::ch4;
}

template<Periph p, class Pin_> constexpr TIM::Channel TIM::channel()
{
   if      constexpr (p == Periph::TIM2) {
      if      constexpr (std::is_same_v<Pin_,PA0 >) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PA1 >) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PA2 >) return Channel::_3;
      else if constexpr (std::is_same_v<Pin_,PA3 >) return Channel::_4;
      else if constexpr (std::is_same_v<Pin_,PA5 >) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PA15>) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PB3 >) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PB10>) return Channel::_3;
      else if constexpr (std::is_same_v<Pin_,PB11>) return Channel::_4;
      else return Channel::error;

   } else if constexpr (p == Periph::TIM3) {
      if      constexpr (std::is_same_v<Pin_,PA6>) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PA7>) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PB0>) return Channel::_3;
      else if constexpr (std::is_same_v<Pin_,PB1>) return Channel::_4;
      else if constexpr (std::is_same_v<Pin_,PB4>) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PB5>) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PC6>) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PC7>) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PC8>) return Channel::_3;
      else if constexpr (std::is_same_v<Pin_,PC9>) return Channel::_4;
      else return Channel::error;

   } else {
      return Channel::error;
   }
}

template<TIM::Channel c> TIM& TIM::set (CompareMode v)
{
   if      constexpr (c == Channel::_1) { CCMR.output.OC1M = v; return *this; }
   else if constexpr (c == Channel::_2) { CCMR.output.OC2M = v; return *this; }
   else if constexpr (c == Channel::_3) { CCMR.output.OC3M = v; return *this; }
   else if constexpr (c == Channel::_4) { CCMR.output.OC4M = v; return *this; }
}

template<TIM::Channel c> TIM& TIM::set (SelectionCompareMode v)
{
   if      constexpr (c == Channel::_1) { CCMR.output.CC1S = v; return *this; }
   else if constexpr (c == Channel::_2) { CCMR.output.CC2S = v; return *this; }
   else if constexpr (c == Channel::_3) { CCMR.output.CC3S = v; return *this; }
   else if constexpr (c == Channel::_4) { CCMR.output.CC4S = v; return *this; }
}

template<TIM::Channel c> TIM& TIM::preload_enable()
{
   if      constexpr (c == Channel::_1)  { CCMR.output.OC1PE = true; return *this; }
   else if constexpr (c == Channel::_2)  { CCMR.output.OC2PE = true; return *this; }
   else if constexpr (c == Channel::_3)  { CCMR.output.OC3PE = true; return *this; }
   else if constexpr (c == Channel::_4)  { CCMR.output.OC4PE = true; return *this; }
}

template<TIM::Channel c> TIM& TIM::compare_enable()
{
   if      constexpr (c == Channel::_1) CCER.CC1E = true;
   else if constexpr (c == Channel::_2) CCER.CC2E = true;
   else if constexpr (c == Channel::_3) CCER.CC3E = true;
   else if constexpr (c == Channel::_4) CCER.CC4E = true;
   return *this;
}

template<TIM::Channel c> TIM& TIM::compare_disable()
{
   if      constexpr (c == Channel::_1) CCER.CC1E = false;
   else if constexpr (c == Channel::_2) CCER.CC2E = false;
   else if constexpr (c == Channel::_3) CCER.CC3E = false;
   else if constexpr (c == Channel::_4) CCER.CC4E = false;
   return *this;
}

TIM& TIM::set_compare (Channel c, uint16_t v)
{
   if      (c == Channel::_1) CCR1 = v;
   else if (c == Channel::_2) CCR2 = v;
   else if (c == Channel::_3) CCR3 = v;
   else if (c == Channel::_4) CCR4 = v;
   return *this;
}

template<TIM::Channel c> __IO uint32_t& TIM::get_compare_reference()
{
   if      constexpr (c == Channel::_1) return *reinterpret_cast<__IO uint32_t*>(&CCR1);
   else if constexpr (c == Channel::_2) return *reinterpret_cast<__IO uint32_t*>(&CCR2);
   else if constexpr (c == Channel::_3) return *reinterpret_cast<__IO uint32_t*>(&CCR3);
   else if constexpr (c == Channel::_4) return *reinterpret_cast<__IO uint32_t*>(&CCR4);
}

} //namespace mcu 