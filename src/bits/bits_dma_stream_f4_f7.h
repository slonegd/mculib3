#pragma once

#include <cstdint>

namespace mcu::DMA_bits {

struct CR {
   enum DataSize      { byte8 = 0b00, word16, dword32 };
   enum Priority      { low = 0b00, medium, high, veryHi = 0b11 };
   enum Direction     { to_memory = 0b0, to_periph };
   enum Channel { _0 = 0b000, _1, _2, _3, _4, _5, _6, _7 };
   bool          EN     :1; // Bit 0 EN: Stream enable / flag stream ready when read low
   bool          DMEIE  :1; // Bit 1 DMEIE: Direct mode error interrupt enable
   bool          TEIE   :1; // Bit 2 TEIE: Transfer error interrupt enable
   bool          HTIE   :1; // Bit 3 HTIE: Half transfer interrupt enable
   bool          TCIE   :1; // Bit 4 TCIE: Transfer complete interrupt enable
   bool          PFCTRL :1; // Bit 5 PFCTRL: Peripheral flow controller
   Direction DIR    :2; // Bits 7:6 DIR[1:0]: Data transfer direction
   bool          CIRC   :1; // Bit 8 CIRC: Circular mode
   bool          PINC   :1; // Bit 9 PINC: Peripheral increment mode
   bool          MINC   :1; // Bit 10 MINC: Memory increment mode
   DataSize      PSIZE  :2; // Bits 12:11 PSIZE[1:0]: Peripheral data size
   DataSize      MSIZE  :2; // Bits 14:13 MSIZE[1:0]: Memory data size
   uint32_t      PINCOS :1; // Bit 15 PINCOS: Peripheral increment offset size
   Priority      PL     :2; // Bits 17:16 PL[1:0]: Priority level
   uint32_t      DBM    :1; // Bit 18 DBM: Double buffer mode
   uint32_t      CT     :1; // Bit 19 CT: Current target (only in double buffer mode)
   uint32_t      res1   :1; // Bit 20 Reserved, must be kept at reset value.
   uint32_t      PBURST :2; // Bits 22:21 PBURST[1:0]: Peripheral burst transfer configuration
   uint32_t      MBURST :2; // Bits 24:23 MBURST: Memory burst transfer configuration
   Channel       CHSEL  :3; // Bits 27:25 CHSEL[2:0]: Channel selection
   uint32_t             :4; // Bits 31:28 Reserved, must be kept at reset value.

}__attribute__((packed));

} // namespace mcu::DMA_bits



