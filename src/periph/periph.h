#pragma once

#if defined(STM32F030x6)
   #define STM32F0
#elif defined(STM32F405xx)
   #define STM32F4
#else
   #define STM32F0
#endif

#if defined(STM32F0)
   #include "stm32f0xx.h"
#elif defined(STM32F4)
   #include "stm32f4xx.h"
#endif

#include <type_traits>
#include <cstddef>
#include <cstdint>

#undef RCC
#undef GPIOA
#undef GPIOB
#undef GPIOC
#undef GPIOD
#undef GPIOE
#undef GPIOF
#undef GPIOG
#undef GPIOH
#undef GPIOI

namespace mcu {

enum class Periph {
    RCC,
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOF,
#if defined(STM32F4)
    GPIOE, GPIOG, GPIOH, GPIOI
#endif
};

}






#if defined(TEST)
   #include <thread>
#endif

#if not defined(__IO)
   #define __IO volatile
#endif

#if defined(TEST)
    #define IF_TEST_WAIT_MS(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#else
    #define SLIF_TEST_WAIT_MSEEP(ms)
#endif
