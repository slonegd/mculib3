#pragma once

#include "periph.h"

#if   defined(STM32F0)
   #error init_clock for STM32F0 not realized
#elif defined(STM32F1)
   #error init_clock for STM32F1 not realized
#elif defined(STM32F4)
   #error init_clock for STM32F4 not realized
#elif defined(STM32F7)
   #include "init_clock_f7.h"
#endif