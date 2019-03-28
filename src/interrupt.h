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

Interrupt interrupt_usart1 {USART1_IRQn};
#if defined(STM32F1)
Interrupt interrupt_usart2 {USART2_IRQn};
Interrupt interrupt_usart3 {USART3_IRQn};
#endif
#if defined(STM32F4)
Interrupt interrupt_usart2 {USART2_IRQn};
Interrupt interrupt_usart3 {USART3_IRQn};
#endif


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

Interrupt interrupt_DMA2_channel0 {DMA1_Stream0_IRQn};
Interrupt interrupt_DMA2_channel1 {DMA1_Stream1_IRQn};
Interrupt interrupt_DMA2_channel2 {DMA1_Stream2_IRQn};
Interrupt interrupt_DMA2_channel3 {DMA1_Stream3_IRQn};
Interrupt interrupt_DMA2_channel4 {DMA1_Stream4_IRQn};
Interrupt interrupt_DMA2_channel5 {DMA1_Stream5_IRQn};
Interrupt interrupt_DMA2_channel6 {DMA1_Stream6_IRQn};
Interrupt interrupt_DMA2_channel7 {DMA1_Stream7_IRQn};
#endif

template<mcu::Periph v>
auto& get_interrupt()
{
    if      constexpr (v == mcu::Periph::USART1)       return interrupt_usart1;
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
    else if constexpr (v == mcu::Periph::DMA1_stream6) return interrupt_DMA1_channel6;
    else if constexpr (v == mcu::Periph::DMA1_stream7) return interrupt_DMA1_channel7;
#endif
} 


#include "interrupts.h"