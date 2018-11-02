#pragma once

#include <cstdint>

namespace mcu::DMA_bits {

enum class Channel { _1 = 1, _2, _3, _4, _5, _6, _7, error };

struct CCR {
   enum DataSize      { byte8 = 0b00, word16, dword32 };
   enum Priority      { low = 0b00, medium, high, veryHi = 0b11 };
   enum DataDirection { PerToMem = 0b0, MemToPer };
   bool          EN     :1; // Bit 0 EN: Stream enable / flag stream ready when read low
   bool          TCIE   :1; // Bit 1 TCIE: Transfer complete interrupt enable
   bool          HTIE   :1; // Bit 2 HTIE: Half transfer interrupt enable
   bool          TEIE   :1; // Bit 3 TEIE: Transfer error interrupt enable
   DataDirection DIR    :1; // Bit 4 DIR: Data transfer direction
   bool          CIRC   :1; // Bit 5 CIRC: Circular mode
   bool          PINC   :1; // Bit 6 PINC: Peripheral increment mode
   bool          MINC   :1; // Bit 7 MINC: Memory increment mode
   DataSize      PSIZE  :2; // Bits 9:8 PSIZE[1:0]: Peripheral size
   DataSize      MSIZE  :2; // Bits 11:10 MSIZE[1:0]: Memory size
   Priority      PL     :2; // Bits 13:12 PL[1:0]: Channel priority level
   bool          MEM2MEM:1; // Bit 14 MEM2MEM: Memory to memory mode
   uint32_t      res1   :17; // Bits 31:15 Reserved, must be kept at reset value.

}__attribute__((packed));

} // namespace mcu::DMA_bits