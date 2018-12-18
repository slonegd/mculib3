#pragma once

#include "periph.h"
#include "pins.h"

#if   defined(STM32F0)
   #include "f0_f4_f7_gpio.h"
#elif defined(STM32F1)
   #include "f1_gpio.h"
#elif defined(STM32F4)
   #include "f0_f4_f7_gpio.h"
#elif defined(STM32F7)
   #include "f0_f4_f7_gpio.h"
#endif