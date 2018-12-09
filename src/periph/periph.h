#pragma once

#if   defined(STM32F030x6)
   #define STM32F0
#elif defined(STM32F103xB)
   #define STM32F1
#elif defined(STM32F405xx)
   #define STM32F4
#elif defined(STM32F746xx)
   #define STM32F7
#else
   #define STM32F0
#endif

#if   defined(STM32F0)
   #include "stm32f0xx.h"
#elif defined(STM32F1)
   #include "stm32f1xx.h"
#elif defined(STM32F4)
   #include "stm32f4xx.h"
#elif defined(STM32F7)
   #include "stm32f7xx.h"
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
#undef GPIOJ
#undef GPIOK
#undef AFIO
#undef USART1
#undef USART2
#undef USART3
#undef UART4
#undef UART5
#undef USART6
#undef UART7
#undef UART8
#undef SysTick
#undef TIM1
#undef TIM3
#undef TIM14
#undef TIM16
#undef TIM17
#undef FLASH
#undef DMA1


namespace mcu {

enum class Periph {
    RCC,
#if   defined(STM32F0)
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOF,
#elif defined(STM32F1)
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, AFIO,
#elif defined(STM32F4) || defined(STM32F7)
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOF, GPIOE, GPIOG, GPIOH, GPIOI,
#endif
#if   defined(STM32F0)
    USART1,
#elif defined(STM32F1)
    USART1, USART2, USART3,
#elif defined(STM32F4)
    USART1, USART2, USART3, USART4, USART5, USART6,
#elif defined(STM32F7)
    USART1, USART2, USART3, USART4, USART5, USART6, USART7, USART8, 
#endif
#if defined(STM32F1)
    DMA1, DMA1_stream1, DMA1_stream2, DMA1_stream3, DMA1_stream4, DMA1_stream5, DMA1_stream6, DMA1_stream7,
#endif
    SysTick,
    TIM1, TIM3, TIM14, TIM16, TIM17,
    FLASH,

#if defined(TEST)
    TEST_RCC, TEST_AFIO, TEST_DMA
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
    #define IF_TEST_WAIT_MS(ms)
#endif
