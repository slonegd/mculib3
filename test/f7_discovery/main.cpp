#define STM32F746xx
#define F_CPU   216'000'000UL

#include "init_clock.h"
#include "timers.h"
#include "pin.h"
#include "periph_usb.h"

/// эта функция вызываеться первой в startup файле
extern "C" void init_clock() { init_clock<25_MHz,F_CPU>(); }


int main()
{
   // usb block start
   volatile decltype(auto) usb_core   = REF(USB_FS_core);
   volatile decltype(auto) usb_device = REF(USB_FS_device);
   volatile decltype(auto) usb_in     = REF(USB_FS_IN);
   volatile decltype(auto) usb_out    = REF(USB_FS_OUT);
   REF(RCC).clock_enable<Periph::USB_FS_core>();

   // settings
   auto vbus_sensing_enable {false};
   auto dev_endpoints {6};

   // inits
   // USB_CoreInit пока пропустил
   // analog USB_DevInit
   usb_core.clear_fifo_size();    // пока не знаю зачем
   usb_core.enable_vbus_detect(); // тоже нафиг
   if (not vbus_sensing_enable) {
      usb_core.enable_vbus_detect (false);
      usb_core.enable_session_B_periph_override();
      usb_core.set_B_periph_vbusvalid();
   }
   USB_power_clock::ref(usb_core).restart_phy_clock();
   usb_device.set (USB_device::Frame_interval::_80_percent); // и так по умолчанию
   usb_device.set (USB_device::Speed::high); // in hal choice
   usb_core.flush (USB_core::TX_FIFO::all);
   usb_core.flush_rx_fifo();
   usb_device.clear_interrupt_flags();
   auto init_endpoint = [](auto& ep, auto i) {
      if (ep[i].is_enable()) {
         if (i != 0)
            ep[i].disable();
         ep[i].set_NAK();
      } else {
         ep[i].reset_control();
      }
      ep[i].clear_transfer_size();
      ep[i].like_CMSIS().DIEPINT = 0xFB7FU; // какой то бред, пока оставлю, потом попробовать без этого
   };
   for (auto i {0}; i < dev_endpoints; ++i) {
      init_endpoint (usb_in , i);
      init_endpoint (usb_out, i);
   }
   // usb block end
   



   decltype(auto) led = Pin::make<mcu::PI1, mcu::PinMode::Output>();
   Timer timer {200_ms};

   while (1) {
      led ^= timer.event();
   } // while(1) {

}