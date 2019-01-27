#pragma once

#include "periph.h"
#define PERIPH_USART_H_

#if defined(STM32F0)
   #include "usart_f0.h"
#elif defined(STM32F1)
   #include "usart_f1_f4.h"
#elif defined(STM32F4)
   #include "usart_f1_f4.h"
#endif
