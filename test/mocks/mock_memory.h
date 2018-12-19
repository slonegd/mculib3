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
   s == Sector::_0 ? std::fill (std::begin(memory<Sector::_0>), std::end(memory<Sector::_0>), 0xFF) : 
   s == Sector::_1 ? std::fill (std::begin(memory<Sector::_1>), std::end(memory<Sector::_1>), 0xFF) :  
   s == Sector::_2 ? std::fill (std::begin(memory<Sector::_2>), std::end(memory<Sector::_2>), 0xFF) : 
   s == Sector::_3 ? std::fill (std::begin(memory<Sector::_3>), std::end(memory<Sector::_3>), 0xFF) : 
   s == Sector::_4 ? std::fill (std::begin(memory<Sector::_4>), std::end(memory<Sector::_4>), 0xFF) : 
   s == Sector::_5 ? std::fill (std::begin(memory<Sector::_5>), std::end(memory<Sector::_5>), 0xFF) : 
   s == Sector::_6 ? std::fill (std::begin(memory<Sector::_6>), std::end(memory<Sector::_6>), 0xFF) : 
                     std::fill (std::begin(memory<Sector::_7>), std::end(memory<Sector::_7>), 0xFF);
}

} // namespace mock {