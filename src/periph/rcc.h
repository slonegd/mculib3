#pragma once

#include "periph.h"

#if defined(STM32F0)
   #include "rcc_f0.h"
#elif defined(STM32F4)
   #include "rcc_f4.h"
#endif