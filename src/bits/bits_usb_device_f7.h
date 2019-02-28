#pragma once

#include <cstdint>

namespace mcu::USB_device_bits {

struct DCFG {
   enum Speed { high = 0b00, full, full_using_internal_phy = 0b11 };
   enum Frame_interval { _80_percent = 0b00, _85_percent, _90_percent, _95_percent };
   Speed          DSPD      :2; // Bits 1:0 DSPD[1:0]: Device speed
   uint32_t       NZLSOHSK  :1; // Bit 2 NZLSOHSK: Non-zero-length status OUT handshake
   uint32_t                 :1; // Bit 3 Reserved, must be kept at reset value.
   uint32_t       DAD       :7; // Bits 10:4 DAD[6:0]: Device address
   Frame_interval PFIVL     :2; // Bits 12:11 PFIVL[1:0]: Periodic frame interval
   uint32_t                 :1; // Bit 13 Reserved, must be kept at reset value.
   bool           XCVRDLY   :1; // Bit 14 XCVRDLY: Transceiver delay
   uint32_t       ERRATIM   :1; // Bit 15 ERRATIM: Erratic error interrupt mask
   uint32_t                 :8; // Bits 23:16 Reserved, must be kept at reset value for USB OTG HS.
   uint32_t       PERSCHIVL :2; // Bits 25:24 PERSCHIVL[1:0]: Periodic schedule interval for USB OTG HS
   uint32_t                 :6; // Bits 31:26 Reserved, must be kept at reset value for USB OTG HS.
}__attribute__((packed));

enum Mask { masked, unmasked };

struct DIEPMSK {
   Mask     XFRCM     :1; // Bit 0 XFRCM: Transfer completed interrupt mask
   Mask     EPDM      :1; // Bit 1 EPDM: Endpoint disabled interrupt mask
   Mask     AHBERRM   :1; // Bit 2 AHBERRM: AHB error mask for USB OTG HS
   Mask     TOM       :1; // Bit 3 TOM: Timeout condition mask (Non-isochronous endpoints)
   Mask     ITTXFEMSK :1; // Bit 4 ITTXFEMSK: IN token received when Tx FIFO empty mask
   Mask     INEPNMM   :1; // Bit 5 INEPNMM: IN token received with EP mismatch mask
   Mask     INEPNEM   :1; // Bit 6 INEPNEM: IN endpoint NAK effective mask
   uint32_t           :1; // Bit 7 Reserved, must be kept at reset value.
   Mask     TXFURM    :1; // Bit 8 TXFURM: FIFO underrun mask
   uint32_t           :4; // Bits 12:9 Reserved, must be kept at reset value.
   Mask     NAKM      :1; // Bit 13 NAKM: NAK interrupt mask
   uint32_t           :18;// Bits 31:14 Reserved, must be kept at reset value.
}__attribute__((packed));

struct DOEPMSK {
   Mask     XFRCM      :1; // Bit 0 XFRCM: Transfer completed interrupt mask
   Mask     EPDM       :1; // Bit 1 EPDM: Endpoint disabled interrupt mask
   Mask     AHBERRM    :1; // Bit 2 AHBERRM: AHB error mask for USB OTG HS
   Mask     STUPM      :1; // Bit 3 STUPM: SETUP phase done mask. Applies to control endpoints only.
   Mask     OTEPDM     :1; // Bit 4 OTEPDM: OUT token received when endpoint disabled mask. Applies to control OUT
   Mask     STSPHSRXM  :1; // Bit 5 STSPHSRXM: Status phase received for control write mask
   Mask     B2BSTUPM   :1; // Bit 6 B2BSTUPM: Back-to-back SETUP packets received mask for USB OTG HS
   uint32_t            :1; // Bit 7 Reserved, must be kept at reset value.
   Mask     OUTPKTERRM :1; // Bit 8 OUTPKTERRM: Out packet error mask
   uint32_t            :3; // Bits 11:9 Reserved, must be kept at reset value.
   Mask     BERRM      :1; // Bit 12 BERRM: Babble error interrupt mask
   Mask     NAKMSK     :1; // Bit 13 NAKMSK: NAK interrupt mask
   Mask     NYETMSK    :1; // Bit 14 NYETMSK: NYET interrupt mask for USB OTG HS
   uint32_t            :17;// Bits 31:15 Reserved, must be kept at reset value.
}__attribute__((packed));

struct DAINTMSK {
   Mask IN_EP_0   :1; // Bits 15:0 IEPM[15:0]: IN EP interrupt mask bits
   Mask IN_EP_1   :1;
   Mask IN_EP_2   :1;
   Mask IN_EP_3   :1;
   Mask IN_EP_4   :1;
   Mask IN_EP_5   :1;
   Mask IN_EP_6   :1;
   Mask IN_EP_7   :1;
   Mask IN_EP_8   :1;
   Mask IN_EP_9   :1;
   Mask IN_EP_10  :1;
   Mask IN_EP_11  :1;
   Mask IN_EP_12  :1;
   Mask IN_EP_13  :1;
   Mask IN_EP_14  :1;
   Mask IN_EP_15  :1;
   Mask OUT_EP_0  :1; // Bits 31:16 OEPM[15:0]: OUT EP interrupt mask bits
   Mask OUT_EP_1  :1;
   Mask OUT_EP_2  :1;
   Mask OUT_EP_3  :1;
   Mask OUT_EP_4  :1;
   Mask OUT_EP_5  :1;
   Mask OUT_EP_6  :1;
   Mask OUT_EP_7  :1;
   Mask OUT_EP_8  :1;
   Mask OUT_EP_9  :1;
   Mask OUT_EP_10 :1;
   Mask OUT_EP_11 :1;
   Mask OUT_EP_12 :1;
   Mask OUT_EP_13 :1;
   Mask OUT_EP_14 :1;
   Mask OUT_EP_15 :1;
}__attribute__((packed));

} //namespace mcu::USB_device_bits



