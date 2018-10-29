#pragma once

#include <cstdint>

namespace mcu::GPIO_bits {

struct CR {
   enum Mode { In_analog = 0b0000, Out10MHz_push_pull,      Out2MHz_push_pull,      Out50MHz_push_pull,
                   In_floating,        Out10MHz_open_drain,     Out2MHz_open_drain,     Out50MHz_open_drain,
                   In_pull,            Out10MHz_push_pull_alt,  Out2MHz_push_pull_alt,  Out50MHz_push_pull_alt,
                   Reserved,           Out10MHz_open_drain_alt, Out2MHz_open_drain_alt, Out50MHz_open_drain_alt
   };
   Mode CNF_MODE0  :4;
   Mode CNF_MODE1  :4;
   Mode CNF_MODE2  :4;
   Mode CNF_MODE3  :4;
   Mode CNF_MODE4  :4;
   Mode CNF_MODE5  :4;
   Mode CNF_MODE6  :4;
   Mode CNF_MODE7  :4;
   Mode CNF_MODE8  :4;
   Mode CNF_MODE9  :4;
   Mode CNF_MODE10 :4;
   Mode CNF_MODE11 :4;
   Mode CNF_MODE12 :4;
   Mode CNF_MODE13 :4;
   Mode CNF_MODE14 :4;
   Mode CNF_MODE15 :4;
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
         uint32_t res1 :16;
      } bits;
   };
}__attribute__((packed));
} // namespace mcu::GPIO_bits