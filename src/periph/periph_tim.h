#pragma once

#include "periph.h"
#include "pins.h"

#if   defined(STM32F0) or defined(STM32F4)
   #include "tim_f0_f4.h"
#endif