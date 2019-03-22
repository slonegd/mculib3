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
   using Resolution  = ADC_bits::CR1::Resolution;
   using Sample_time = ADC_bits::SMPR::Sample_time;
   
};

} //namespace mcu