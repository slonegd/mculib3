#pragma once

#include "bits_usb_endpoint_f7.h"

namespace mcu {

class USB_endpoint {
   volatile USB_endpoint_bits::DIOEPCTL  DIOEPCTL;   // dev IN/OUT Endpoint Control Reg    (ep_num * 20h) + 00h
   volatile uint32_t                     Reserved04; // Reserved                           (ep_num * 20h) + 04h
   volatile uint32_t                     DIOEPINT;   // dev IN/OUT Endpoint Itr Reg        (ep_num * 20h) + 08h
   volatile uint32_t                     Reserved0C; // Reserved                           (ep_num * 20h) + 0Ch
   volatile USB_endpoint_bits::DIOEPTSIZ DIOEPTSIZ;  // IN/OUT Endpoint Txfer Size         (ep_num * 20h) + 10h
   volatile uint32_t                     DIOEPDMA;   // IN/OUT Endpoint DMA Address Reg    (ep_num * 20h) + 14h
   volatile uint32_t                     DTXFSTS;    // IN Endpoint Tx FIFO Status Reg     (ep_num * 20h) + 18h
   volatile uint32_t                     Reserved18; // Reserved                           (ep_num * 20h) + 1Ch
public:
   using CMSIS_type = USB_OTG_INEndpointTypeDef;

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   bool  is_enable()           { return DIOEPCTL.EPENA; }
   auto& disable()             { DIOEPCTL.EPDIS = true;     return *this; }
   auto& set_NAK()             { DIOEPCTL.SNAK  = true;     return *this; }
   auto& reset_control()       { like_CMSIS().DIEPCTL  = 0; return *this; }
   auto& clear_transfer_size() { like_CMSIS().DIEPTSIZ = 0; return *this; }

};

#if not defined(USE_MOCK_USB)
template <Periph p> std::enable_if_t<p == Periph::USB_FS_IN, USB_endpoint(&)[15]> make_reference()
{
   return *reinterpret_cast<USB_endpoint(*)[15]>(USB_OTG_FS_PERIPH_BASE + USB_OTG_IN_ENDPOINT_BASE);
}
template <Periph p> std::enable_if_t<p == Periph::USB_FS_OUT, USB_endpoint(&)[15]> make_reference()
{
   return *reinterpret_cast<USB_endpoint(*)[15]>(USB_OTG_FS_PERIPH_BASE + USB_OTG_OUT_ENDPOINT_BASE);
}
template <Periph p> std::enable_if_t<p == Periph::USB_HS_IN, USB_endpoint(&)[15]> make_reference()
{
   return *reinterpret_cast<USB_endpoint(*)[15]>(USB_OTG_HS_PERIPH_BASE + USB_OTG_IN_ENDPOINT_BASE);
}
template <Periph p> std::enable_if_t<p == Periph::USB_HS_OUT, USB_endpoint(&)[15]> make_reference()
{
   return *reinterpret_cast<USB_endpoint(*)[15]>(USB_OTG_HS_PERIPH_BASE + USB_OTG_OUT_ENDPOINT_BASE);
}
#endif

} // namespace mcu {
