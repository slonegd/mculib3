#pragma once

#include "bits_adc_f0.h"

namespace mcu {

class ADC
{

};

#if not defined(USE_MOCK_ADC)
SFINAE(ADC1,ADC) make_reference() {return *reinterpret_cast<ADC*>(ADC1_BASE);}
#endif




} // namespace mcu {