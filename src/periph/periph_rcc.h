#pragma once

#include "periph.h"

#if   defined(STM32F0)
   #include "rcc_f0.h"
#elif defined(STM32F1)
   #include "rcc_f1.h"
#elif defined(STM32F4)
   #include "rcc_f4.h"
#elif defined(STM32F7)
   #include "rcc_f7.h"
#endif