#pragma once

#include "periph.h"

#if defined(STM32F0) or defined(STM32F4)
   #include "f0_f4_tim.h"
#elif defined(STM32F1)
   #include "f1_tim.h"
#endif