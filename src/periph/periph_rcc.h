#pragma once

#include "periph.h"

#if   defined(STM32F0)
   #include "f0_rcc.h"
#elif defined(STM32F1)
   #include "f1_rcc.h"
#elif defined(STM32F4)
   #include "f4_rcc.h"
#elif defined(STM32F7)
   #include "rcc_f7.h"
#endif