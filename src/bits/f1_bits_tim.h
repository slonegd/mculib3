#pragma once

#include <cstdint>

namespace mcu::TIM_bits {

struct CR1 {
   enum OnePulseMode { notStopped  = 0b0, counterStop };
   enum Direction { up  = 0b0, down };
   bool         CEN  :1;  // Bit  0 CEN: Counter enable
   uint32_t     UDIS :1;  // Bit  1 UDIS: Update disable
   uint32_t     URS  :1;  // Bit  2 URS: Update request source
   OnePulseMode OPM  :1;  // Bit  3 OPM: One-pulse mode
   Direction    DIR  :1;  // Bit  4 DIR: Direction
   uint32_t     CMS  :2;  // Bits 6:5 CMS: Center-aligned mode selection
   bool         ARPE :1;  // Bit  7 ARPE: Auto-reload preload enable
   uint32_t     CKD  :2;  // Bits 9:8 CKD: Clock division
   uint32_t          :6;  // Bits 15:10 Reserved, must be kept at reset value.
}__attribute__((packed));

struct SR {
   bool UIF   :1; // Bit 0 UIF: Update interrupt flag
   bool CC1IF :1; // Bit 1 CC1IF: Capture/compare 1 interrupt flag
   bool CC2IF :1; // Bit 2 CC2IF: Capture/compare 2 interrupt flag
   bool CC3IF :1; // Bit 3 CC3IF: Capture/compare 3 interrupt flag
   bool CC4IF :1; // Bit 4 CC4IF: Capture/compare 4 interrupt flag
   uint32_t   :1; // Bit 5 Reserved, must be kept at reset value.
   bool TIF   :1; // Bit 6 TIF: Trigger interrupt flag
   uint32_t   :2; // Bit 7-8 Reserved, must be kept at reset value.
   bool CC1OF :1; // Bit 9 CC1OF: Capture/Compare 1 overcapture flag
   bool CC2OF :1; // Bit 10 CC2OF: Capture/Compare 2 overcapture flag
   bool CC3OF :1; // Bit 11 CC3OF: Capture/Compare 3 overcapture flag
   bool CC4OF :1; // Bit 12 CC4OF: Capture/Compare 4 overcapture flag
   uint32_t   :3; // Bit 13-15 Reserved, must be kept at reset value.
}__attribute__((packed));

struct EGR {
   bool UG   :1; // Bit 0 UG:Update generation
   bool CC1G :1; // Bit 1 CC1G: Capture/compare 1 generation
   bool CC2G :1; // Bit 2 CC2G: Capture/compare 2 generation
   bool CC3G :1; // Bit 3 CC3G: Capture/compare 3 generation
   bool CC4G :1; // Bit 4 CC4G: Capture/compare 4 generation
   uint32_t  :1; // Bit 5 Reserved, must be kept at reset value.
   bool TG   :1; // Bit 6 TG: Trigger generation
   uint32_t  :9; // Bit 7-15 Reserved, must be kept at reset value.
}__attribute__((packed));

} //namespace mcu::TIM_bits