#pragma once

#include "bits_usb_device_f7.h"

namespace mcu {

class USB_device {
   volatile USB_device_bits::DCFG     DCFG;           // dev Configuration Register   800h
   volatile uint32_t                  DCTL;           // dev Control Register         804h
   volatile uint32_t                  DSTS;           // dev Status Register (RO)     808h
   volatile uint32_t                  Reserved0C;     // Reserved                     80Ch
   volatile USB_device_bits::DIEPMSK  DIEPMSK;        // dev IN Endpoint Mask         810h
   volatile USB_device_bits::DOEPMSK  DOEPMSK;        // dev OUT Endpoint Mask        814h
   volatile uint32_t                  DAINT;          // dev All Endpoints Itr Reg    818h
   volatile USB_device_bits::DAINTMSK DAINTMSK;       // dev All Endpoints Itr Mask   81Ch
   volatile uint32_t                  Reserved20;     // Reserved                     820h
   volatile uint32_t                  Reserved9;      // Reserved                     824h
   volatile uint32_t                  DVBUSDIS;       // dev VBUS discharge Register  828h
   volatile uint32_t                  DVBUSPULSE;     // dev VBUS Pulse Register      82Ch
   volatile uint32_t                  DTHRCTL;        // dev threshold                830h
   volatile uint32_t                  DIEPEMPMSK;     // dev empty msk                834h
   volatile uint32_t                  DEACHINT;       // dedicated EP interrupt       838h
   volatile uint32_t                  DEACHMSK;       // dedicated EP msk             83Ch
   volatile uint32_t                  Reserved40;     // dedicated EP mask            840h
   volatile uint32_t                  DINEP1MSK;      // dedicated EP mask            844h
   volatile uint32_t                  Reserved44[15]; // Reserved                 844-87Ch
   volatile uint32_t                  DOUTEP1MSK;     // dedicated EP msk             884h
public:
   using CMSIS_type = USB_OTG_DeviceTypeDef;
   using Frame_interval = USB_device_bits::DCFG::Frame_interval;
   using Speed = USB_device_bits::DCFG::Speed;

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   auto& set (Frame_interval v) { DCFG.PFIVL = v; return *this; }
   auto& set (Speed          v) { DCFG.DSPD  = v; return *this; }
   auto& clear_interrupt_flags()
   {
      like_CMSIS().DIEPMSK  = 0;
      like_CMSIS().DOEPMSK  = 0;
      like_CMSIS().DAINTMSK = 0;
      return *this;
   }

};

#if not defined(USE_MOCK_USB)
SFINAE(USB_FS_device,USB_device) make_reference() {return *reinterpret_cast<USB_device*>(USB_OTG_FS_PERIPH_BASE + USB_OTG_DEVICE_BASE);}
SFINAE(USB_HS_device,USB_device) make_reference() {return *reinterpret_cast<USB_device*>(USB_OTG_HS_PERIPH_BASE + USB_OTG_DEVICE_BASE);}
#endif

} // namespace mcu {
