#pragma once

#include <cstdint>

namespace mcu::FLASH_bits {

struct ACR {
   enum Latency { _0_24MHz = 0b000, _24_48MHz = 0b001, _48_72MHz = 0b010};
   Latency  LATENCY :3;  // Bits 2:0 LATENCY[2:0]: Latency
   bool     HLFCYA  :1;  // Bit 3 HLFCYA: Flash half cycle access enable
   bool     PRFTBE  :1;  // Bit 4 PRFTBE: Prefetch buffer enable
   bool     PRFTBS  :1;  // Bit 5 PRFTBS: Prefetch buffer status
   uint32_t         :26; // Bits 31:6 Reserved, must be kept cleared.
}__attribute__((packed));

} // namespace mcu::FLASH_bits {