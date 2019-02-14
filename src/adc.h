#pragma once

#include "periph_adc.h"
#include "periph_dma.h"
#include "pin.h"
#include "list.h"
#include "heap.h"
#include <algorithm>

struct ADC_average;

struct ADC_channel : Listable<ADC_channel> {
    template<class Pin>
    static ADC_channel& make();
    operator const uint32_t() { return v; }
    friend ADC_average;
private:
    uint32_t v;
    Pin& pin;
    int channel {-1};
    ADC_channel (Pin& pin) : pin{pin} {}
    ADC_channel (const ADC_channel&) = delete;
};

struct ADC_average : private List<ADC_channel> {
    template<mcu::Periph>
    static ADC_average& make();
    template<class Pin>
    ADC_channel& add_channel();
    void start();
private:
    uint16_t* pbuffer   {nullptr}; // сюда данные по дма
    size_t    size      {0};
    size_t    value_qty {0};       // вроде не нужен
    ADC&        adc;
    DMA_stream& dma;
    ADC_average (
          ADC&        adc
        , DMA_stream& dma
    ) : adc {adc}
      , dma {dma} 
    {}
    ADC_average (const ADC_average&) = delete;
};



namespace example {

void ADC_average() {
    auto& adc         = ADC_average::make<mcu::Periph::ADC1>();
    auto& power       = adc.add_channel<PA0>();
    auto& temperature = adc.add_channel<PA1>();
    adc.start();
    if (power < 100 or temperature < 50)
        __WFI();
}

} // namespace example {




















template<class Pin_>
ADC_channel& ADC_channel::make()
{
    static auto res = ADC_channel {Pin::make<Pin_,mcu::PinMode::Analog>()};
    return res;
}


template<mcu::Periph ADC>
ADC_average& ADC_average::make()
{
    auto constexpr dma_periph = mcu::ADC::default_dma<ADC>();
    static auto res = ADC_average {
          mcu::make_reference<ADC>()
        , mcu::make_reference<dma_periph>()
    };
    REF(RCC).clock_enable<ADC>();
    res.adc.set (mcu::ADC::Clock::PCLKdiv4)     // no choice yet
           .set (mcu::ADC::Resolution::_12_bit) // no choice yet
           .set (mcu::ADC::Sample_time::Default)// no choice yet
           .set_continuous()
           .dma_enable()
           .set (mcu::ADC::DMA_mode::circular);

    REF(RCC).clock_enable<dma_periph>();
    auto periph_adr = size_t(&res.adc.data());
    res.dma.set_periph_adr (periph_adr)
           .set (mcu::DMA_stream::Direction::to_memory)
           .size_memory (mcu::DMA_stream::DataSize::word16)
           .size_periph (mcu::DMA_stream::DataSize::word16)
           .inc_memory();
    // TODO add interrupt
   
    return res;
}


template<class Pin>
ADC_channel& ADC_average::add_channel()
{
    auto& value = ADC_channel::make<Pin>();
    // определить канал по пину
    constexpr auto channel = 1; // placeholder
    // включить нужный канал
    // чтобы каналы располагались в листе в порядке, как они оцифровываются
    this->insert (
        std::find_if (
              this->begin()
            , this->end()
            , [channel](auto& v){ return v.channel > channel; }
        )
        , value
    );
    if (not pbuffer)
        delete[] pbuffer;
    pbuffer = new uint16_t[size += 16]; // placeholder
    this->dma.set_memory_adr (size_t(pbuffer))
             .set_qty_transactions (size);
    value_qty++; 
    return value;
}


void ADC_average::start()
{
    dma.enable();
    
    // maybe need wrap in function
    if (adc.is_ready())
      adc.set_busy();
    adc.enable();
    while ( not adc.is_ready() ) { }

    adc.start();
}