#pragma once

#include <cstdint>

namespace mcu::USB_core_bits {

struct GCCFG {
   bool     DCDET  :1;  // Bit 0 DCDET: Data contact detection (DCD) status
   uint32_t        :15; // Bits 15:1 Reserved, must be kept at reset value.
   bool     PWRDWN :1;  // Bit 16 PWRDWN: Power down control
   uint32_t        :1;  // Bit 17 Reserved, must be kept at reset value.
   bool     DCDEN  :1;  // Bit 18 DCDEN: Data contact detection (DCD) mode enable
   uint32_t        :2;  // Bits 20:19 Reserved, must be kept at reset value.
   bool     VBDEN  :1;  // Bit 21 VBDEN: USB V BUS detection enable
   uint32_t        :10; // Bits 31:22 Reserved, must be kept at reset value.
}__attribute__((packed));

struct GOTGCTL {
   bool     SRQSCS    :1; // Bit 0 SRQSCS: Session request success
   bool     SRQ       :1; // Bit 1 SRQ: Session request
   bool     VBVALOEN  :1; // Bit 2 VBVALOEN: V BUS valid override enable.
   uint32_t VBVALOVAL :1; // Bit 3 VBVALOVAL: V BUS valid override value.
   bool     AVALOEN   :1; // Bit 4 AVALOEN: A-peripheral session valid override enable.
   uint32_t AVALOVAL  :1; // Bit 5 AVALOVAL: A-peripheral session valid override value.
   bool     BVALOEN   :1; // Bit 6 BVALOEN: B-peripheral session valid override enable.
   uint32_t BVALOVAL  :1; // Bit 7 BVALOVAL: B-peripheral session valid override value.
   bool     HNGSCS    :1; // Bit 8 HNGSCS: Host negotiation success
   bool     HNPRQ     :1; // Bit 9 HNPRQ: HNP request
   bool     HSHNPEN   :1; // Bit 10 HSHNPEN: host set HNP enable
   bool     DHNPEN    :1; // Bit 11 DHNPEN: Device HNP enabled
   bool     EHEN      :1; // Bit 12 EHEN: Embedded host enable
   uint32_t           :3; // Bits 15:13 Reserved, must be kept at reset value.
   uint32_t CIDSTS    :1; // Bit 16 CIDSTS: Connector ID status
   uint32_t DBCT      :1; // Bit 17 DBCT: Long/short debounce time
   bool     ASVLD     :1; // Bit 18 ASVLD: A-session valid
   bool     BSVLD     :1; // Bit 19 BSVLD: B-session valid
   uint32_t OTGVER    :1; // Bit 20 OTGVER: OTG version
   uint32_t CURMOD    :1; // Bit 21 CURMOD: Current mode of operation
   uint32_t           :10;// Bits 31:22 Reserved, must be kept at reset value.
}__attribute__((packed));

struct GRSTCTL {
   enum TX_FIFO {
      non_periodic_host_or_device_0 = 0b0000,
      periodic_host_or_device_1,
      device_2, device_3 , device_4 , device_5 , device_6 , device_7 , device_8 ,
      device_9, device_10, device_11, device_12, device_13, device_14, device_15,
      all
   };
   bool     CSRST   :1; // Bit 0 CSRST: Core soft reset
   bool     PSRST   :1; // Bit 1 PSRST: Partial soft reset
   bool     FCRST   :1; // Bit 2 FCRST: Host frame counter reset for USB OTG FS
   uint32_t         :1; // Bit 3 Reserved, must be kept at reset value.
   bool     RXFFLSH :1; // Bit 4 RXFFLSH: Rx FIFO flush
   bool     TXFFLSH :1; // Bit 5 TXFFLSH: Tx FIFO flush
   TX_FIFO  TXFNUM  :5; // Bits 10:6 TXFNUM[4:0]: Tx FIFO number
   uint32_t         :19;// Bits 29:11 Reserved, must be kept at reset value for USB OTG HS.
   bool     DMAREQ  :1; // Bit 30 DMAREQ: DMA request signal enabled for USB OTG HS
   bool     AHBIDL  :1; // Bit 31 AHBIDL: AHB master idle
};


} //namespace mcu::USB_core_bits


