#pragma once

#include "bits_adc_f4.h"
#include "pins.h"


namespace mcu {

class ADC
{
   volatile ADC_bits::SR   SR;    // status register,                         offset: 0x00
   volatile ADC_bits::CR1  CR1;   // control register 1,                      offset: 0x04
   volatile ADC_bits::CR2  CR2;   // control register 2,                      offset: 0x08
   volatile ADC_bits::SMPR SMPR;  // sample time register 1, 2                offset: 0x0C
   volatile uint32_t       JOFR1; // injected channel data offset register 1, offset: 0x14
   volatile uint32_t       JOFR2; // injected channel data offset register 2, offset: 0x18
   volatile uint32_t       JOFR3; // injected channel data offset register 3, offset: 0x1C
   volatile uint32_t       JOFR4; // injected channel data offset register 4, offset: 0x20
   volatile uint32_t       HTR;   // watchdog higher threshold register,      offset: 0x24
   volatile uint32_t       LTR;   // watchdog lower threshold register,       offset: 0x28
   volatile ADC_bits::SQR  SQR;   // regular sequence register 1, 2, 3        offset: 0x2C
   volatile uint32_t       JSQR;  // injected sequence register,              offset: 0x38
   volatile uint32_t       JDR1;  // injected data register 1,                offset: 0x3C
   volatile uint32_t       JDR2;  // injected data register 2,                offset: 0x40
   volatile uint32_t       JDR3;  // injected data register 3,                offset: 0x44
   volatile uint32_t       JDR4;  // injected data register 4,                offset: 0x48
   volatile uint32_t       DR;    // regular data register,                   offset: 0x4C

public:
   using CMSIS_type  = ADC_TypeDef;
   // using Clock       = ADCC::CCR::Clock;
   using Resolution  = ADC_bits::CR1::Resolution;
   using Sample_time = ADC_bits::SMPR::Sample_time;
   // using Channel     = DMA::Channel;
   using Sample_time = ADC_bits::SMPR::Sample_time;

   ADC& enable()              {CR2.ADON    = true;  return *this;}
   ADC& disable()             {CR2.ADON    = false; return *this;}
   ADC& start()               {CR2.SWSTART = true;  return *this;}
   ADC& set (Resolution v)    {CR1.RES     = v;     return *this;}
   ADC& set_scan_mode()       {CR1.SCAN    = true;  return *this;}
   ADC& set_continuous_mode() {CR2.CONT    = true;  return *this;}
   // ADC& set (Clock v)         {                     return *this;}
   ADC& DMA_enable()          {CR2.DMA     = true;  return *this;}
   ADC& set_circular_DMA()    {CR2.DDS     = true;  return *this;}

   template <uint8_t channel> ADC& set (Sample_time);
   template <size_t order, uint8_t channel> static constexpr ADC& set_regular_sequence_order();
   template <size_t length> static constexpr ADC& set_regular_sequence_length();
   // template <Periph adc> static constexpr Channel DMA_channel();
};

#if not defined(USE_MOCK_ADC)
SFINAE(ADC1,ADC) make_reference() {return *reinterpret_cast<ADC*>(ADC1_BASE);}
SFINAE(ADC2,ADC) make_reference() {return *reinterpret_cast<ADC*>(ADC2_BASE);}
SFINAE(ADC3,ADC) make_reference() {return *reinterpret_cast<ADC*>(ADC3_BASE);}
#endif



template <uint8_t channel> 
ADC& ADC::set (ADC::Sample_time v)
{
   static_assert (channel >= 0 and channel <= 18);
   if      constexpr (channel == 0)  SMPR.SMP0  = v;
   else if constexpr (channel == 1)  SMPR.SMP1  = v;
   else if constexpr (channel == 2)  SMPR.SMP2  = v;
   else if constexpr (channel == 3)  SMPR.SMP3  = v;
   else if constexpr (channel == 4)  SMPR.SMP4  = v;
   else if constexpr (channel == 5)  SMPR.SMP5  = v;
   else if constexpr (channel == 6)  SMPR.SMP6  = v;
   else if constexpr (channel == 7)  SMPR.SMP7  = v;
   else if constexpr (channel == 8)  SMPR.SMP8  = v;
   else if constexpr (channel == 9)  SMPR.SMP9  = v;
   else if constexpr (channel == 10) SMPR.SMP10 = v;
   else if constexpr (channel == 11) SMPR.SMP11 = v;
   else if constexpr (channel == 12) SMPR.SMP12 = v;
   else if constexpr (channel == 13) SMPR.SMP13 = v;
   else if constexpr (channel == 14) SMPR.SMP14 = v;
   else if constexpr (channel == 15) SMPR.SMP15 = v;
   else if constexpr (channel == 16) SMPR.SMP16 = v;
   else if constexpr (channel == 17) SMPR.SMP17 = v;
   else if constexpr (channel == 18) SMPR.SMP18 = v;
}

template <size_t order, uint8_t channel> // n - номер по порядку (1 - 16)
constexpr ADC& ADC::set_regular_sequence_order()
{
   static_assert (order >= 1 and order <= 16);
   static_assert (channel >= 0 and channel <= 18);
   if      constexpr (order == 1)  SQR.SQ1  = channel;
   else if constexpr (order == 2)  SQR.SQ2  = channel;
   else if constexpr (order == 3)  SQR.SQ3  = channel;
   else if constexpr (order == 4)  SQR.SQ4  = channel;
   else if constexpr (order == 5)  SQR.SQ5  = channel;
   else if constexpr (order == 6)  SQR.SQ6  = channel;
   else if constexpr (order == 7)  SQR.SQ7  = channel;
   else if constexpr (order == 8)  SQR.SQ8  = channel;
   else if constexpr (order == 9)  SQR.SQ9  = channel;
   else if constexpr (order == 10) SQR.SQ10 = channel;
   else if constexpr (order == 11) SQR.SQ11 = channel;
   else if constexpr (order == 12) SQR.SQ12 = channel;
   else if constexpr (order == 13) SQR.SQ13 = channel;
   else if constexpr (order == 14) SQR.SQ14 = channel;
   else if constexpr (order == 15) SQR.SQ15 = channel;
   else if constexpr (order == 16) SQR.SQ16 = channel;
}

template <size_t length>
constexpr ADC& ADC::set_regular_sequence_length()
{
   static_assert (length >= 1 and length <= 16);
   SQR.L = length - 1;
}

// template<Periph adc> 
// constexpr ADC::Channel ADC::DMA_channel()
// {
//    if      constexpr (adc == Periph::ADC1) return Channel::_0;
//    else if constexpr (adc == Periph::ADC2) return Channel::_1;
//    else if constexpr (adc == Periph::ADC3) return Channel::_2;
// } 

} //namespace mcu