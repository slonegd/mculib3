#pragma once

#include "bits_adc_f0.h"

namespace mcu {

class ADC
{
   volatile ADC_bits::SR     ISR;     // interrupt and status register,        offset: 0x00
   volatile ADC_bits::IER    IER;     // interrupt enable register,            offset: 0x04
   volatile ADC_bits::CR     CR;      // control register,                     offset: 0x08
   volatile ADC_bits::CFGR   CFGR;    // configuration register 1,2            offset: 0x0C
   volatile ADC_bits::SMPR   SMPR;    // sampling time register,               offset: 0x14
   volatile uint32_t         RES1[2]; // Reserved,                                     0x18
   volatile ADC_bits::TR     TR;      // analog watchdog 1 threshold register, offset: 0x20
   volatile uint32_t         RES2;    // Reserved,                                     0x24
   volatile ADC_bits::CHSELR CHSELR;  // group regular sequencer register,     offset: 0x28
   volatile uint32_t         RES[5];  // Reserved,                                     0x2C
   volatile uint32_t         DR;      // group regular data register,          offset: 0x40
public:
   using CMSIS_type  = ADC_TypeDef;
   using DMA_mode    = ADC_bits::CFGR::DMA_mode;
   using Clock       = ADC_bits::CFGR::Clock;
   using Resolution  = ADC_bits::CFGR::Resolution;
   using Sample_time = ADC_bits::SMPR::Sample_time;

   ADC& enable()            { CR.ADEN  = true;    return *this; }
   ADC& disable()           { CR.ADDIS = true;    return *this; }
   bool is_enable()         { return CR.ADEN; }
   ADC& start()             { CR.ADSTART  = true; return *this;}
   ADC& dma_enable()        { CFGR.DMAEN  = true; return *this; }
   ADC& set (DMA_mode v)    { CFGR.DMACFG = v;    return *this; }
   ADC& set_continuous()    { CFGR.CONT   = true; return *this; }
   ADC& set (Clock v)       { CFGR.CKMODE = v;    return *this; }
   ADC& set (Resolution v)  { CFGR.RES    = v;    return *this; }
   ADC& set (Sample_time v) { SMPR.SMP    = v;    return *this; }
   bool is_ready()          { return ISR.ADRDY; }
   ADC& set_busy()          { ISR.ADRDY = true;   return *this; }
   ADC& stop()              { CR.ADSTP  = true;   return *this; }
   bool is_stoping()        { return CR.ADSTP; }
   volatile const uint32_t& data() { return DR; }

   // template<uint8_t> static void setChannel();
   // template <class PIN> static constexpr uint8_t channel();
   // template <class PIN> static constexpr bool PINenabled();
   // template <class DMA> static constexpr bool DMAenabled();

};

#if not defined(USE_MOCK_ADC)
SFINAE(ADC1,ADC) make_reference() {return *reinterpret_cast<ADC*>(ADC1_BASE);}
#endif

} // namespace mcu {