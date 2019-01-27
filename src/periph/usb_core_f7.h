#pragma once

#include "bits_usb_core_f7.h"
#include "delay.h"
#include <algorithm>
#include <iterator>


namespace mcu {

class USB_core;

class USB_power_clock {
   volatile uint32_t PCGCCTL; // OTG power and clock gating control register (OTG_PCGCCTL)
public:
   static auto& ref(USB_core& v)
   {
      auto base = reinterpret_cast<size_t>(&v);
      return *reinterpret_cast<USB_power_clock*>(base + USB_OTG_PCGCCTL_BASE);
   }
   void restart_phy_clock() { PCGCCTL = 0; }
};

class USB_core {
   volatile USB_core_bits::GOTGCTL GOTGCTL;            // USB_OTG Control and Status Register          000h
   volatile uint32_t               GOTGINT;            // USB_OTG Interrupt Register                   004h
   volatile uint32_t               GAHBCFG;            // Core AHB Configuration Register              008h
   volatile uint32_t               GUSBCFG;            // Core USB Configuration Register              00Ch
   volatile USB_core_bits::GRSTCTL GRSTCTL;            // Core Reset Register                          010h
   volatile uint32_t               GINTSTS;            // Core Interrupt Register                      014h
   volatile uint32_t               GINTMSK;            // Core Interrupt Mask Register                 018h
   volatile uint32_t               GRXSTSR;            // Receive Sts Q Read Register                  01Ch
   volatile uint32_t               GRXSTSP;            // Receive Sts Q Read & POP Register            020h
   volatile uint32_t               GRXFSIZ;            // Receive FIFO Size Register                   024h
   volatile uint32_t               DIEPTXF0_HNPTXFSIZ; // EP0 / Non Periodic Tx FIFO Size Register     028h
   volatile uint32_t               HNPTXSTS;           // Non Periodic Tx FIFO/Queue Sts reg           02Ch
   volatile uint32_t               Reserved30[2];      // Reserved                                     030h
   volatile USB_core_bits::GCCFG   GCCFG;              // General Purpose IO Register                  038h
   volatile uint32_t               CID;                // User ID Register                             03Ch
   volatile uint32_t               Reserved5[3];       // Reserved                                040h-048h
   volatile uint32_t               GHWCFG3;            // User HW config3                              04Ch
   volatile uint32_t               Reserved6;          // Reserved                                     050h
   volatile uint32_t               GLPMCFG;            // LPM Register                                 054h
   volatile uint32_t               GPWRDN;             // Power Down Register                          058h
   volatile uint32_t               GDFIFOCFG;          // DFIFO Software Config Register               05Ch
   volatile uint32_t               GADPCTL;            // ADP Timer, Control and Status Register       60Ch
   volatile uint32_t               Reserved43[39];     // Reserved                                058h-0FFh
   volatile uint32_t               HPTXFSIZ;           // Host Periodic Tx FIFO Size Reg               100h
   volatile uint32_t               DIEPTXF[0x0F];      // dev Periodic Transmit FIFO
public:
   using CMSIS_type = USB_OTG_GlobalTypeDef;
   using TX_FIFO = USB_core_bits::GRSTCTL::TX_FIFO;

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   auto& clear_fifo_size()
   {
      std::fill (std::begin(DIEPTXF), std::end(DIEPTXF), 0);
      return *this;
   }
   auto& enable_vbus_detect (bool v = true) { GCCFG.VBDEN = v; return *this; }
   auto& enable_session_B_periph_override() { GOTGCTL.BVALOEN = true; return *this; }
   auto& set_B_periph_vbusvalid () { GOTGCTL.BVALOVAL = 1; return *this; }
   auto& flush (TX_FIFO v)
   {
      GRSTCTL.TXFNUM = v;
      GRSTCTL.TXFFLSH = true;
      block_delay (20'000);
      while (GRSTCTL.TXFFLSH) {}
      return *this;
   }
   auto& flush_rx_fifo()
   {
      GRSTCTL.RXFFLSH = true;
      block_delay (20'000);
      while (GRSTCTL.RXFFLSH) {}
      return *this;
   }

};

#if not defined(USE_MOCK_USB)
SFINAE(USB_FS_core,USB_core) make_reference() {return *reinterpret_cast<USB_core*>(USB_OTG_FS_PERIPH_BASE + USB_OTG_GLOBAL_BASE);}
SFINAE(USB_HS_core,USB_core) make_reference() {return *reinterpret_cast<USB_core*>(USB_OTG_HS_PERIPH_BASE + USB_OTG_GLOBAL_BASE);}
#endif

} // namespace mcu {
