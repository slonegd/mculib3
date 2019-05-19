#pragma once
#include "flash.h"
#include "timers.h"

namespace mcu::example {

void flash() {
    struct Data {
        bool   d1 {true};
        size_t d2 {0};
        // more other fields, Data must be trivially copyable
    };
    Flash<Data, mcu::FLASH::Sector::_7> flash {};

    auto ticker = Timer{200};

    while (1) {
        flash.d2 += ticker.event() and flash.d2;
    }
}

} // namespace mcu::example {