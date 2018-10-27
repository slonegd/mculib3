#pragma once

#include <cstdint>

namespace mcu::USART_bits {

struct SR {
   bool     PE   :1; // Bit 0 PE: Parity error
   bool     FE   :1; // Bit 1 FE: Framing error
   bool     NF   :1; // Bit 2 NF: Noise detected flag
   bool     ORE  :1; // Bit 3 ORE: Overrun error
   bool     IDLE :1; // Bit 4 IDLE: IDLE line detected
   bool     RXNE :1; // Bit 5 RXNE: Read data register not empty
   bool     TC   :1; // Bit 6 TC: Transmission complete
   bool     TXE  :1; // Bit 7 TXE: Transmit data register empty
   bool     LBD  :1; // Bit 8 LBD: LIN break detection flag
   bool     CTS  :1; // Bit 9 CTS: CTS flag
   uint32_t res1 :22; // Bits 31:10 Reserved, must be kept at reset value
}__attribute__((packed));

struct CR1 {
   enum Parity     { even = 0b0, odd     };
   enum WakeMethod { idle = 0b0, address };
   enum DataBits   { _8   = 0b0, _9      };
   enum OverSample { by8  = 0b0, by16    };
   bool       SBK    :1; // Bit 0 SBK: Send break
   bool       RWU    :1; // Bit 1 RWU: Receiver wakeup
   bool       RE     :1; // Bit 2 RE: Receiver enable
   bool       TE     :1; // Bit 3 TE: Transmitter enable
   bool       IDLEIE :1; // Bit 4 IDLEIE: IDLE interrupt enable
   bool       RXNEIE :1; // Bit 5 RXNEIE: RXNE interrupt enable
   bool       TCIE   :1; // Bit 6 TCIE: Transmission complete interrupt enable
   bool       TXEIE  :1; // Bit 7 TXEIE: TXE interrupt enable
   bool       PEIE   :1; // Bit 8 PEIE: PE interrupt enable
   Parity     PS     :1; // Bit 9 PS: Parity selection
   bool       PCE    :1; // Bit 10 PCE: Parity control enable
   WakeMethod WAKE   :1; // Bit 11 WAKE: Wakeup method
   DataBits   M      :1; // Bit 12 M: Word length
   bool       UE     :1; // Bit 13 UE: USART enable
   uint32_t   res1   :1; // Bit 14 Reserved, must be kept at reset value
   OverSample OVER8  :1; // Bit 15 OVER8: Oversampling mode
   uint32_t   res2   :16; // Bits 31:16 Reserved, must be kept at reset value
}__attribute__((packed));

struct CR2 {
   enum BreakDetection { _10bit = 0b0, _11bit };
   enum StopBits       { _1 = 0b00, _0_5, _2, _1_5 };
   uint32_t       ADD   :4; // Bits 3:0 ADD[3:0]: Address of the USART node
   uint32_t       res1  :1; // Bit 4 Reserved, must be kept at reset value
   BreakDetection LBDL  :1; // Bit 5 LBDL: lin break detection length
   bool           LBDIE :1; // Bit 6 LBDIE: LIN break detection interrupt enable
   bool           res2  :1; // Bit 7 Reserved, must be kept at reset value
   uint32_t       LBCL  :1; // Bit 8 LBCL: Last bit clock pulse
   uint32_t       CPHA  :1; // Bit 9 CPHA: Clock phase
   uint32_t       CPOL  :1; // Bit 10 CPOL: Clock polarity
   bool           CLKEN :1; // Bit 11 CLKEN: Clock enable
   StopBits       STOP  :2; // Bits 13:12 STOP: STOP bits
   bool           LINEN :1; // Bit 14 LINEN: LIN mode enable
   uint32_t       res3  :17; // Bits 31:15 Reserved, must be kept at reset value
}__attribute__((packed));

struct CR3 {
   bool     EIE    :1; // Bit 0 EIE: Error interrupt enable
   bool     IREN   :1; // Bit 1 IREN: IrDA mode enable
   bool     IRLP   :1; // Bit 2 IRLP: IrDA low-power
   bool     HDSEL  :1; // Bit 3 HDSEL: Half-duplex selection
   bool     NACK   :1; // Bit 4 NACK: Smartcard NACK enable
   bool     SCEN   :1; // Bit 5 SCEN: Smartcard mode enable
   bool     DMAR   :1; // Bit 6 DMAR: DMA enable receiver
   bool     DMAT   :1; // Bit 7 DMAT: DMA enable transmitter
   bool     RTSE   :1; // Bit 8 RTSE: RTS enable
   bool     CTSE   :1; // Bit 9 CTSE: CTS enable
   bool     CTSIE  :1; // Bit 10 CTSIE: CTS interrupt enable
   bool     ONEBIT :1; // Bit 11 ONEBIT: One sample bit method enable
   uint32_t res1   :20; // Bits 31:12 Reserved, must be kept at reset value
}__attribute__((packed));


} // namespace mcu::USART_bits