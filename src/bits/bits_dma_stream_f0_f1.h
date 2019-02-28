#pragma once

#include <cstdint>

namespace mcu::DMA_bits {

struct CCR {
   enum DataSize  { byte8 = 0b00, word16, dword32 };
   enum Priority  { low = 0b00, medium, high, very_hi = 0b11 };
   enum Direction { to_memory = 0b0, to_periph };
   bool      EN      :1; // Bit 0 EN: Stream enable / flag stream ready when read low
   bool      TCIE    :1; // Bit 1 TCIE: Transfer complete interrupt enable
   bool      HTIE    :1; // Bit 2 HTIE: Half transfer interrupt enable
   bool      TEIE    :1; // Bit 3 TEIE: Transfer error interrupt enable
   Direction DIR     :1; // Bit 4 DIR: Data transfer direction
   bool      CIRC    :1; // Bit 5 CIRC: Circular mode
   bool      PINC    :1; // Bit 6 PINC: Peripheral increment mode
   bool      MINC    :1; // Bit 7 MINC: Memory increment mode
   DataSize  PSIZE   :2; // Bits 9:8 PSIZE[1:0]: Peripheral size
   DataSize  MSIZE   :2; // Bits 11:10 MSIZE[1:0]: Memory size
   Priority  PL      :2; // Bits 13:12 PL[1:0]: Channel priority level
   bool      MEM2MEM :1; // Bit 14 MEM2MEM: Memory to memory mode
   uint32_t          :17; // Bits 31:15 Reserved, must be kept at reset value.

}__attribute__((packed));

} // namespace mcu::DMA_bits