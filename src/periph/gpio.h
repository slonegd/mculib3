#pragma once

#include "periph.h"

#if defined(STM32F0)
   #include "f0_f4_gpio.h"
#elif defined(STM32F1)
   #include "f1_gpio.h"
#elif defined(STM32F4)
   #include "f0_f4_gpio.h"
#endif