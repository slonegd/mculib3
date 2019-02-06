#pragma once

#include "periph.h"
#include "f0_bits_tim.h"
#include "rcc.h"
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
   __IO TIM_bits::BDTR BDTR; // break and dead-time register,           offset: 0x44
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
   enum InterruptMask { 
      ch1 = TIM_DIER_CC1IE_Msk,
      ch2 = TIM_DIER_CC2IE_Msk,
      ch3 = TIM_DIER_CC3IE_Msk,
      ch4 = TIM_DIER_CC4IE_Msk
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
   TIM&     main_output_enable()            { BDTR.MOE = true;    return *this; }
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
   template<Channel> TIM& set_compare (uint32_t);
   template<Channel> __IO uint32_t& get_compare_reference();

   template<Periph, class Pin_> static constexpr PinMode pin_mode();
   template<Periph, class Pin_> static constexpr Channel channel();
   template<Channel>            static constexpr EnableMask    enable_mask();
   template<Channel>            static constexpr InterruptMask interrupt_mask();
   
   
   // // static void     clockEnable()                  {RCC::template clockEnable<template_TIM>();}
   
   

   // void     clearInterruptFlags()           {}
   // 
   // TIM&     set (Trigger v)                 { SMCR.TS  = v;       return *this; }
   // TIM&     set (OnePulseMode v)            { CR1.OPM  = v;       return *this; }
   // TIM&     set (ExtTriggerPolarity v)      { SMCR.ETP = v;       return *this; }

   // template<Channel channel> void preloadEnable  ();
   // template<Channel channel> bool isCompareEnable();

   // static constexpr IRQn_Type IRQn();
   // template <class PIN> static constexpr Channel channel();
   // template <class PIN> static constexpr GPIO::AF AltFunc();


};























template<Periph p> std::enable_if_t<p==Periph::TIM1 , TIM&> make_reference() { return *reinterpret_cast<TIM*>(TIM1_BASE);  }
template<Periph p> std::enable_if_t<p==Periph::TIM3 , TIM&> make_reference() { return *reinterpret_cast<TIM*>(TIM3_BASE);  }
template<Periph p> std::enable_if_t<p==Periph::TIM14, TIM&> make_reference() { return *reinterpret_cast<TIM*>(TIM14_BASE); }
template<Periph p> std::enable_if_t<p==Periph::TIM16, TIM&> make_reference() { return *reinterpret_cast<TIM*>(TIM16_BASE); }
template<Periph p> std::enable_if_t<p==Periph::TIM17, TIM&> make_reference() { return *reinterpret_cast<TIM*>(TIM17_BASE); }

template<Periph p, class Pin_> constexpr PinMode TIM::pin_mode()
{
   if      constexpr (p == Periph::TIM1) {
      if      constexpr (std::is_same_v<Pin_,PA6 >) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PA7 >) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PA8 >) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PA9 >) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PA10>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PA11>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PA12>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PB0 >) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PB1 >) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PB12>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PB13>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PB14>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PB15>) return PinMode::Alternate_2;
      else return PinMode::Input;

   } else if constexpr (p == Periph::TIM3) {
      if      constexpr (std::is_same_v<Pin_,PA6>) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PB4>) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PC6>) return PinMode::Alternate_0;
      else if constexpr (std::is_same_v<Pin_,PA7>) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PB5>) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PC7>) return PinMode::Alternate_0;
      else if constexpr (std::is_same_v<Pin_,PB0>) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PC8>) return PinMode::Alternate_0;
      else if constexpr (std::is_same_v<Pin_,PB1>) return PinMode::Alternate_1;
      else if constexpr (std::is_same_v<Pin_,PC9>) return PinMode::Alternate_0;
      else return PinMode::Input;

   } else if constexpr (p == Periph::TIM14) {
      if      constexpr (std::is_same_v<Pin_,PA4>) return PinMode::Alternate_4;
      else if constexpr (std::is_same_v<Pin_,PA7>) return PinMode::Alternate_4;
      else if constexpr (std::is_same_v<Pin_,PB1>) return PinMode::Alternate_0;
      else return PinMode::Input;

   } else if constexpr (p == Periph::TIM16) {
      if      constexpr (std::is_same_v<Pin_,PA6>) return PinMode::Alternate_5;
      else if constexpr (std::is_same_v<Pin_,PB8>) return PinMode::Alternate_2;
      else return PinMode::Input;

   } else if constexpr (p == Periph::TIM17) {
      if      constexpr (std::is_same_v<Pin_,PA7>) return PinMode::Alternate_5;
      else if constexpr (std::is_same_v<Pin_,PB9>) return PinMode::Alternate_2;
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

template<TIM::Channel c> constexpr TIM::InterruptMask TIM::interrupt_mask()
{
   if      constexpr (c == Channel::_1) return TIM::InterruptMask::ch1;
   else if constexpr (c == Channel::_2) return TIM::InterruptMask::ch2;
   else if constexpr (c == Channel::_3) return TIM::InterruptMask::ch3;
   else if constexpr (c == Channel::_4) return TIM::InterruptMask::ch4;
}

template<Periph p, class Pin_> constexpr TIM::Channel TIM::channel()
{
   if      constexpr (p == Periph::TIM1) {
      if      constexpr (std::is_same_v<Pin_,PA8 >) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PA9 >) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PA10>) return Channel::_3;
      else if constexpr (std::is_same_v<Pin_,PA11>) return Channel::_4;
      else return Channel::error;

   } else if constexpr (p == Periph::TIM3) {
      if      constexpr (std::is_same_v<Pin_,PA6>) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PA7>) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PB0>) return Channel::_3;
      else if constexpr (std::is_same_v<Pin_,PB1>) return Channel::_4;
      else if constexpr (std::is_same_v<Pin_,PB4>) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PC5>) return Channel::_2;
      else return Channel::error;

   } else if constexpr (p == Periph::TIM14) {
      if      constexpr (std::is_same_v<Pin_,PA4>) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PA7>) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PB1>) return Channel::_1;
      else return Channel::error;

   } else if constexpr (p == Periph::TIM16) {
      if      constexpr (std::is_same_v<Pin_,PA6>) return Channel::_1;
      else return Channel::error;

   } else if constexpr (p == Periph::TIM17) {
      if      constexpr (std::is_same_v<Pin_,PA7>) return Channel::_1;
      else return Channel::error;
   } else {
      return Channel::error;
   }
}

template<TIM::Channel c> TIM& TIM::set (Polarity v)
{
   if constexpr (c == Channel::_1) {
      if constexpr (v == Polarity::rising) {
         CCER.CC1P  = false;
         CCER.CC1NP = false;
      } else if constexpr (v == Polarity::falling) {
         CCER.CC1P  = true;
         CCER.CC1NP = false;
      } else if constexpr (v == Polarity::both) {
         CCER.CC1P  = true;
         CCER.CC1NP = true;
      }
      return *this;
   } else if constexpr (c == Channel::_2) {
      if constexpr (v == Polarity::rising) {
         CCER.CC2P  = false;
         CCER.CC2NP = false;
      } else if constexpr (v == Polarity::falling) {
         CCER.CC2P  = true;
         CCER.CC2NP = false;
      } else if constexpr (v == Polarity::both) {
         CCER.CC2P  = true;
         CCER.CC2NP = true;
      }
      return *this;
   } else if constexpr (c == Channel::_3) {
      if constexpr (v == Polarity::rising) {
         CCER.CC3P  = false;
         CCER.CC3NP = false;
      } else if constexpr (v == Polarity::falling) {
         CCER.CC3P  = true;
         CCER.CC3NP = false;
      } else if constexpr (v == Polarity::both) {
         CCER.CC3P  = true;
         CCER.CC3NP = true;
      }
      return *this;
   } else if constexpr (c == Channel::_4) {
      if constexpr (v == Polarity::rising) {
         CCER.CC4P  = false;
         CCER.CC4NP = false;
      } else if constexpr (v == Polarity::falling) {
         CCER.CC4P  = true;
         CCER.CC4NP = false;
      } else if constexpr (v == Polarity::both) {
         CCER.CC4P  = true;
         CCER.CC4NP = true;
      }
      return *this;
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

template<TIM::Channel c> TIM& TIM::set_compare (uint32_t v)
{
   if      constexpr (c == Channel::_1) CCR1 = v;
   else if constexpr (c == Channel::_2) CCR2 = v;
   else if constexpr (c == Channel::_3) CCR3 = v;
   else if constexpr (c == Channel::_4) CCR4 = v;
   return *this;
}

template<TIM::Channel c> __IO uint32_t& TIM::get_compare_reference()
{
   if      constexpr (c == Channel::_1) return *reinterpret_cast<__IO uint32_t*>(&CCR1);
   else if constexpr (c == Channel::_2) return *reinterpret_cast<__IO uint32_t*>(&CCR2);
   else if constexpr (c == Channel::_3) return *reinterpret_cast<__IO uint32_t*>(&CCR3);
   else if constexpr (c == Channel::_4) return *reinterpret_cast<__IO uint32_t*>(&CCR4);
}

} // namespace mcu {