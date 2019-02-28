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

struct SR {
   bool     BSY      :1; // Bit 0 BSY: Busy
   uint32_t          :1; // Bit 1 Reserved, must be kept at reset value
   bool     PGERR    :1; // Bit 2 PGERR: Programming error
   uint32_t          :1; // Bit 3 Reserved, must be kept at reset value.
   bool     WRPRTERR :1; // Bit 4 WRPRTERR: Write protection error
   bool     EOP      :1; // Bit 5 EOP: End of operation
   uint32_t          :26;
}__attribute__((packed));

struct CR {
   bool     PG         :1; // Bit 0 PG: Programming
   bool     PER        :1; // Bit 1 SER: Sector Erase
   bool     MER        :1; // Bit 2 MER: Mass Erase
   uint32_t            :1; // Bit 3 Reserved, must be kept at reset value.
   bool     OPTPG      :1; // Bit 4 OPTPG: Option byte programming
   bool     OPTER      :1; // Bit 5 OPTER: Option byte erase
   bool     STRT       :1; // Bit 6 STRT: Start
   bool     LOCK       :1; // Bit 7 LOCK: Lock
   uint32_t            :1; // Bit 8 Reserved, must be kept at reset value.
   bool     OPTWRE     :1; // Bit 9 OPTWRE: Option byte write enable
   bool     ERRIE      :1; // Bit 10 ERRIE: Error interrupt enable
   uint32_t            :1; // Bit 11 Reserved, must be kept at reset value
   bool     EOPIE      :1; // Bit 12 EOPIE: End of operation interrupt enable
   uint32_t            :17; // Bits 31:13 Reserved, must be kept at reset value.
}__attribute__((packed));

} // namespace mcu::FLASH_bits