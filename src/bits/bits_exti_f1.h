#pragma once

#include <cstdint>

namespace mcu::EXTI_bits {

struct R {
    bool  _0 :1;
    bool  _1 :1;
    bool  _2 :1;
    bool  _3 :1;
    bool  _4 :1;
    bool  _5 :1;
    bool  _6 :1;
    bool  _7 :1;
    bool  _8 :1;
    bool  _9 :1;
    bool _10 :1;
    bool _11 :1;
    bool _12 :1;
    bool _13 :1;
    bool _14 :1;
    bool _15 :1;
    bool _16 :1;
    bool _17 :1;
    uint32_t :14;// Bits 31:18 Reserved, must be kept at reset value (0).
}__attribute__((packed));

} //namespace mcu::EXTI_bits