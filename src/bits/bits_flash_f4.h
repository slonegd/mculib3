#pragma once

#include <cstdint>

namespace mcu::FLASH_bits {

struct ACR {
   enum Latency { _0 = 0, _1, _2, _3,_4, _5, _6, _7 };
   Latency  LATENCY :3; // Bits 2:0 LATENCY[2:0]: Latency
   uint32_t         :5; // Bits 7:3 Reserved, must be kept cleared.
   bool     PRFTEN  :1; // Bit 8 PRFTEN: Prefetch enable
   bool     ICEN    :1; // Bit 9 ICEN: Instruction cache enable
   bool     DCEN    :1; // Bit 10 DCEN: Data cache enable
   bool     ICRST   :1; // Bit 11 ICRST: Instruction cache reset
   bool     DCRST   :1; // Bit 12 DCRST: Data cache reset
   uint32_t         :19; // Bits 31:13 Reserved, must be kept cleared.
}__attribute__((packed));

struct SR {
   bool     EOP    :1; // Bit 0 EOP: End of operation
   bool     OPERR  :1; // Bit 1 OPERR: Operation error
   uint32_t        :2; // Bits 3:2 Reserved, must be kept cleared.
   bool     WRPERR :1; // Bit 4 WRPERR: Write protection error
   bool     PGAERR :1; // Bit 5 PGAERR: Programming alignment error
   bool     PGPERR :1; // Bit 6 PGPERR: Programming parallelism error
   bool     PGSERR :1; // Bit 7 PGSERR: Programming sequence error
   uint32_t        :8; // Bits 15:8 Reserved, must be kept cleared.
   bool     BSY    :1; // Bit 16 BSY: Busy
   uint32_t        :15; // Bits 31:17 Reserved, must be kept cleared.
}__attribute__((packed));

struct CR {
   enum Sector { _0 = 0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11 };
   enum ProgSize { x8  = 0b00, x16, x32, x64 };
   bool     PG    :1; // Bit 0 PG: Programming
   bool     SER   :1; // Bit 1 SER: Sector Erase
   bool     MER   :1; // Bit 2 MER: Mass Erase
   Sector   SNB   :4; // Bits 6:3 SNB[3:0]: Sector number
   uint32_t       :1; // Bit 7 Reserved, must be kept cleared.
   ProgSize PSIZE :2; // Bits 9:8 PSIZE[1:0]: Program size
   uint32_t       :6; // Bits 15:10 Reserved, must be kept cleared.
   bool     STRT  :1; // Bit 16 STRT: Start
   uint32_t       :7; // Bits 23:17 Reserved, must be kept cleared.
   bool     EOPIE :1; // Bit 24 EOPIE: End of operation interrupt enable
   bool     ERRIE :1; // Bit 25 ERRIE: Error interrupt enable
   uint32_t       :5; // Bits 30:26 Reserved, must be kept cleared.
   bool     LOCK  :1; // Bit 31 LOCK: Lock
}__attribute__((packed));


} // namespace mcu::FLASH_bits {

