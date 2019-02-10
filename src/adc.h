#pragma once

#include "periph_adc.h"
#include "pin.h"
#include "list.h"
#include "heap.h"
#include <tuple>


struct ADC_value : Listable<ADC_value> {
    template<class Pin>
    static ADC_value& make();
    operator const uint32_t() { return v; }
private:
    uint32_t v;
    ADC_value() = default;
    ADC_value (const ADC_value&) = delete;
};

struct ADC_average : private List<ADC_value> {
    static ADC_average& make();
    template<class Pin>
    ADC_value& add_value();
    void start() {} // placeholder
private:
    uint16_t* pbuffer   {nullptr}; // сюда данные по дма
    size_t    size      {0};
    size_t    value_qty {0};       // вроде не нужен
    ADC_average() = default;
    ADC_average (const ADC_average&) = delete;
};



namespace example {

void ADC_average() {
    auto& adc         = ADC_average::make();
    auto& power       = adc.add_value<PA0>();
    auto& temperature = adc.add_value<PA1>();
    adc.start();
    if (power < 100 or temperature < 50)
        __WFI();
}

} // namespace example {




















template<class Pin>
ADC_value& ADC_value::make()
{
    static ADC_value v;
    // TODO init
    return v;
}


ADC_average& ADC_average::make()
{
    static ADC_average adc;
    // TODO init
    return adc;
}


template<class Pin>
ADC_value& ADC_average::add_value()
{
    auto& value = ADC_value::make<Pin>();
    this->push_back(&value);
    if (not pbuffer)
        delete[] pbuffer;
    pbuffer = new uint16_t[size += 16]; // placeholder
    value_qty++;
    return value;
}