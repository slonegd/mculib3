#pragma once

#include "bits_adc_f0.h"
#include "pins.h"


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
	// template <class PIN> static constexpr bool PINenabled();
	// template <class DMA> static constexpr bool DMAenabled();
	template<Periph> static constexpr auto default_dma() { return Periph::DMA1_stream1; }
	template <class Pin> constexpr int set_channel(Periph);
};

#if not defined(USE_MOCK_ADC)
SFINAE(ADC1,ADC) make_reference() {return *reinterpret_cast<ADC*>(ADC1_BASE);}
#endif








template <class Pin> constexpr int ADC::set_channel(Periph p)
{
	 if (p == Periph::ADC1) {
		  if      constexpr (std::is_same_v<Pin,PA0>) { CHSELR.CHSEL0  = true; return 0;  }
		  else if constexpr (std::is_same_v<Pin,PA1>) { CHSELR.CHSEL1  = true; return 1;  }
		  else if constexpr (std::is_same_v<Pin,PA2>) { CHSELR.CHSEL2  = true; return 2;  }
		  else if constexpr (std::is_same_v<Pin,PA3>) { CHSELR.CHSEL3  = true; return 3;  }
		  else if constexpr (std::is_same_v<Pin,PA4>) { CHSELR.CHSEL4  = true; return 4;  }
		  else if constexpr (std::is_same_v<Pin,PA5>) { CHSELR.CHSEL5  = true; return 5;  }
		  else if constexpr (std::is_same_v<Pin,PA6>) { CHSELR.CHSEL6  = true; return 6;  }
		  else if constexpr (std::is_same_v<Pin,PA7>) { CHSELR.CHSEL7  = true; return 7;  }
		  else if constexpr (std::is_same_v<Pin,PB0>) { CHSELR.CHSEL8  = true; return 8;  }
		  else if constexpr (std::is_same_v<Pin,PB1>) { CHSELR.CHSEL9  = true; return 9;  }
		  else if constexpr (std::is_same_v<Pin,PC0>) { CHSELR.CHSEL10 = true;  return 10; }
		  else if constexpr (std::is_same_v<Pin,PC1>) { CHSELR.CHSEL11 = true;  return 11; }
		  else if constexpr (std::is_same_v<Pin,PC2>) { CHSELR.CHSEL12 = true;  return 12; }
		  else if constexpr (std::is_same_v<Pin,PC3>) { CHSELR.CHSEL13 = true;  return 13; }
		  else if constexpr (std::is_same_v<Pin,PC4>) { CHSELR.CHSEL14 = true;  return 14; }
		  else if constexpr (std::is_same_v<Pin,PC5>) { CHSELR.CHSEL15 = true;  return 15; }
		  else static_assert(always_false_v<Pin>, "Вывод не поддерживает работы с этим АЦП");
	 }

	 return 0;
}




} // namespace mcu {