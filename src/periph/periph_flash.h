#pragma once

#include "periph.h"

#if defined(STM32F0)
   // #include "f0_flash.h"
#elif defined(STM32F1)
   #include "f1_flash.h"
#elif defined(STM32F4)
   #include "f4_flash.h"
#endif