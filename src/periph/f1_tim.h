#pragma once

#include <type_traits>
#include "periph.h"
#include "f1_bits_tim.h"
#include "rcc.h"
#include "pin.h"

namespace mcu {

class TIM {
   __IO TIM_bits::CR1 CR1;   //TIM control register 1,                     offset: 0x00
   __IO uint32_t      CR2;   //TIM control register 2,                     offset: 0x04
   __IO uint32_t      SMCR;  //TIM slave Mode Control register,            offset: 0x08
   __IO uint32_t      DIER;  //TIM DMA/interrupt enable register,          offset: 0x0C
   __IO TIM_bits::SR  SR;    //TIM status register,                        offset: 0x10
   __IO TIM_bits::EGR EGR;   //TIM event generation register,              offset: 0x14
   __IO uint32_t      CCMR1; //TIM capture/compare mode register 1,        offset: 0x18
   __IO uint32_t      CCMR2; //TIM capture/compare mode register 2,        offset: 0x1C
   __IO uint32_t      CCER;  //TIM capture/compare enable register,        offset: 0x20
   __IO uint32_t      CNT;   //TIM counter register,                       offset: 0x24
   __IO uint32_t      PSC;   //TIM prescaler register,                     offset: 0x28
   __IO uint32_t      ARR;   //TIM auto-reload register,                   offset: 0x2C
   __IO uint32_t      RCR;   //TIM repetition counter register,            offset: 0x30
   __IO uint32_t      CCR1;  //TIM capture/compare register 1,             offset: 0x34
   __IO uint32_t      CCR2;  //TIM capture/compare register 2,             offset: 0x38
   __IO uint32_t      CCR3;  //TIM capture/compare register 3,             offset: 0x3C
   __IO uint32_t      CCR4;  //TIM capture/compare register 4,             offset: 0x40
   __IO uint32_t      BDTR;  //TIM break and dead-time register,           offset: 0x44
   __IO uint32_t      DCR;   //TIM DMA control register,                   offset: 0x48
   __IO uint32_t      DMAR;  //TIM DMA address for full transfer register, offset: 0x4C
   __IO uint32_t      OR;    //TIM option register,                        offset: 0x50

public:

   using CMSIS_type           = TIM_TypeDef;
   using OnePulseMode         = TIM_bits::CR1::OnePulseMode;
   using Direction            = TIM_bits::CR1::Direction;

   template <Periph tim, Periph rcc = Periph::RCC>
   TIM& clock_enable  (){make_reference<rcc>().template clock_enable<tim>(); return *this;}
   TIM& counter_disable()           {CR1.CEN = false; return *this;}
   TIM& counter_enable()            {CR1.CEN = true;  return *this;}
   TIM& set (OnePulseMode v)        {CR1.OPM  = v;    return *this;}
   TIM& clear_counter()             {CNT = 0;         return *this;}
   TIM& set_counter (uint16_t v)    {CNT = v;         return *this;}
   TIM& update_generation()         {EGR.UG = true;   return *this;}
   TIM& set_prescaller (uint16_t v) {PSC = v;         return *this;}
   TIM& clear_interrupt_flag()      {SR.UIF = false;  return *this;}
   TIM& reload_value(uint16_t v)    {ARR = v;         return *this;}
   bool is_interrupt()              {return SR.UIF;}
   bool is_count()                  {return CR1.CEN;}

};

template<Periph p> std::enable_if_t<p==Periph::TIM4, TIM&> make_reference() {return *reinterpret_cast<TIM*>(TIM4_BASE);}

} // namespace mcu