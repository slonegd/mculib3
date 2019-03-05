#pragma once

#include "periph_adc.h"
#include "periph_dma.h"
#include "pin.h"
#include "list.h"
#include "dynarray.h"
#include "function.h"
#include "interrupt.h"
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

struct ADC_average : private List<ADC_channel>, Interrupting {
    template<mcu::Periph>
    static ADC_average& make (size_t conversion_qty);
    template<class Pin>
    ADC_channel& add_channel();
    template<class Function>
    void set_callback (const Function& v) { callback = v; }
    void start();
private:
    Dyn_array<uint16_t> buffer{};  // сюда данные по дма
    const size_t        conversion_qty;
    ADC&        adc;
    DMA_stream& dma;
    Interrupt& interrupt_;
    Function<void()> callback {};
    ADC_average (
         size_t       conversion_qty
        , ADC&        adc
        , DMA_stream& dma
        , Interrupt&  interrupt_
        
    ) : conversion_qty {conversion_qty}
      , adc            {adc}
      , dma            {dma}
      , interrupt_     {interrupt_}
    {
        interrupt_.subscribe (this);
    } 
    ADC_average (const ADC_average&) = delete;
    void interrupt() override {} // placeholder
};



namespace example {

void ADC_average() {
    constexpr auto conversion_on_channel {16};
    auto& adc         = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
    auto& power       = adc.add_channel<PA0>();
    auto& temperature = adc.add_channel<PA1>();
    auto& alarm       = Pin::make<PB0,PinMode::Output>();
    adc.set_callback ([&]{
        alarm = power < 100 or temperature > 50;
    });
    adc.start();
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
ADC_average& ADC_average::make (size_t conversion_qty)
{
    auto constexpr dma_periph = mcu::ADC::default_dma<ADC>();
    auto& interrupt = get_interrupt<dma_periph>();
    static auto res = ADC_average {
          conversion_qty
        , mcu::make_reference<ADC>()
        , mcu::make_reference<dma_periph>()
        , interrupt
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
           .inc_memory()
           .enable_transfer_complete_interrupt();

    NVIC_EnableIRQ (res.interrupt_.IRQn());
   
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
    buffer.add_size(conversion_qty);
    this->dma.disable()
             .set_memory_adr (buffer.address())
             .set_qty_transactions (buffer.size())
             .enable();
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