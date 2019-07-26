#pragma once
#include "safe_flash.h"
#include "timers.h"

namespace mcu::example {

void safe_flash() {
    struct Data {
        bool   d1 {true};
        size_t d2 {0};
        // more other fields, Data must be trivially copyable
    };
    Safe_flash<Data, mcu::FLASH::Sector::_23, mcu::FLASH::Sector::_28> flash {};

    auto ticker = Timer{1000};

    while (1) {
        flash.d2 += ticker.event() and flash.d1;
    }
}

} // namespace mcu::example {