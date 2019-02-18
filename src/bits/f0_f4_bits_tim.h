#pragma once

#include "periph.h"

#if defined(STM32F0)
   #include "f0_bits_tim.h"
#elif defined(STM32F4)
   #include "f4_bits_tim.h"
#endif