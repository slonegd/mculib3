#pragma once

#include "bits_rcc_f4.h"

namespace mcu {

class RCC {
	__IO RCC_bits::CR      CR;            // RCC clock control register,                                  offset: 0x00
	__IO RCC_bits::PLLCFGR PLLCFGR;       // RCC PLL configuration register,                              offset: 0x04
	__IO RCC_bits::CFGR    CFGR;          // RCC clock configuration register,                            offset: 0x08
	__IO uint32_t          CIR;           // RCC clock interrupt register,                                offset: 0x0C
	__IO uint32_t          AHB1RSTR;      // RCC AHB1 peripheral reset register,                          offset: 0x10
	__IO uint32_t          AHB2RSTR;      // RCC AHB2 peripheral reset register,                          offset: 0x14
	__IO uint32_t          AHB3RSTR;      // RCC AHB3 peripheral reset register,                          offset: 0x18
	__IO uint32_t          RESERVED0;     // Reserved, 0x1C                                                           
	__IO uint32_t          APB1RSTR;      // RCC APB1 peripheral reset register,                          offset: 0x20
	__IO uint32_t          APB2RSTR;      // RCC APB2 peripheral reset register,                          offset: 0x24
	__IO uint32_t          RESERVED1[2];  // Reserved, 0x28-0x2C                                                      
	__IO RCC_bits::AHB1ENR AHB1ENR;       // RCC AHB1 peripheral clock register,                          offset: 0x30
	__IO uint32_t          AHB2ENR;       // RCC AHB2 peripheral clock register,                          offset: 0x34
	__IO uint32_t          AHB3ENR;       // RCC AHB3 peripheral clock register,                          offset: 0x38
	__IO uint32_t          RESERVED2;     // Reserved, 0x3C                                                           
	__IO RCC_bits::APB1ENR APB1ENR;       // RCC APB1 peripheral clock enable register,                   offset: 0x40
	__IO RCC_bits::APB2ENR APB2ENR;       // RCC APB2 peripheral clock enable register,                   offset: 0x44
	__IO uint32_t          RESERVED3[2];  // Reserved, 0x48-0x4C                                                      
	__IO uint32_t          AHB1LPENR;     // RCC AHB1 peripheral clock enable in low power mode register, offset: 0x50
	__IO uint32_t          AHB2LPENR;     // RCC AHB2 peripheral clock enable in low power mode register, offset: 0x54
	__IO uint32_t          AHB3LPENR;     // RCC AHB3 peripheral clock enable in low power mode register, offset: 0x58
	__IO uint32_t          RESERVED4;     // Reserved, 0x5C                                                           
	__IO uint32_t          APB1LPENR;     // RCC APB1 peripheral clock enable in low power mode register, offset: 0x60
	__IO uint32_t          APB2LPENR;     // RCC APB2 peripheral clock enable in low power mode register, offset: 0x64
	__IO uint32_t          RESERVED5[2];  // Reserved, 0x68-0x6C                                                      
	__IO uint32_t          BDCR;          // RCC Backup domain control register,                          offset: 0x70
	__IO uint32_t          CSR;           // RCC clock control & status register,                         offset: 0x74
	__IO uint32_t          RESERVED6[2];  // Reserved, 0x78-0x7C                                                      
	__IO uint32_t          SSCGR;         // RCC spread spectrum clock generation register,               offset: 0x80
	__IO uint32_t          PLLI2SCFGR;    // RCC PLLI2S configuration register,                           offset: 0x84

public:
	using CMSIS_type   = RCC_TypeDef;
	using AHBprescaler = RCC_bits::   CFGR::AHBprescaler;
	using APBprescaler = RCC_bits::   CFGR::APBprescaler;
	using SystemClock  = RCC_bits::   CFGR::SystemClock;
	using PLLPdiv      = RCC_bits::PLLCFGR::PLLPdiv;
	using PLLsource    = RCC_bits::PLLCFGR::PLLsource;

	auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

	RCC& set       (AHBprescaler v) { CFGR.HPRE      = v; return *this; }
	RCC& set_APB1  (APBprescaler v) { CFGR.PPRE1     = v; return *this; }
	RCC& set_APB2  (APBprescaler v) { CFGR.PPRE2     = v; return *this; }
	RCC& set       (SystemClock  v) { CFGR.SW        = v; return *this; }
	RCC& set       (PLLPdiv      v) { PLLCFGR.PLLP   = v; return *this; }
	RCC& set       (PLLsource    v) { PLLCFGR.PLLSRC = v; return *this; }

	size_t get_APB1_clock(){return get_APB_clock (CFGR.PPRE1);}
	size_t get_APB2_clock(){return get_APB_clock (CFGR.PPRE2);}
	
	template<int v> RCC& set_PLLM() { static_assert (v >= 2  and v <= 63);
												 PLLCFGR.PLLM   = v; return *this; }
	template<int v> RCC& set_PLLN() { static_assert (v >= 50 and v <= 432);
												 PLLCFGR.PLLN   = v; return *this; }
	template<int v> RCC& set_PLLQ() { static_assert (v >= 2  and v <= 15);
												 PLLCFGR.PLLQ = v;   return *this; }

	RCC& on_HSE        () { CR.HSEON = true;         return *this; }
	RCC& wait_HSE_ready() { while (not CR.HSERDY) {} return *this; }
	RCC& on_PLL        () { CR.PLLON = true;         return *this; }
	RCC& wait_PLL_ready() { while (not CR.PLLRDY) {} return *this; }

	template<Periph p> void clock_enable()
	{
		if      constexpr (p == Periph::GPIOA) AHB1ENR.GPIOAEN = true;
		else if constexpr (p == Periph::GPIOB) AHB1ENR.GPIOBEN = true;
		else if constexpr (p == Periph::GPIOC) AHB1ENR.GPIOCEN = true;
		else if constexpr (p == Periph::GPIOD) AHB1ENR.GPIODEN = true;
		else if constexpr (p == Periph::GPIOE) AHB1ENR.GPIOEEN = true;
		else if constexpr (p == Periph::GPIOF) AHB1ENR.GPIOFEN = true;
		else if constexpr (p == Periph::GPIOG) AHB1ENR.GPIOGEN = true;
		else if constexpr (p == Periph::GPIOH) AHB1ENR.GPIOHEN = true;
		else if constexpr (p == Periph::GPIOI) AHB1ENR.GPIOIEN = true;

		else if constexpr (p == Periph::USART1) APB2ENR.USART1EN = true;
		else if constexpr (p == Periph::USART2) APB1ENR.USART2EN = true;
		else if constexpr (p == Periph::USART3) APB1ENR.USART3EN = true;
		else if constexpr (p == Periph::USART4) APB1ENR.UART4EN  = true;
		else if constexpr (p == Periph::USART5) APB1ENR.UART5EN  = true;
		else if constexpr (p == Periph::USART6) APB2ENR.USART6EN = true;

		else if constexpr (p == Periph::DMA1) AHB1ENR.DMA1EN         = true;
		else if constexpr (p == Periph::DMA2) AHB1ENR.DMA2EN         = true;
		else if constexpr (p == Periph::DMA1_stream0) AHB1ENR.DMA1EN = true;
		else if constexpr (p == Periph::DMA1_stream1) AHB1ENR.DMA1EN = true;
		else if constexpr (p == Periph::DMA1_stream2) AHB1ENR.DMA1EN = true;
		else if constexpr (p == Periph::DMA1_stream3) AHB1ENR.DMA1EN = true;
		else if constexpr (p == Periph::DMA1_stream4) AHB1ENR.DMA1EN = true;
		else if constexpr (p == Periph::DMA1_stream5) AHB1ENR.DMA1EN = true;
		else if constexpr (p == Periph::DMA1_stream6) AHB1ENR.DMA1EN = true;
		else if constexpr (p == Periph::DMA1_stream7) AHB1ENR.DMA1EN = true;
		else if constexpr (p == Periph::DMA2_stream0) AHB1ENR.DMA2EN = true;
		else if constexpr (p == Periph::DMA2_stream1) AHB1ENR.DMA2EN = true;
		else if constexpr (p == Periph::DMA2_stream2) AHB1ENR.DMA2EN = true;
		else if constexpr (p == Periph::DMA2_stream3) AHB1ENR.DMA2EN = true;
		else if constexpr (p == Periph::DMA2_stream4) AHB1ENR.DMA2EN = true;
		else if constexpr (p == Periph::DMA2_stream5) AHB1ENR.DMA2EN = true;
		else if constexpr (p == Periph::DMA2_stream6) AHB1ENR.DMA2EN = true;
		else if constexpr (p == Periph::DMA2_stream7) AHB1ENR.DMA2EN = true;

		else if constexpr (p == Periph::TIM2) APB1ENR.TIM2EN  = true;
		else if constexpr (p == Periph::TIM3) APB1ENR.TIM3EN  = true;

		else if constexpr (p == Periph::ADC1) APB2ENR.ADC1EN  = true;
		else if constexpr (p == Periph::ADC2) APB2ENR.ADC2EN  = true;
		else if constexpr (p == Periph::ADC3) APB2ENR.ADC3EN  = true;

        else static_assert ( always_false_v<decltype(p)>, "допиши clock_enable");
	}

	auto clock (Periph p)
	{
		if (p == Periph::USART1 or p == Periph::USART6) 
			return get_APB2_clock();
		else if (p == Periph::USART2 or p == Periph::USART3 or p == Periph::USART4 
				or p == Periph::USART5)
			return get_APB1_clock();
		return size_t(0);
	}

	
private:
	size_t get_APB_clock (APBprescaler v)
	{
		return v == APBprescaler::APBnotdiv ? F_CPU     :
			   v == APBprescaler::APBdiv2   ? F_CPU / 2 :
			   v == APBprescaler::APBdiv4   ? F_CPU / 4 :
			   v == APBprescaler::APBdiv8   ? F_CPU / 8 : F_CPU / 16;
	}
};

#if not defined(USE_MOCK_RCC)
template<Periph p> std::enable_if_t<p == Periph::RCC, RCC&> make_reference() { return *reinterpret_cast<RCC*>(RCC_BASE); }
#endif
} // namespace mcu {