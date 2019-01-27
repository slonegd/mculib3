#pragma once

#include "periph.h"

#if defined(STM32F0)
   #include "dma_f0.h"
   #include "dma_stream_f0_f1.h"
#elif defined(STM32F1)
   #include "dma_f1.h"
   #include "dma_stream_f0_f1.h"
#elif defined(STM32F4) or defined(STM32F7)
   #include "dma_f4_f7.h"
   #include "dma_stream_f4_f7.h"
#endif