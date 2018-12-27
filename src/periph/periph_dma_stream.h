#pragma once

#include "periph.h"

#if defined(STM32F0) or defined(STM32F1)
   #include "f0_f1_periph_dma_stream.h"
// #elif defined(STM32F4) or defined(STM32F7)
//    #include "f4_f7_dma.h"
#endif