#pragma once

#include "periph.h"


#if defined(STM32F0)
   #error usb for STM32F0 not realized
#elif defined(STM32F1)
   #error usb for STM32F1 not realized
#elif defined(STM32F4)
   #error usb for STM32F4 not realized
#elif defined(STM32F7)
   #include "usb_core_f7.h"
   #include "usb_device_f7.h"
   #include "usb_endpoint_f7.h"
#endif
