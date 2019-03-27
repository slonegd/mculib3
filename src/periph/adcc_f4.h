#pragma once

#include "bits_adcc_f4.h"

namespace mcu {

class ADCC
{
   volatile ADCC_bits::CSR CSR;
   volatile ADCC_bits::CCR CCR;
   volatile uint32_t       CDR;

public:
   using CMSIS_type  = ADC_Common_TypeDef;
   using Clock       = ADCC_bits::CCR::Clock;
   using Delay       = ADCC_bits::CCR::Delay;

   ADCC& set (Clock v) {CCR.ADCPRE = v; return *this;}
   ADCC& set (Delay v) {CCR.DELAY  = v; return *this;}

};

#if not defined(USE_MOCK_ADCC)
// template <Periph p> std::enable_if_t<p == Periph::ADCC, ADCC&> make_reference() {return *reinterpret_cast<ADCC*>(ADC1_BASE+0x300);}
SFINAE(ADCC, ADCC) make_reference() {return *reinterpret_cast<ADCC*>(ADC1_BASE+0x300);}
#endif

} //namespace mcu