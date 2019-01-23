#pragma once

#include "periph.h"

#if defined(STM32F0)
   #include "f0_usart.h"
#elif defined(STM32F1)
   #include "f1_f4_usart.h"
#elif defined(STM32F4)
   #include "f1_f4_usart.h"
#endif