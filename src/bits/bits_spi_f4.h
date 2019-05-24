#pragma once

#include <cstdint>

namespace mcu::SPI_bits {

    struct CR1 {
        enum Mode {slave = 0b0, master = 0b1};
        enum Polarity {low_level = 0b0, high_level = 0b1};
        enum Edge {front = 0b0, rear = 0b1};
        enum First_bit {high = 0b0, low = 0b1};
        enum Data_size {_8bits = 0b0, _16bits};
        enum Prescaler {div2 = 0b000, div4, div8, div16, div32, div64, div128, div256};

        bool      CPHA     :1;  // Bit 0 CPHA: Clock phase
        Polarity  CPOL     :1;  // Bit 1 CPOL: Clock polarity
        Mode      MSTR     :1;  // Bit 2 MSTR: Master selection
        Prescaler BR       :3;  // Bits 5:3 BR[2:0]: Baud rate control
        bool      SPE      :1;  // Bit 6 SPE: SPI enable
        First_bit LSBFIRST :1;  // Bit 7 LSBFIRST: Frame format
        bool      SSI      :1;  // Bit 8 SSI: Internal slave select
        bool      SSM      :1;  // Bit 9 SSM: Software slave management
        bool      RXONLY   :1;  // Bit 10 RXONLY: Receive only
        Data_size DFF      :1;  // Bit 11 DFF: Data frame format
        bool      CRCNEXT  :1;  // Bit 12 CRCNEXT: CRC transfer next
        bool      CRCEN    :1;  // Bit 13 CRCEN: Hardware CRC calculation enable
        bool      BIDIOE   :1;  // Bit 14 BIDIOE: Output enable in bidirectional mode
        bool      BIDIMODE :1;  // Bit 15 BIDIMODE: Bidirectional data mode enable
        uint32_t           :16; // Bits 31:16 Reserved, must be kept at reset value.
    } __attribute__((packed));

    struct CR2 {
        bool     RXDMAEN :1;  // Bit 0 RXDMAEN: Rx buffer DMA enable
        bool     TXDMAEN :1;  // Bit 1 TXDMAEN: Tx buffer DMA enable
        bool     SSOE    :1;  // Bit 2 SSOE: SS output enable
        uint32_t         :1;  // Bit 3 Reserved. Forced to 0 by hardware.
        bool     FRF     :1;  // Bit 4 FRF: Frame format
        bool     ERRIE   :1;  // Bit 5 ERRIE: Error interrupt enable
        bool     RXNEIE  :1;  // Bit 6 RXNEIE: RX buffer not empty interrupt enable
        bool     TXEIE   :1;  // Bit 7 TXEIE: Tx buffer empty interrupt enable
        uint32_t         :24; // Bits 31:8 Reserved, must be kept at reset value.
    }__attribute__((packed));

    struct SR {
        bool     RXNE   :1;  // Bit 0 RXNE: Receive buffer not empty
        bool     TXE    :1;  // Bit 1 TXE: Transmit buffer empty
        bool     CHSIDE :1;  // Bit 2 CHSIDE: Channel side
        bool     UDR    :1;  // Bit 3 UDR: Underrun flag
        bool     CRCERR :1;  // Bit 4 CRCERR: CRC error flag
        bool     MODF   :1;  // Bit 5 MODF: Mode fault
        bool     OVR    :1;  // Bit 6 OVR: Overrun flag
        bool     BSY    :1;  // Bit 7 BSY: Busy flag
        bool     FRE    :1;  // Bit 8 FRE: Frame format error
        uint32_t        :23; // Bits 31:9 Reserved, must be kept at reset value.
    }__attribute__((packed));
} //namespace mcu::SPI_bits