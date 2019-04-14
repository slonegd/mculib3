#pragma once

#include "periph.h"

#if   defined(STM32F0)
   #include "init_clock_f0.h"
#elif defined(STM32F1)
   #include "init_clock_f1.h"
#elif defined(STM32F4)
   #include "init_clock_f4.h"
#elif defined(STM32F7)
   #include "init_clock_f7.h"
#endif