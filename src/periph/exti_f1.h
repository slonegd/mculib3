#pragma once

#include "bits_exti_f1.h"

namespace mcu {

class EXTI
{
    volatile EXTI_bits::R     IMR;     // Interrupt mask register            offset: 0x00
    volatile EXTI_bits::R     EMR;     // Event mask register                offset: 0x04
    volatile EXTI_bits::R     RTSR;    // Rising trigger selection register  offset: 0x08
    volatile EXTI_bits::R     FTSR;    // Falling trigger selection register offset: 0x0C
    volatile EXTI_bits::R     SWIER;   // Software interrupt event register  offset: 0x10
    volatile EXTI_bits::R     PR;      // Pending register                   offset: 0x14
public:
    using CMSIS_type  = EXTI_TypeDef;

};

#if not defined(USE_MOCK_EXTI)
SFINAE(EXTI,EXTI) make_reference() {return *reinterpret_cast<EXTI*>(EXTI_BASE);}
#endif







} // namespace mcu {