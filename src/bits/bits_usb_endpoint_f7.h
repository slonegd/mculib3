#pragma once

#include <cstdint>

namespace mcu::USB_endpoint_bits {

struct DIOEPCTL {
   enum Type { control = 0b00, isochronous, bulk, interrupt };
   uint32_t MPSIZ          :11; // Bits 10:0 MPSIZ[10:0]: Maximum packet size
   uint32_t                :4;  // Bits 14:11 Reserved, must be kept at reset value.
   bool     USBAEP         :1;  // Bit 15 USBAEP: USB active endpoint
   uint32_t EONUM          :1;  // Bit 16 EONUM: Even/odd frame
   uint32_t NAKSTS         :1;  // Bit 17 NAKSTS: NAK status
   Type     EPTYP          :2;  // Bits 19:18 EPTYP[1:0]: Endpoint type
   uint32_t                :1;  // Bit 20 Reserved, must be kept at reset value.
   uint32_t STALL          :1;  // Bit 21 STALL: STALL handshake
   uint32_t TXFNUM         :4;  // Bits 25:22 TXFNUM: Tx FIFO number
   bool     CNAK           :1;  // Bit 26 CNAK: Clear NAK
   bool     SNAK           :1;  // Bit 27 SNAK: Set NAK
   uint32_t SD0PID_SEVNFRM :1;  // Bit 28 SD0PID: Set DATA0 PID // SEVNFRM: Set even frame
   bool     SODDFRM        :1;  // Bit 29 SODDFRM: Set odd frame
   bool     EPDIS          :1;  // Bit 30 EPDIS: Endpoint disable
   bool     EPENA          :1;  // Bit 31 EPENA: Endpoint enable
}__attribute__((packed));

struct DIOEPTSIZ {
   uint32_t XFRSIZ :19; // Bits 18:0 XFRSIZ[18:0]: Transfer size
   uint32_t PKTCNT :10; // Bits 28:19 PKTCNT[9:0]: Packet count
   uint32_t MCNT   :2;  // Bits 30:29 MCNT[1:0]: Multi count
   uint32_t        :1;  // Bit 31 Reserved, must be kept at reset value.
}__attribute__((packed));

} //namespace mcu::USB_core_bits


