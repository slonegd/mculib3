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
   using DMA_mode    = ADC_bits::CR2::DMA_mode;
   using Resolution  = ADC_bits::CR1::Resolution;
   using Sample_time = ADC_bits::SMPR::Sample_time;
   // using Channel     = DMA::Channel;

   ADC& enable()           {CR2.ADON    = true;  return *this;}
   bool is_enable()        {return CR2.ADON;}
   ADC& disable()          {CR2.ADON    = false; return *this;}
   ADC& start()            {CR2.SWSTART = true;  return *this;}
   ADC& dma_enable()       {CR2.DMA     = true;  return *this;}
   ADC& set (Resolution v) {CR1.RES     = v;     return *this;}
   ADC& set_scan_mode()    {CR1.SCAN    = true;  return *this;}
   ADC& set_continuous()   {CR2.CONT    = true;  return *this;}
   ADC& set (DMA_mode v)   {CR2.DDS     = v;     return *this;}
   // ADC& set (Clock v)         {                     return *this;}
   volatile const uint32_t& data() {return DR;}

   ADC& set (uint8_t channel, Sample_time);
   ADC& set_regular_sequence_order (size_t order, uint8_t channel);
   ADC& set_regular_sequence_length(size_t length);
   template <class Pin> int set_channel(Periph);
   template<Periph> static constexpr Periph default_dma();
   // template <Periph adc> static constexpr Channel DMA_channel();
};

#if not defined(USE_MOCK_ADC)
SFINAE(ADC1,ADC) make_reference() {return *reinterpret_cast<ADC*>(ADC1_BASE);}
SFINAE(ADC2,ADC) make_reference() {return *reinterpret_cast<ADC*>(ADC2_BASE);}
SFINAE(ADC3,ADC) make_reference() {return *reinterpret_cast<ADC*>(ADC3_BASE);}
#endif


ADC& ADC::set (uint8_t channel, ADC::Sample_time v)
{
   if      (channel == 0)  SMPR.SMP0  = v;
   else if (channel == 1)  SMPR.SMP1  = v;
   else if (channel == 2)  SMPR.SMP2  = v;
   else if (channel == 3)  SMPR.SMP3  = v;
   else if (channel == 4)  SMPR.SMP4  = v;
   else if (channel == 5)  SMPR.SMP5  = v;
   else if (channel == 6)  SMPR.SMP6  = v;
   else if (channel == 7)  SMPR.SMP7  = v;
   else if (channel == 8)  SMPR.SMP8  = v;
   else if (channel == 9)  SMPR.SMP9  = v;
   else if (channel == 10) SMPR.SMP10 = v;
   else if (channel == 11) SMPR.SMP11 = v;
   else if (channel == 12) SMPR.SMP12 = v;
   else if (channel == 13) SMPR.SMP13 = v;
   else if (channel == 14) SMPR.SMP14 = v;
   else if (channel == 15) SMPR.SMP15 = v;
   else if (channel == 16) SMPR.SMP16 = v;
   else if (channel == 17) SMPR.SMP17 = v;
   else if (channel == 18) SMPR.SMP18 = v;

   return *this;
}

ADC& ADC::set_regular_sequence_order(size_t order, uint8_t channel)
{
   // static_assert (order >= 1 and order <= 16);
   // static_assert (channel >= 0 and channel <= 18);
   if      (order == 1)  SQR.SQ1  = channel;
   else if (order == 2)  SQR.SQ2  = channel;
   else if (order == 3)  SQR.SQ3  = channel;
   else if (order == 4)  SQR.SQ4  = channel;
   else if (order == 5)  SQR.SQ5  = channel;
   else if (order == 6)  SQR.SQ6  = channel;
   else if (order == 7)  SQR.SQ7  = channel;
   else if (order == 8)  SQR.SQ8  = channel;
   else if (order == 9)  SQR.SQ9  = channel;
   else if (order == 10) SQR.SQ10 = channel;
   else if (order == 11) SQR.SQ11 = channel;
   else if (order == 12) SQR.SQ12 = channel;
   else if (order == 13) SQR.SQ13 = channel;
   else if (order == 14) SQR.SQ14 = channel;
   else if (order == 15) SQR.SQ15 = channel;
   else if (order == 16) SQR.SQ16 = channel;

   return *this;
}

ADC& ADC::set_regular_sequence_length(size_t length)
{
   // static_assert (length >= 1 and length <= 16);
   SQR.L = length - 1;

   return *this;
}

template <class Pin> int ADC::set_channel(Periph p)
{
    if (p == Periph::ADC1 or p == Periph::ADC2) {
        if      constexpr (std::is_same_v<Pin,PA0>) return 0; 
        else if constexpr (std::is_same_v<Pin,PA1>) return 1; 
        else if constexpr (std::is_same_v<Pin,PA2>) return 2; 
        else if constexpr (std::is_same_v<Pin,PA3>) return 3; 
        else if constexpr (std::is_same_v<Pin,PA4>) return 4; 
        else if constexpr (std::is_same_v<Pin,PA5>) return 5; 
        else if constexpr (std::is_same_v<Pin,PA6>) return 6; 
        else if constexpr (std::is_same_v<Pin,PA7>) return 7; 
        else if constexpr (std::is_same_v<Pin,PB0>) return 8; 
        else if constexpr (std::is_same_v<Pin,PB1>) return 9; 
        else if constexpr (std::is_same_v<Pin,PC0>) return 10;
        else if constexpr (std::is_same_v<Pin,PC1>) return 11;
        else if constexpr (std::is_same_v<Pin,PC2>) return 12;
        else if constexpr (std::is_same_v<Pin,PC3>) return 13;
        else if constexpr (std::is_same_v<Pin,PC4>) return 14;
        else if constexpr (std::is_same_v<Pin,PC5>) return 15;
        else static_assert(always_false_v<Pin>, "Вывод не поддерживает работы с АЦП1/2");
    } 
    // else if (p == Periph::ADC3) {
    //     if      constexpr (std::is_same_v<Pin,PA0>) return 0;
    //     else if constexpr (std::is_same_v<Pin,PA1>) return 1;
    //     else if constexpr (std::is_same_v<Pin,PA2>) return 2;
    //     else if constexpr (std::is_same_v<Pin,PA3>) return 3;
    //     else if constexpr (std::is_same_v<Pin,PF6>) return 4; 
    //     else if constexpr (std::is_same_v<Pin,PF7>) return 5; 
    //     else if constexpr (std::is_same_v<Pin,PF8>) return 6; 
    //     else if constexpr (std::is_same_v<Pin,PF9>) return 7; 
    //     else if constexpr (std::is_same_v<Pin,PF10>) return 8; 
    //     else if constexpr (std::is_same_v<Pin,PF3>) return 9; 
    //     else if constexpr (std::is_same_v<Pin,PC0>) return 10;
    //     else if constexpr (std::is_same_v<Pin,PC1>) return 11;
    //     else if constexpr (std::is_same_v<Pin,PC2>) return 12;
    //     else if constexpr (std::is_same_v<Pin,PC3>) return 13;
    //     else if constexpr (std::is_same_v<Pin,PF4>) return 14;
    //     else if constexpr (std::is_same_v<Pin,PF5>) return 15;
    //     else static_assert(always_false_v<Pin>, "Вывод не поддерживает работы c АЦП3");
    // }
    return 0;
}

template<Periph adc> constexpr Periph ADC::default_dma()
{
   if      constexpr (adc == Periph::ADC1) return Periph::DMA2_stream0;
   else if constexpr (adc == Periph::ADC2) return Periph::DMA2_stream2;
   else if constexpr (adc == Periph::ADC3) return Periph::DMA2_stream0;
}

// template<Periph adc> 
// constexpr ADC::Channel ADC::DMA_channel()
// {
//    if      constexpr (adc == Periph::ADC1) return Channel::_0;
//    else if constexpr (adc == Periph::ADC2) return Channel::_1;
//    else if constexpr (adc == Periph::ADC3) return Channel::_2;
// } 

} //namespace mcu