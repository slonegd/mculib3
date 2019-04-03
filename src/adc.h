#pragma once

#include "periph_adc.h"
#include "periph_dma.h"
#include "periph_rcc.h"
#include "pin.h"
#include "list.h"
#include "dynarray.h"
#include "function.h"
#include "interrupt.h"
#include <algorithm>

#if defined(USE_MOCK_ADC)
using ADC = mock::ADC;
#else
using ADC = mcu::ADC;
#endif

#if defined(USE_MOCK_DMA)
using DMA_stream = mock::DMA_stream;
#else
using DMA_stream = mcu::DMA_stream;
#endif

struct ADC_average;

struct ADC_channel : Listable<ADC_channel> {
    template<class Pin>
    static ADC_channel& make();
    operator const uint32_t() { return v; }
    uint32_t operator=  (uint32_t v) { return this->v = v;  }
    uint32_t operator+= (uint32_t v) { return this->v += v; }
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
    size_t buffer_address() const { return buffer.address(); }
    void   reset_channels() { clear(); buffer.clear(); }
private:
    Dyn_array<uint16_t> buffer{};  // сюда данные по дма
    const size_t        conversion_qty;
    size_t order{0};
    ADC&        adc;
    DMA_stream& dma;
    Interrupt& interrupt_;
    const mcu::Periph adc_periph;
    Function<void()> callback {};
    ADC_average (
         size_t       conversion_qty
        , ADC&        adc
        , DMA_stream& dma
        , Interrupt&  interrupt_
        , mcu::Periph adc_periph
        
    ) : conversion_qty {conversion_qty}
      , adc            {adc}
      , dma            {dma}
      , interrupt_     {interrupt_}
      , adc_periph     {adc_periph}
    {
        interrupt_.subscribe (this);
    } 
    ADC_average (const ADC_average&) = delete;
    void interrupt() override;
};
























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
        , ADC
    };
    REF(RCC).clock_enable<ADC>();
    res.adc.set (mcu::ADC::Resolution::_12_bit)
           .set_continuous()
           .dma_enable()
           .set (mcu::ADC::DMA_mode::circular); // no choice yet
#if defined (STM32F0)
    res.adc.set (mcu::ADC::Clock::PCLKdiv4)      // no choice yet
           .set (mcu::ADC::Sample_time::Default);// no choice yet
#elif defined (STM32F4)
    res.adc.set_scan_mode();
#endif
    REF(RCC).clock_enable<dma_periph>();
    auto periph_adr = size_t(&res.adc.data());
    res.dma.set_periph_adr (periph_adr)
           .set (mcu::DMA_stream::Direction::to_memory)
           .size_memory (mcu::DMA_stream::DataSize::word16)
           .size_periph (mcu::DMA_stream::DataSize::word16)
           .inc_memory()
           .enable_transfer_complete_interrupt();
#if defined (STM32F4)
    res.dma.circular_mode();
    res.dma.set_channel(DMA_stream::select_channel<ADC, dma_periph>());
#endif
    res.interrupt_.enable();
   
    return res;
}

template<class Pin>
ADC_channel& ADC_average::add_channel()
{
    auto& value = ADC_channel::make<Pin>();
    auto channel = adc.set_channel<Pin>(adc_periph);
    value.channel = channel;
#if defined (STM32F4)
    adc.set(channel, mcu::ADC::Sample_time::Default);
    adc.set_regular_sequence_order(++order, channel);
    adc.set_regular_sequence_length(order);
#endif
    // чтобы каналы располагались в листе в порядке, как они оцифровываются
    this->insert (
        std::find_if (
              this->begin()
            , this->end()
            , [=](auto& v){ return v.channel > channel; }
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
#if defined (STM32F0)
    // maybe need wrap in function
    if (adc.is_ready())
        adc.set_busy();
    adc.enable();
    while ( not adc.is_ready() ) { }
#elif defined (STM32F4)
    adc.enable();
    while ( not adc.is_enable() ) { }
#endif

    adc.start();
}


void ADC_average::interrupt()
{
    for (auto& channel : *this)
        channel = 0;
    auto it_channel = begin();
    for (auto v : buffer) {
        *it_channel += v;
        if (++it_channel == end())
            it_channel = begin();
    }

    if (callback) callback();
}