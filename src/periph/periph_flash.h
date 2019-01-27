#pragma once

#include "periph.h"
#define PERIPH_FLASH_H_

#if   defined(STM32F0)
   #include "flash_f0.h"
#elif defined(STM32F1)
   #include "flash_f1.h"
#elif defined(STM32F4)
   #include "flash_f4_f7.h"
#elif defined(STM32F7)
   #include "flash_f4_f7.h"
#endif
