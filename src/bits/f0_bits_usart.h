#pragma once

#include <cstdint>

namespace mcu::USART_bits {

struct SR {
   bool     PE    :1; // Bit 0 PE: Parity error
   bool     FE    :1; // Bit 1 FE: Framing error
   bool     NF    :1; // Bit 2 NF: START bit Noise detection flag
   bool     ORE   :1; // Bit 3 ORE: Overrun error
   bool     IDLE  :1; // Bit 4 IDLE: Idle line detected
   bool     RXNE  :1; // Bit 5 RXNE: Read data register not empty
   bool     TC    :1; // Bit 6 TC: Transmission complete
   bool     TXE   :1; // Bit 7 TXE: Transmit data register empty
   uint32_t       :1; // Bit 8 Reserved, must be kept at reset value.
   bool     CTSIF :1; // Bit 9 CTSIF: CTS interrupt flag
   bool     CTS   :1; // Bit 10 CTS: CTS flag
   bool     RTOF  :1; // Bit 11 RTOF: Receiver timeout
   uint32_t       :2; // Bits 13:12 Reserved, must be kept at reset value.
   bool     ABRE  :1; // Bit 14 ABRE: Auto baud rate error
   bool     ABRF  :1; // Bit 15 ABRF: Auto baud rate flag
   bool     BUSY  :1; // Bit 16 BUSY: Busy flag
   bool     CMF   :1; // Bit 17 CMF: Character match flag
   bool     SBKF  :1; // Bit 18 SBKF: Send break flag
   bool     RWU   :1; // Bit 19 RWU: Receiver wakeup from Mute mode
   uint32_t       :12; // Bits 31:20 Reserved, must be kept at reset value.
}__attribute__((packed));

struct CR1 {
   enum Parity     { even = 0b0, odd     };
   enum DataBits   { _8   = 0b0, _9      };
   bool     UE     :1; // Bit 0 UE: USART enable
   uint32_t        :1; // Bit 1 Reserved, must be kept at reset value.
   bool     RE     :1; // Bit 2 RE: Receiver enable
   bool     TE     :1; // Bit 3 TE: Transmitter enable
   bool     IDLEIE :1; // Bit 4 IDLEIE: IDLE interrupt enable
   bool     RXNEIE :1; // Bit 5 RXNEIE: RXNE interrupt enable
   bool     TCIE   :1; // Bit 6 TCIE: Transmission complete interrupt enable
   bool     TXEIE  :1; // Bit 7 TXEIE: interrupt enable
   bool     PEIE   :1; // Bit 8 PEIE: PE interrupt enable
   Parity   PS     :1; // Bit 9 PS: Parity selection
   bool     PCE    :1; // Bit 10 PCE: Parity control enable
   uint32_t WAKE   :1; // Bit 11 WAKE: Receiver wakeup method
   DataBits M0     :1; // Bit 12 M0: Word length
   bool     MME    :1; // Bit 13 MME: Mute mode enable
   bool     CMIE   :1; // Bit 14 CMIE: Character match interrupt enable
   uint32_t OVER8  :1; // Bit 15 OVER8: Oversampling mode
   uint32_t DEDT   :5; // Bits 20:16 DEDT[4:0]: Driver Enable de-assertion time
   uint32_t DEAT   :5; // Bits 25:21 DEAT[4:0]: Driver Enable assertion time
   bool     RTOIE  :1; // Bit 26 RTOIE: Receiver timeout interrupt enable
   uint32_t        :1; // Bit 27
   uint32_t M1     :1; // Bit 28 M1: Word length
   uint32_t        :3; // Bits 31:29 Reserved, must be kept at reset value
}__attribute__((packed));

struct CR2 {
   enum StopBits { _1 = 0b00, _2 = 0b10 };
   uint32_t          :4; // Bits 3:0 Reserved, must be kept at reset value.
   uint32_t ADDM7    :1; // Bit 4 ADDM7:7-bit Address Detection/4-bit Address Detection
   uint32_t          :3; // Bits 7:5 Reserved, must be kept at reset value.
   uint32_t LBCL     :1; // Bit 8 LBCL: Last bit clock pulse
   uint32_t CPHA     :1; // Bit 9 CPHA: Clock phase
   uint32_t CPOL     :1; // Bit 10 CPOL: Clock polarity
   bool     CLKEN    :1; // Bit 11 CLKEN: Clock enable
   StopBits STOP     :2; // Bits 13:12 STOP[1:0]: STOP bits
   uint32_t          :1; // Bit 14 Reserved, must be kept at reset value.
   uint32_t SWAP     :1; // Bit 15 SWAP: Swap TX/RX pins
   bool     RXINV    :1; // Bit 16 RXINV: RX pin active level inversion
   bool     TXINV    :1; // Bit 17 TXINV: TX pin active level inversion
   bool     DATAINV  :1; // Bit 18 DATAINV: Binary data inversion
   uint32_t MSBFIRST :1; // Bit 19 MSBFIRST: Most significant bit first
   bool     ABREN    :1; // Bit 20 ABREN: Auto baud rate enable
   uint32_t ABRMOD   :2; // Bits 22:21 ABRMOD[1:0]: Auto baud rate mode
   uint32_t RTOEN    :1; // Bit 23 RTOEN: Receiver timeout enable
   uint32_t ADD1     :4; // Bits 27:24 ADD[3:0]: Address of the USART node
   uint32_t ADD2     :4; // Bits 31:28 ADD[7:4]: Address of the USART node
}__attribute__((packed));

struct CR3 {
   bool     EIE    :1; // Bit 0 EIE: Error interrupt enable
   uint32_t        :2; // Bits 2:1 Reserved, must be kept at reset value.
   bool     HDSEL  :1; // Bit 3 HDSEL: Half-duplex selection
   uint32_t        :2; // Bits 5:4 Reserved, must be kept at reset value.
   bool     DMAR   :1; // Bit 6 DMAR: DMA enable receiver
   bool     DMAT   :1; // Bit 7 DMAT: DMA enable transmitter
   bool     RTSE   :1; // Bit 8 RTSE: RTS enable
   bool     CTSE   :1; // Bit 9 CTSE: CTS enable
   bool     CTSIE  :1; // Bit 10 CTSIE: CTS interrupt enable
   bool     ONEBIT :1; // Bit 11 ONEBIT: One sample bit method enable
   bool     OVRDIS :1; // Bit 12 OVRDIS: Overrun Disable
   bool     DDRE   :1; // Bit 13 DDRE: DMA Disable on Reception Error
   bool     DEM    :1; // Bit 14 DEM: Driver enable mode
   bool     DEP    :1; // Bit 15 DEP: Driver enable polarity selection
   uint32_t        :16; // Bits 31:16 Reserved, must be kept at reset value.
}__attribute__((packed));


} // namespace mcu::USART_bits