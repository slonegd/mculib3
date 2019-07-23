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
    Flash<Data, mcu::FLASH::Sector::_7, mcu::FLASH::Sector::_6> flash {};

    struct OtherData {
        bool   d1 {true};
        size_t d2 {0};
    };
    Flash<OtherData, mcu::FLASH::Sector::_5, mcu::FLASH::Sector::_4> other_flash {};

    auto ticker       = Timer{200};
    auto other_ticker = Timer{300};

    while (1) {
        flash.d2 += ticker.event() and flash.d1;
        other_flash.d2 += other_ticker.event() and other_flash.d1;
    }
}

} // namespace mcu::example {