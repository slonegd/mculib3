#pragma once

#include "periph.h"
#include "pins.h"

#if   defined(STM32F0)
   #include "gpio_f0_f4_f7.h"
#elif defined(STM32F1)
   #include "gpio_f1.h"
#elif defined(STM32F4)
   #include "gpio_f0_f4_f7.h"
#elif defined(STM32F7)
   #include "gpio_f0_f4_f7.h"
#endif