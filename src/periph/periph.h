#pragma once

#if defined(STM32F030x6)
   #define STM32F0
#elif defined(STM32F103xB)
   #define STM32F1
#elif defined(STM32F405xx)
   #define STM32F4
#else
   #define STM32F0
#endif

#if defined(STM32F0)
   #include "stm32f0xx.h"
#elif defined(STM32F1)
   #include "stm32f1xx.h"
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
#undef USART1
#undef USART2
#undef USART3
#undef UART4
#undef UART5
#undef USART6
#undef SysTick
#undef TIM1
#undef TIM3
#undef TIM14
#undef TIM16
#undef TIM17

namespace mcu {

enum class Periph {
    RCC,
#if defined(STM32F0)
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOF,
#endif
#if defined(STM32F1)
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE,
#endif
#if defined(STM32F4)
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOF, GPIOE, GPIOG, GPIOH, GPIOI,
#endif
#if defined(STM32F0)
    USART1,
#endif
#if defined(STM32F1)
    USART1, USART2, USART3,
#endif
#if defined(STM32F4)
    USART1, USART2, USART3, USART4, USART5, USART6,
#endif
    SysTick,
    TIM1, TIM3, TIM14, TIM16, TIM17,

#if defined(TEST)
    TEST_RCC
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
    #define IF_TEST_WAIT_MSEEP(ms)
#endif
