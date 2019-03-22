#pragma once
#include "adc.h"

namespace mcu::example {

void adc_average() {
    constexpr auto conversion_on_channel {16};
    struct {
        ADC_average& control     = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
        ADC_channel& power       = control.add_channel<mcu::PA0>();
        ADC_channel& temperature = control.add_channel<mcu::PA1>();
    } adc{};

    auto& alarm       = Pin::make<mcu::PB0,mcu::PinMode::Output>();

    adc.control.set_callback ([&]{
        alarm = adc.power < 100 or adc.temperature > 50;
    });
    adc.control.start();
    while (1) {}
}

} // namespace mcu::example {