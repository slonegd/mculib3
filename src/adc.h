#pragma once

#include "periph_adc.h"
#include "pin.h"
#include "meta.h"
#include <tuple>



struct ADC_value {
    template<class Pin>
    static auto& make()
    {
        static ADC_value v;
        return v;
    }
    operator const uint32_t() { return v; }
private:
    uint32_t v;
};

template<size_t n>
class ADC_average
{
    using Values = meta::tuple_generate_t<ADC_value&,n>;
public:
    Values& values;
    ADC_average (Values values) : values{values} {}
};

template<mcu::Periph ADC, class ... Pins>
auto& make_adc_average()
{
    static ADC_average<sizeof...(Pins)> adc (std::tie(ADC_value::make<Pins>()...));
    return adc.values;
}



namespace example {

void ADC_average()
{
    auto& [   power, temperature] = make_adc_average <mcu::Periph::ADC1,
           mcu::PA1,    mcu::PA2
    >();

    while (1) {
        if (power != 0 and temperature < 50)
            __WFI();
    }
}

} // namespace example {