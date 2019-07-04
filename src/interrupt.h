#pragma once

#include "periph.h"

#if defined(USE_MOCK_NVIC)
    inline static auto NVIC_EnableIRQ_t = mock::NVIC_EnableIRQ;
#else
    inline static auto NVIC_EnableIRQ_t = ::NVIC_EnableIRQ;
#endif

/// интерфейс для подписки на прерывания
struct Interrupting
{
    Interrupting* next {nullptr};
    virtual void interrupt() = 0;
};

class Interrupt 
{
    Interrupting* first{nullptr};
    const IRQn_Type irq_n;

public:
    Interrupt (IRQn_Type irq_n) : irq_n {irq_n} {}

    auto IRQn() const { return irq_n; }

    void enable() { NVIC_EnableIRQ_t(irq_n); }

    void subscribe(Interrupting* ps)
    {
        auto p = first;
        if (p) {
            while (p->next)
                p = p->next;
            p->next = ps;
        } else {  
            first = ps;
        } 
    }

    void clear_subscribe() { first = nullptr; }

    void interrupt()
    {
        auto p = first;
        while (p) {
            p->interrupt();
            p = p->next;
        }
    }
};

Interrupt interrupt_spi1 {SPI1_IRQn};
#if defined(STM32F4)
Interrupt interrupt_spi2 {SPI2_IRQn};
Interrupt interrupt_spi3 {SPI3_IRQn};
#endif

Interrupt interrupt_usart1 {USART1_IRQn};
#if defined(STM32F1)
Interrupt interrupt_usart2 {USART2_IRQn};
Interrupt interrupt_usart3 {USART3_IRQn};
#endif
#if defined(STM32F4)
Interrupt interrupt_usart2 {USART2_IRQn};
Interrupt interrupt_usart3 {USART3_IRQn};
#endif

Interrupt interrupt_tim1 {TIM1_BRK_UP_TRG_COM_IRQn};
Interrupt interrupt_tim3 {TIM3_IRQn};


#if defined(STM32F0)
Interrupt interrupt_DMA1_channel1 {DMA1_Channel1_IRQn};
Interrupt interrupt_DMA1_channel2 {DMA1_Channel2_3_IRQn};
Interrupt interrupt_DMA1_channel3 {DMA1_Channel2_3_IRQn};
Interrupt interrupt_DMA1_channel4 {DMA1_Channel4_5_IRQn};
Interrupt interrupt_DMA1_channel5 {DMA1_Channel4_5_IRQn};
#elif defined(STM32F1)
Interrupt interrupt_DMA1_channel1 {DMA1_Channel1_IRQn};
Interrupt interrupt_DMA1_channel2 {DMA1_Channel2_IRQn};
Interrupt interrupt_DMA1_channel3 {DMA1_Channel3_IRQn};
Interrupt interrupt_DMA1_channel4 {DMA1_Channel4_IRQn};
Interrupt interrupt_DMA1_channel5 {DMA1_Channel5_IRQn};
Interrupt interrupt_DMA1_channel6 {DMA1_Channel6_IRQn};
Interrupt interrupt_DMA1_channel7 {DMA1_Channel7_IRQn};
#elif defined(STM32F4)
Interrupt interrupt_DMA1_channel0 {DMA1_Stream0_IRQn};
Interrupt interrupt_DMA1_channel1 {DMA1_Stream1_IRQn};
Interrupt interrupt_DMA1_channel2 {DMA1_Stream2_IRQn};
Interrupt interrupt_DMA1_channel3 {DMA1_Stream3_IRQn};
Interrupt interrupt_DMA1_channel4 {DMA1_Stream4_IRQn};
Interrupt interrupt_DMA1_channel5 {DMA1_Stream5_IRQn};
Interrupt interrupt_DMA1_channel6 {DMA1_Stream6_IRQn};
Interrupt interrupt_DMA1_channel7 {DMA1_Stream7_IRQn};

Interrupt interrupt_DMA2_channel0 {DMA2_Stream0_IRQn};
Interrupt interrupt_DMA2_channel1 {DMA2_Stream1_IRQn};
Interrupt interrupt_DMA2_channel2 {DMA2_Stream2_IRQn};
Interrupt interrupt_DMA2_channel3 {DMA2_Stream3_IRQn};
Interrupt interrupt_DMA2_channel4 {DMA2_Stream4_IRQn};
Interrupt interrupt_DMA2_channel5 {DMA2_Stream5_IRQn};
Interrupt interrupt_DMA2_channel6 {DMA2_Stream6_IRQn};
Interrupt interrupt_DMA2_channel7 {DMA2_Stream7_IRQn};
#endif

#if defined(STM32F1)
Interrupt interrupt_EXTI0     {EXTI0_IRQn};
Interrupt interrupt_EXTI1     {EXTI1_IRQn};
Interrupt interrupt_EXTI2     {EXTI2_IRQn};
Interrupt interrupt_EXTI3     {EXTI3_IRQn};
Interrupt interrupt_EXTI4     {EXTI4_IRQn};
Interrupt interrupt_EXTI5     {EXTI9_5_IRQn};
Interrupt interrupt_EXTI6     {EXTI9_5_IRQn};
Interrupt interrupt_EXTI7     {EXTI9_5_IRQn};
Interrupt interrupt_EXTI8     {EXTI9_5_IRQn};
Interrupt interrupt_EXTI9     {EXTI9_5_IRQn};
Interrupt interrupt_EXTI10    {EXTI15_10_IRQn};
Interrupt interrupt_EXTI11    {EXTI15_10_IRQn};
Interrupt interrupt_EXTI12    {EXTI15_10_IRQn};
Interrupt interrupt_EXTI13    {EXTI15_10_IRQn};
Interrupt interrupt_EXTI14    {EXTI15_10_IRQn};
Interrupt interrupt_EXTI15    {EXTI15_10_IRQn};
#endif

template<mcu::Periph v>
auto& get_interrupt()
{
    if      constexpr (v == mcu::Periph::TIM1)         return interrupt_tim1;
    else if constexpr (v == mcu::Periph::TIM3)         return interrupt_tim3;
    else if constexpr (v == mcu::Periph::USART1)       return interrupt_usart1;
    else if constexpr (v == mcu::Periph::DMA1_stream1) return interrupt_DMA1_channel1;
    else if constexpr (v == mcu::Periph::DMA1_stream2) return interrupt_DMA1_channel2;
    else if constexpr (v == mcu::Periph::DMA1_stream3) return interrupt_DMA1_channel3;
    else if constexpr (v == mcu::Periph::DMA1_stream4) return interrupt_DMA1_channel4;
    else if constexpr (v == mcu::Periph::DMA1_stream5) return interrupt_DMA1_channel5;
#if defined(STM32F1) 
    else if constexpr (v == mcu::Periph::USART2)       return interrupt_usart2;
    else if constexpr (v == mcu::Periph::USART3)       return interrupt_usart3;
#endif

#if defined(STM32F4) 
    else if constexpr (v == mcu::Periph::USART2)       return interrupt_usart2;
    else if constexpr (v == mcu::Periph::USART3)       return interrupt_usart3;

    else if constexpr (v == mcu::Periph::DMA1_stream0) return interrupt_DMA1_channel0;
    else if constexpr (v == mcu::Periph::DMA1_stream1) return interrupt_DMA1_channel1;
    else if constexpr (v == mcu::Periph::DMA1_stream2) return interrupt_DMA1_channel2;
    else if constexpr (v == mcu::Periph::DMA1_stream3) return interrupt_DMA1_channel3;
    else if constexpr (v == mcu::Periph::DMA1_stream4) return interrupt_DMA1_channel4;
    else if constexpr (v == mcu::Periph::DMA1_stream5) return interrupt_DMA1_channel5;
    else if constexpr (v == mcu::Periph::DMA1_stream6) return interrupt_DMA1_channel6;
    else if constexpr (v == mcu::Periph::DMA1_stream7) return interrupt_DMA1_channel7;

    else if constexpr (v == mcu::Periph::DMA2_stream0) return interrupt_DMA2_channel0;
    else if constexpr (v == mcu::Periph::DMA2_stream1) return interrupt_DMA2_channel1;
    else if constexpr (v == mcu::Periph::DMA2_stream2) return interrupt_DMA2_channel2;
    else if constexpr (v == mcu::Periph::DMA2_stream3) return interrupt_DMA2_channel3;
    else if constexpr (v == mcu::Periph::DMA2_stream4) return interrupt_DMA2_channel4;
    else if constexpr (v == mcu::Periph::DMA2_stream5) return interrupt_DMA2_channel5;
    else if constexpr (v == mcu::Periph::DMA2_stream6) return interrupt_DMA2_channel6;
    else if constexpr (v == mcu::Periph::DMA2_stream7) return interrupt_DMA2_channel7;
#endif
#if defined(STM32F1)
    else if constexpr (v == mcu::Periph::DMA1_stream1) return interrupt_DMA1_channel1;
    else if constexpr (v == mcu::Periph::DMA1_stream2) return interrupt_DMA1_channel2;
    else if constexpr (v == mcu::Periph::DMA1_stream3) return interrupt_DMA1_channel3;
    else if constexpr (v == mcu::Periph::DMA1_stream4) return interrupt_DMA1_channel4;
    else if constexpr (v == mcu::Periph::DMA1_stream5) return interrupt_DMA1_channel5;
    else if constexpr (v == mcu::Periph::DMA1_stream6) return interrupt_DMA1_channel6;
    else if constexpr (v == mcu::Periph::DMA1_stream7) return interrupt_DMA1_channel7;
#endif
    else if constexpr (v == mcu::Periph::SPI1) return interrupt_spi1;
#if defined(STM32F4)
    else if constexpr (v == mcu::Periph::SPI2) return interrupt_spi2;
    else if constexpr (v == mcu::Periph::SPI3) return interrupt_spi3;
#endif
}


#if defined(STM32F1)
constexpr auto& get_external_interrupt (int n)
{
    if (n == 0)  return interrupt_EXTI0;
    if (n == 1)  return interrupt_EXTI1;
    if (n == 2)  return interrupt_EXTI2;
    if (n == 3)  return interrupt_EXTI3;
    if (n == 4)  return interrupt_EXTI4;
    if (n == 5)  return interrupt_EXTI5;
    if (n == 6)  return interrupt_EXTI6;
    if (n == 7)  return interrupt_EXTI7;
    if (n == 8)  return interrupt_EXTI8;
    if (n == 9)  return interrupt_EXTI9;
    if (n == 10) return interrupt_EXTI10;
    if (n == 11) return interrupt_EXTI11;
    if (n == 12) return interrupt_EXTI12;
    if (n == 13) return interrupt_EXTI13;
    if (n == 14) return interrupt_EXTI14;
    if (n == 15) return interrupt_EXTI15;

}
#endif


#include "interrupts.h"