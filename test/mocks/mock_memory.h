#pragma once

#include "periph_flash.h"
#include <array>
#include <algorithm>

namespace mock {

using Sector = mcu::FLASH::Sector;

template<Sector s>
auto constexpr sector_size = mcu::FLASH::size<s>();

template<Sector s>
std::array<uint8_t, sector_size<s>> memory;

template<Sector s>
size_t address = reinterpret_cast<size_t>(&memory<s>);

bool start_erase {false};
void erase (Sector s)
{
   start_erase = true;
   auto void_lambda = [](){};

   #define HELPER(sector) s == sector ? std::fill (std::begin(memory<sector>), std::end(memory<sector>), 0xFF) :
   
   HELPER (Sector::_0) HELPER (Sector::_1) HELPER (Sector::_2) HELPER (Sector::_3) HELPER (Sector::_4)
   HELPER (Sector::_5) HELPER (Sector::_6) HELPER (Sector::_7)
   #if defined(STM32F4) or defined(STM32F0)
   HELPER (Sector::_8) HELPER (Sector::_9) HELPER (Sector::_10) HELPER (Sector::_11)
   #endif
   #if defined(STM32F0)
   HELPER (Sector::_12) HELPER (Sector::_13) HELPER (Sector::_14)
   HELPER (Sector::_15) HELPER (Sector::_16) HELPER (Sector::_17) HELPER (Sector::_18) HELPER (Sector::_19)
   HELPER (Sector::_20) HELPER (Sector::_21) HELPER (Sector::_22) HELPER (Sector::_23) HELPER (Sector::_24)
   HELPER (Sector::_25) HELPER (Sector::_26) HELPER (Sector::_27) HELPER (Sector::_28) HELPER (Sector::_29)
   HELPER (Sector::_30) HELPER (Sector::_31)
   #endif
   void_lambda();

   #undef HELPER

}

} // namespace mock {