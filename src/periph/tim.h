#pragma once

#include "periph.h"

#if defined(STM32F0)
   #include "f0_tim.h"
#elif defined(STM32F1)
   #include "f1_tim.h"
#elif defined(STM32F4)
   // #include "f4_tim.h"
#endif