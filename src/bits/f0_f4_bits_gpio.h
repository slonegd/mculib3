#pragma once

#include <cstdint>

namespace mcu::GPIO_bits {

struct MODER {
   enum Mode { Input = 0b00, Output, Alternate, Analog };
   Mode MODE0  :2;
   Mode MODE1  :2;
   Mode MODE2  :2;
   Mode MODE3  :2;
   Mode MODE4  :2;
   Mode MODE5  :2;
   Mode MODE6  :2;
   Mode MODE7  :2;
   Mode MODE8  :2;
   Mode MODE9  :2;
   Mode MODE10 :2;
   Mode MODE11 :2;
   Mode MODE12 :2;
   Mode MODE13 :2;
   Mode MODE14 :2;
   Mode MODE15 :2;
}__attribute__((packed));

struct OTYPER {
   enum OutType  {PushPull = 0b0, OpenDrain};
   OutType OT0  :1;
   OutType OT1  :1;
   OutType OT2  :1;
   OutType OT3  :1;
   OutType OT4  :1;
   OutType OT5  :1;
   OutType OT6  :1;
   OutType OT7  :1;
   OutType OT8  :1;
   OutType OT9  :1;
   OutType OT10 :1;
   OutType OT11 :1;
   OutType OT12 :1;
   OutType OT13 :1;
   OutType OT14 :1;
   OutType OT15 :1;
   uint32_t     :16;
}__attribute__((packed));

struct OSPEEDR {
   enum OutSpeed { Low = 0b00, Medium, High,
   #if defined(STM32F4)
      VeryHigh
   #endif
   };
   OutSpeed OSPEEDR0  :2;
   OutSpeed OSPEEDR1  :2;
   OutSpeed OSPEEDR2  :2;
   OutSpeed OSPEEDR3  :2;
   OutSpeed OSPEEDR4  :2;
   OutSpeed OSPEEDR5  :2;
   OutSpeed OSPEEDR6  :2;
   OutSpeed OSPEEDR7  :2;
   OutSpeed OSPEEDR8  :2;
   OutSpeed OSPEEDR9  :2;
   OutSpeed OSPEEDR10 :2;
   OutSpeed OSPEEDR11 :2;
   OutSpeed OSPEEDR12 :2;
   OutSpeed OSPEEDR13 :2;
   OutSpeed OSPEEDR14 :2;
   OutSpeed OSPEEDR15 :2;
}__attribute__((packed));

struct PUPDR {
   enum PullResistor { No = 0b00, Up, Down };
   PullResistor PUPDR0  :2;
   PullResistor PUPDR1  :2;
   PullResistor PUPDR2  :2;
   PullResistor PUPDR3  :2;
   PullResistor PUPDR4  :2;
   PullResistor PUPDR5  :2;
   PullResistor PUPDR6  :2;
   PullResistor PUPDR7  :2;
   PullResistor PUPDR8  :2;
   PullResistor PUPDR9  :2;
   PullResistor PUPDR10 :2;
   PullResistor PUPDR11 :2;
   PullResistor PUPDR12 :2;
   PullResistor PUPDR13 :2;
   PullResistor PUPDR14 :2;
   PullResistor PUPDR15 :2;
}__attribute__((packed));

struct DR  { // для IDR ODR
   union {
      uint32_t reg;
      struct {
         bool _0  :1;
         bool _1  :1;
         bool _2  :1;
         bool _3  :1;
         bool _4  :1;
         bool _5  :1;
         bool _6  :1;
         bool _7  :1;
         bool _8  :1;
         bool _9  :1;
         bool _10 :1;
         bool _11 :1;
         bool _12 :1;
         bool _13 :1;
         bool _14 :1;
         bool _15 :1;
         uint32_t :16;
      } bits;
   };
}__attribute__((packed));

struct AFR {
   enum AF { _0 = 0b0000, _1, _2, _3, _4, _5, _6, _7,
   #if defined(STM32F4)
      _8, _9, _10, _11, _12, _13, _14, _15
   #endif
   };
   AF AF0  : 4;
   AF AF1  : 4;
   AF AF2  : 4;
   AF AF3  : 4;
   AF AF4  : 4;
   AF AF5  : 4;
   AF AF6  : 4;
   AF AF7  : 4;
   AF AF8  : 4;
   AF AF9  : 4;
   AF AF10 : 4;
   AF AF11 : 4;
   AF AF12 : 4;
   AF AF13 : 4;
   AF AF14 : 4;
   AF AF15 : 4;
}__attribute__((packed));

} // namespace mcu::GPIO {