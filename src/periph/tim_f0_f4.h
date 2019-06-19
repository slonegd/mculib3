#pragma once

#include "periph.h"
#include "f0_f4_bits_tim.h"
#include "periph_rcc.h"
#include "pin.h"
#include <type_traits>

namespace mcu {

class TIM {
   volatile TIM_bits::CR1  CR1;  // control register 1,                     offset: 0x00
   volatile TIM_bits::CR2  CR2;  // control register 2,                     offset: 0x04
   volatile TIM_bits::SMCR SMCR; // slave Mode Control register,            offset: 0x08
   volatile TIM_bits::DIER DIER; // DMA/interrupt enable register,          offset: 0x0C
   volatile uint32_t       SR;   // status register,                        offset: 0x10
   volatile uint32_t       EGR;  // event generation register,              offset: 0x14
   volatile TIM_bits::CCMR CCMR; // capture/compare mode register,          offset: 0x18
   volatile TIM_bits::CCER CCER; // capture/compare enable register,        offset: 0x20
   volatile uint32_t       CNT;  // counter register,                       offset: 0x24
   volatile uint32_t       PSC;  // prescaler register,                     offset: 0x28
   volatile uint32_t       ARR;  // auto-reload register,                   offset: 0x2C
   volatile uint32_t       RCR;  // repetition counter register,            offset: 0x30
   volatile uint32_t       CCR1; // capture/compare registers 1-4,          offset: 0x34
   volatile uint32_t       CCR2; // capture/compare register 2,             offset: 0x38
   volatile uint32_t       CCR3; // capture/compare register 3,             offset: 0x3C
   volatile uint32_t       CCR4; // capture/compare register 4,             offset: 0x40
#if defined(STM32F0)
   volatile TIM_bits::BDTR BDTR; // break and dead-time register,           offset: 0x44
#elif defined(STM32F4)
   volatile uint32_t       res;  // break and dead-time register,           offset: 0x44
#endif
   volatile TIM_bits::DCR  DCR;  // DMA control register,                   offset: 0x48
   volatile uint32_t       DMAR; // DMA address for full transfer register, offset: 0x4C
   volatile uint32_t       OR;   // option register,                        offset: 0x50
public:
   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

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
   // enum InterruptMask { 
   //    ch1 = TIM_DIER_CC1IE_Msk,
   //    ch2 = TIM_DIER_CC2IE_Msk,
   //    ch3 = TIM_DIER_CC3IE_Msk,
   //    ch4 = TIM_DIER_CC4IE_Msk
   // };

   
   TIM&     counter_enable()                { CR1.CEN = true;     return *this; }
   TIM&     counter_disable()               { CR1.CEN = false;    return *this; }
   TIM&     clear_counter()                 { CNT = 0;            return *this; }
   TIM&     set_counter (uint16_t v)        { CNT = v;            return *this; }
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
#if defined(STM32F0)
   TIM&     main_output_enable()            { BDTR.MOE = true;    return *this; }
#endif
   
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
   template<Channel>            static constexpr EnableMask enable_mask();

   template <Periph tim, class Pin_> static void pin_static_assert();
};






















#if not defined(USE_MOCK_TIM)
SFINAE(TIM1 , TIM) make_reference() { return *reinterpret_cast<TIM*>(TIM1_BASE);  }
SFINAE(TIM3 , TIM) make_reference() { return *reinterpret_cast<TIM*>(TIM3_BASE);  }
SFINAE(TIM14, TIM) make_reference() { return *reinterpret_cast<TIM*>(TIM14_BASE); }
#if defined(STM32F0)
SFINAE(TIM16, TIM) make_reference() { return *reinterpret_cast<TIM*>(TIM16_BASE); }
SFINAE(TIM17, TIM) make_reference() { return *reinterpret_cast<TIM*>(TIM17_BASE); }
#elif defined(STM32F4)
SFINAE(TIM2 , TIM) make_reference() { return *reinterpret_cast<TIM*>(TIM2_BASE);  }
SFINAE(TIM4 , TIM) make_reference() { return *reinterpret_cast<TIM*>(TIM4_BASE);  }
SFINAE(TIM8 , TIM) make_reference() { return *reinterpret_cast<TIM*>(TIM8_BASE);  }
#endif
#endif

template<Periph p, class Pin_> constexpr PinMode TIM::pin_mode()
{
#if defined(STM32F0)
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

#elif defined(STM32F4)
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

   } else if constexpr (p == Periph::TIM4) {
      if      constexpr (std::is_same_v<Pin_,PD12>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PD13>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PD14>) return PinMode::Alternate_2;
      else if constexpr (std::is_same_v<Pin_,PD15>) return PinMode::Alternate_2;
   
   } else if constexpr (p == Periph::TIM8) {
      if      constexpr (std::is_same_v<Pin_,PC6>) return PinMode::Alternate_3;
      else if constexpr (std::is_same_v<Pin_,PC7>) return PinMode::Alternate_3;
      else if constexpr (std::is_same_v<Pin_,PC8>) return PinMode::Alternate_3;
      else if constexpr (std::is_same_v<Pin_,PC9>) return PinMode::Alternate_3;
   
   } else {
      return PinMode::Input;
   }
#endif
}

template<TIM::Channel c> constexpr TIM::EnableMask TIM::enable_mask()
{
   if      constexpr (c == Channel::_1) return TIM::EnableMask::ch1;
   else if constexpr (c == Channel::_2) return TIM::EnableMask::ch2;
   else if constexpr (c == Channel::_3) return TIM::EnableMask::ch3;
   else if constexpr (c == Channel::_4) return TIM::EnableMask::ch4;
}

// template<TIM::Channel c> constexpr TIM::InterruptMask TIM::interrupt_mask()
// {
//    if      constexpr (c == Channel::_1) return TIM::InterruptMask::ch1;
//    else if constexpr (c == Channel::_2) return TIM::InterruptMask::ch2;
//    else if constexpr (c == Channel::_3) return TIM::InterruptMask::ch3;
//    else if constexpr (c == Channel::_4) return TIM::InterruptMask::ch4;
// }

template<Periph p, class Pin_> constexpr TIM::Channel TIM::channel()
{
#if defined(STM32F0)
   if      constexpr (p == Periph::TIM1) {
      if      constexpr (std::is_same_v<Pin_,PA8 >) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PA9 >) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PA10>) return Channel::_3;
      else if constexpr (std::is_same_v<Pin_,PA11>) return Channel::_4;
      else if constexpr (std::is_same_v<Pin_,PB13>) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PB14>) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PB15>) return Channel::_3;
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

#elif defined(STM32F4)
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

   } else if constexpr (p == Periph::TIM4) {
      if      constexpr (std::is_same_v<Pin_,PD12>) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PD13>) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PD14>) return Channel::_3;
      else if constexpr (std::is_same_v<Pin_,PD15>) return Channel::_4;
   
   } else if constexpr (p == Periph::TIM8) {
      if      constexpr (std::is_same_v<Pin_,PC6>) return Channel::_1;
      else if constexpr (std::is_same_v<Pin_,PC7>) return Channel::_2;
      else if constexpr (std::is_same_v<Pin_,PC8>) return Channel::_3;
      else if constexpr (std::is_same_v<Pin_,PC9>) return Channel::_4;
   } else {
      return Channel::error;
   }
#endif
}

template<TIM::Channel c> TIM& TIM::set (Polarity v)
{
   if constexpr (c == Channel::_1) {
      if (v == Polarity::rising) {
         CCER.CC1P  = false;
         CCER.CC1NP = false;
      } else if (v == Polarity::falling) {
         CCER.CC1P  = true;
         CCER.CC1NP = false;
      } else if (v == Polarity::both) {
         CCER.CC1P  = true;
         CCER.CC1NP = true;
      }
      return *this;
   } else if constexpr (c == Channel::_2) {
      if (v == Polarity::rising) {
         CCER.CC2P  = false;
         CCER.CC2NP = false;
      } else if (v == Polarity::falling) {
         CCER.CC2P  = true;
         CCER.CC2NP = false;
      } else if (v == Polarity::both) {
         CCER.CC2P  = true;
         CCER.CC2NP = true;
      }
      return *this;
   } else if constexpr (c == Channel::_3) {
      if (v == Polarity::rising) {
         CCER.CC3P  = false;
         CCER.CC3NP = false;
      } else if (v == Polarity::falling) {
         CCER.CC3P  = true;
         CCER.CC3NP = false;
      } else if (v == Polarity::both) {
         CCER.CC3P  = true;
         CCER.CC3NP = true;
      }
      return *this;
   } else if constexpr (c == Channel::_4) {
      if (v == Polarity::rising) {
         CCER.CC4P  = false;
         CCER.CC4NP = false;
      } else if (v == Polarity::falling) {
         CCER.CC4P  = true;
         CCER.CC4NP = false;
      } else if (v == Polarity::both) {
         CCER.CC4P  = true;
         CCER.CC4NP = true;
      }
      return *this;
   }
}

template<TIM::Channel c> TIM& TIM::set (CompareMode v)
{
   if      constexpr (c == Channel::_1) CCMR.output.OC1M = v;
   else if constexpr (c == Channel::_2) CCMR.output.OC2M = v;
   else if constexpr (c == Channel::_3) CCMR.output.OC3M = v;
   else if constexpr (c == Channel::_4) CCMR.output.OC4M = v;
   return *this;
}

template<TIM::Channel c> TIM& TIM::set (SelectionCompareMode v)
{
   if      constexpr (c == Channel::_1) CCMR.output.CC1S = v;
   else if constexpr (c == Channel::_2) CCMR.output.CC2S = v;
   else if constexpr (c == Channel::_3) CCMR.output.CC3S = v;
   else if constexpr (c == Channel::_4) CCMR.output.CC4S = v;
   return *this;
}

template<TIM::Channel c> TIM& TIM::preload_enable()
{
   if      constexpr (c == Channel::_1) CCMR.output.OC1PE = true;
   else if constexpr (c == Channel::_2) CCMR.output.OC2PE = true;
   else if constexpr (c == Channel::_3) CCMR.output.OC3PE = true;
   else if constexpr (c == Channel::_4) CCMR.output.OC4PE = true;
   return *this;
}

template<TIM::Channel c> TIM& TIM::preload_disable()
{
   if      constexpr (c == Channel::_1) CCMR.output.OC1PE = false;
   else if constexpr (c == Channel::_2) CCMR.output.OC2PE = false;
   else if constexpr (c == Channel::_3) CCMR.output.OC3PE = false;
   else if constexpr (c == Channel::_4) CCMR.output.OC4PE = false;
   return *this; 
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

template<Periph tim, class Pin_> void TIM::pin_static_assert()
{
   // static_assert (
   //    WRAP(meta::position_v<Pin
   //       ,PA9,PA10,PB6,PB7,PA2,PA3,PD5,PD6,PB10,PB11,PC10,PC11,PD8,PD9
   // #if defined(STM32F4)
   //       ,PA0,PA1,PC10,PC11,PC12,PD2,PC6,PC7,PG14,PG9
   // #endif
   //    >) != -1,
   if constexpr (tim == Periph::TIM1) {
      static_assert (
         std::is_same_v<Pin_, PA8> or std::is_same_v<Pin_, PA9> or
         std::is_same_v<Pin_, PA10> or std::is_same_v<Pin_, PA11> or
         std::is_same_v<Pin_, PB13> or std::is_same_v<Pin_, PB14> or
         std::is_same_v<Pin_, PB15>, 
         "\033[7;33mTIM1 of STM32(F0/F4) can only work with these pins: pin(№channel) \
PA*(1), PA9(2), PB0(3), PA10(4), PA11(1), PB13(1), \
PB14(2), PB15(3)\033[0m"
      );
   } else if constexpr (tim == Periph::TIM3) {
      static_assert (
         std::is_same_v<Pin_, PA6> or std::is_same_v<Pin_, PA7> or
         std::is_same_v<Pin_, PB0> or std::is_same_v<Pin_, PB1> or
         std::is_same_v<Pin_, PB4> or std::is_same_v<Pin_, PB5> or
         std::is_same_v<Pin_, PC6> or std::is_same_v<Pin_, PC7> or
         std::is_same_v<Pin_, PC8> or std::is_same_v<Pin_, PC9> or
         std::is_same_v<Pin_, PD2>, 
         "\033[7;33mTIM3 of STM32(F0/F4) can only work with these pins: pin(№channel) \
PA6(1), PA7(2), PB0(3), PB1(4), PB4(1), PB5(2), \
PC6(1), PC7(2), PC8(3), PC9(4), PD2(ETR)\033[0m"
      );
   } 
#if defined (STM32F4)
   if constexpr (tim == Periph::TIM2) {
      static_assert (
         std::is_same_v<Pin_, PA0 > or std::is_same_v<Pin_, PA1 > or
         std::is_same_v<Pin_, PA2 > or std::is_same_v<Pin_, PA3 > or
         std::is_same_v<Pin_, PA5 > or std::is_same_v<Pin_, PA15> or
         std::is_same_v<Pin_, PB3 > or std::is_same_v<Pin_, PB10> or
         std::is_same_v<Pin_, PB11>, 
         "\033[7;33mTIM2 of STM32F4 can only work with these pins: pin(№channel) \
PA0(1_ETR),  PA1(2), PA2(3),  PA3(4), PA5(1_ETR),\
PA15(1,ETR), PB3(2), PB10(3), PB11(4)\033[0m"
      );
   } else if constexpr (tim == Periph::TIM8) {
      static_assert (
         std::is_same_v<Pin_, PC6 > or std::is_same_v<Pin_, PC7 > or
         std::is_same_v<Pin_, PC8 > or std::is_same_v<Pin_, PC9 > or 
         "\033[7;33mTIM2 of STM32F4 can only work with these pins: pin(№channel) \
PC6(1),  PC7(2), PC8(3),  PC9(4)\033[0m"
      );
   } 
#endif
}


} // namespace mcu {