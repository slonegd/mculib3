#pragma once

#include "bits_exti_f1.h"
#include <bitset>

namespace mcu {

class EXTI
{
    volatile EXTI_bits::R     IMR;     // Interrupt mask register            offset: 0x00
    volatile EXTI_bits::R     EMR;     // Event mask register                offset: 0x04
    volatile EXTI_bits::R     RTSR;    // Rising trigger selection register  offset: 0x08
    volatile EXTI_bits::R     FTSR;    // Falling trigger selection register offset: 0x0C
    volatile EXTI_bits::R     SWIER;   // Software interrupt event register  offset: 0x10
    volatile EXTI_bits::R     PR;      // Pending register                   offset: 0x14
public:
    using CMSIS_type  = EXTI_TypeDef;
    enum Edge {rising, falling, both};

    EXTI& set_trigger    (Edge, int n, bool v = true);
    EXTI& toggle_trigger (Edge, int n);
    EXTI& enable_interrupt (int n) { registr(IMR) |= 1 << n; return *this; }
    template<int first, int last = 0>
    EXTI& clear_interrupt_flags();


};

#if not defined(USE_MOCK_EXTI)
SFINAE(EXTI,EXTI) make_reference() {return *reinterpret_cast<EXTI*>(EXTI_BASE);}
#endif




EXTI& EXTI::set_trigger(Edge edge, int n, bool v)
{
    auto mask = 1 << n;
    if (edge == rising or edge == both) {
        if (v)
            registr(RTSR) |= mask;
        else
            registr(RTSR) &= ~mask;
    }

    if (edge == falling or edge == both) {
        if (v)
            registr(FTSR) |= mask;
        else
            registr(FTSR) &= ~mask;
    }
    
    return *this;
}


EXTI& EXTI::toggle_trigger(Edge edge, int n)
{
    auto mask = 1 << n;
    if (edge == rising or edge == both)
        registr(RTSR) ^= mask;

    if (edge == falling or edge == both)
        registr(FTSR) ^= mask;
    
    return *this;
}

template<int first, int last = 0>
EXTI& EXTI::clear_interrupt_flags()
{
    static_assert (
        first >= 0 and first <= 15 and last >=0 and last <= 15,
        "first and last ust be from 0 to 15"
    );

    std::bitset<32> mask;
    mask[first] = true;
    if (last > first)
        for (auto i = first + 1; i <= last; i++)
            mask[i] = true;
 
    registr(PR) = mask.to_ulong();

    return *this;
}



} // namespace mcu {