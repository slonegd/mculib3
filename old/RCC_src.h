#pragma once

#include <type_traits>
#include "DMA.h"
#include "GPIO.h"
#include "adc_periph.h"
#include "USART.h"
#include "SPI.h"

#if defined(STM32F405xx) or defined(STM32F030x6)

uint32_t RCC::getAPBclock(APBprescaler val)
{
   return val == RCC_ral::APBprescaler::APBnotdiv ? fCPU     :
          val == RCC_ral::APBprescaler::APBdiv2   ? fCPU / 2 :
          val == RCC_ral::APBprescaler::APBdiv4   ? fCPU / 4 :
          val == RCC_ral::APBprescaler::APBdiv8   ? fCPU / 8 :
                                                    fCPU / 16;
}


void RCC::setAHBprescaler (AHBprescaler val)
{
   uint32_t tmp = conf().reg;
   tmp &= ~_3BIT_TO_MASK(conf(), HPRE);
   tmp |= VAL_TO_MASK(conf(), HPRE, val);
   conf().reg = tmp;
}


void RCC::systemClockSwitch (SystemClockSwitch val)
{
   uint32_t tmp = conf().reg;
   tmp &= ~_2BIT_TO_MASK(conf(), SW);
   tmp |= VAL_TO_MASK(conf(), SW, val);
   conf().reg = tmp;
}


void RCC::waitPLLready()
{
   while (IS_CLEAR(clockContr(), PLLRDY)) { }
}


void RCC::PLLon()
{
   SET(clockContr(), PLLON);
}


void RCC::waitHSEready()
{
   while (IS_CLEAR(clockContr(), HSERDY)) { }
}


void RCC::HSEon()
{
   SET(clockContr(), HSEON);
}

///////////////////////////////////////////////////////////////////////////////
/// clockEnable
///////////////////////////////////////////////////////////////////////////////
template<> void RCC::clockEnable<USART1>()
{
   SET (APB2en(), USART1EN);
   while (IS_CLEAR (APB2en(), USART1EN)) { }
}
template<> void RCC::clockEnable<USART1alt>()
{
   SET (APB2en(), USART1EN);
   while (IS_CLEAR (APB2en(), USART1EN)) { }
}
template<> void RCC::clockEnable<ADC1>()
{
   SET (APB2en(), ADC1EN);
   while (IS_CLEAR (APB2en(), ADC1EN)) { }
}


#endif


#if defined(STM32F405xx)

void RCC::setPLLsource (PLLsource val)
{
   BIT_BAND(pllConf(), PLLSRC) = val;
}


uint32_t RCC::getAPB2clock()
{
   auto tmp = static_cast<APBprescaler>(_3BIT_READ(conf(), PPRE2));
   return getAPBclock (tmp);
}


void RCC::setAPB1prescaler (APBprescaler val)
{
   uint32_t tmp = conf().reg;
   tmp &= ~_3BIT_TO_MASK(conf(), PPRE1);
   tmp |= VAL_TO_MASK(conf(), PPRE1, val);
   conf().reg = tmp;
}


void RCC::setAPB2prescaler (APBprescaler val)
{
   uint32_t tmp = conf().reg;
   tmp &= ~_3BIT_TO_MASK(conf(), PPRE2);
   tmp |= VAL_TO_MASK(conf(), PPRE2, val);
   conf().reg = tmp;
}


uint32_t RCC::getAPB1clock()
{
   auto tmp = static_cast<APBprescaler>(_3BIT_READ(conf(), PPRE1));
   return getAPBclock (tmp);
}


void RCC::setPLLP (PLLPdiv val)
{
   uint32_t tmp = pllConf().reg;
   tmp &= ~_2BIT_TO_MASK(pllConf(), PLLP);
   tmp |= VAL_TO_MASK(pllConf(), PLLP, val);
   pllConf().reg = tmp;
}


template <uint8_t val>
void RCC::setPLLM()
{
   static_assert (
      val >= 2 && val <= 63,
      "значение множителя должно быть в диапазоне 2-63"
   );
   uint32_t tmp = pllConf().reg;
   tmp &= ~_6BIT_TO_MASK(pllConf(), PLLM);
   tmp |= VAL_TO_MASK(pllConf(), PLLM, val);
   pllConf().reg = tmp;
}


template <uint16_t val>
void RCC::setPLLN()
{
   static_assert (
      val >= 50 && val <= 432,
      "значение множителя должно быть в диапазоне 50-432"
   );
   uint32_t tmp = pllConf().reg;
   tmp &= ~_9BIT_TO_MASK(pllConf(), PLLN);
   tmp |= VAL_TO_MASK(pllConf(), PLLN, val);
   pllConf().reg = tmp;
}


template <uint8_t val>
void RCC::setPLLQ()
{
   static_assert (
      val >= 2 && val <= 15,
      "значение множителя должно быть в диапазоне 2-15"
   );
   uint32_t tmp = pllConf().reg;
   tmp &= ~_4BIT_TO_MASK(pllConf(), PLLQ);
   tmp |= VAL_TO_MASK(pllConf(), PLLQ, val);
   pllConf().reg = tmp;
}

///////////////////////////////////////////////////////////////////////////////
/// clockEnable
///////////////////////////////////////////////////////////////////////////////
template<> void RCC::clockEnable<DMA1>()
{
   SET (AHB1en(), DMA1EN);
   while (IS_CLEAR (AHB1en(), DMA1EN)) { }
}
template<> void RCC::clockEnable<DMA2>()
{
   SET (AHB1en(), DMA2EN);
   while (IS_CLEAR (AHB1en(), DMA2EN)) { }
}
template<> void RCC::clockEnable<PA>()
{
   SET (AHB1en(), GPIOAEN);
   while (IS_CLEAR (AHB1en(), GPIOAEN)) { }
}
template<> void RCC::clockEnable<PB>()
{
   SET (AHB1en(), GPIOBEN);
   while (IS_CLEAR (AHB1en(), GPIOBEN)) { }
}
template<> void RCC::clockEnable<PC>()
{
   SET (AHB1en(), GPIOCEN);
   while (IS_CLEAR (AHB1en(), GPIOCEN)) { }
}
template<> void RCC::clockEnable<PD>()
{
   SET (AHB1en(), GPIODEN);
   while (IS_CLEAR (AHB1en(), GPIODEN)) { }
}
template<> void RCC::clockEnable<PF>()
{
   SET (AHB1en(), GPIOFEN);
   while (IS_CLEAR (AHB1en(), GPIOFEN)) { }
}
template<> void RCC::clockEnable<PE>()
{
   SET (AHB1en(), GPIOEEN);
   while (IS_CLEAR (AHB1en(), GPIOEEN)) { }
}
template<> void RCC::clockEnable<PG>()
{
   SET (AHB1en(), GPIOGEN);
   while (IS_CLEAR (AHB1en(), GPIOGEN)) { }
}
template<> void RCC::clockEnable<PH>()
{
   SET (AHB1en(), GPIOHEN);
   while (IS_CLEAR (AHB1en(), GPIOHEN)) { }
}
template<> void RCC::clockEnable<PI>()
{
   SET (AHB1en(), GPIOIEN);
   while (IS_CLEAR (AHB1en(), GPIOIEN)) { }
}
template<> void RCC::clockEnable<ADC2>()
{
   SET (APB2en(), ADC2EN);
   while (IS_CLEAR (APB2en(), ADC2EN)) { }
}
template<> void RCC::clockEnable<ADC3>()
{
   SET (APB2en(), ADC3EN);
   while (IS_CLEAR (APB2en(), ADC3EN)) { }
}
// template<> void RCC::clockEnable<SPI2>()
// {
//    SET (APB1en(), SPI2EN);
//    while (IS_CLEAR (APB1en(), SPI2EN)) { }
// }
// template<> void RCC::clockEnable<SPI3>()
// {
//    SET (APB1en(), SPI3EN);
//    while (IS_CLEAR (APB1en(), SPI3EN)) { }
// }
template<> void RCC::clockEnable<USART2>()
{
   SET (APB1en(), USART2EN);
   while (IS_CLEAR (APB1en(), USART2EN)) { }
}
template<> void RCC::clockEnable<USART3>()
{
   SET (APB1en(), USART3EN);
   while (IS_CLEAR (APB1en(), USART3EN)) { }
}
template<> void RCC::clockEnable<USART3alt>()
{
   SET (APB1en(), USART3EN);
   while (IS_CLEAR (APB1en(), USART3EN)) { }
}
template<> void RCC::clockEnable<USART6>()
{
   SET (APB2en(), USART6EN);
   while (IS_CLEAR (APB2en(), USART6EN)) { }
}
template<> void RCC::clockEnable<USART6alt17>()
{
   SET (APB2en(), USART6EN);
   while (IS_CLEAR (APB2en(), USART6EN)) { }
}
template<> void RCC::clockEnable<USART6alt27>()
{
   SET (APB2en(), USART6EN);
   while (IS_CLEAR (APB2en(), USART6EN)) { }
}
template<> void RCC::clockEnable<USART6alt26>()
{
   SET (APB2en(), USART6EN);
   while (IS_CLEAR (APB2en(), USART6EN)) { }
}






#elif defined(STM32F030x6)

void RCC::setPLLsource (PLLsource val)
{
   if (val)
      SET(conf(), PLLSRC);
   else
      CLEAR(conf(), PLLSRC);
}


uint32_t RCC::getAPB2clock()
{
   auto tmp = static_cast<APBprescaler>(_3BIT_READ(conf(), PPRE));
   return getAPBclock (tmp);
}


void RCC::setAPBprecsaler (APBprescaler val)
{
   uint32_t tmp = conf().reg;
   tmp &= ~_3BIT_TO_MASK(conf(), PPRE);
   tmp |= VAL_TO_MASK(conf(), PPRE, val);
   conf().reg = tmp;
}


void RCC::setPLLmultiplier (PLLmultiplier val)
{
   uint32_t tmp = conf().reg;
   tmp &= ~_4BIT_TO_MASK(conf(), PLLMUL);
   tmp |= VAL_TO_MASK(conf(), PLLMUL, val);
   conf().reg = tmp;
}

///////////////////////////////////////////////////////////////////////////////
/// clockEnable
///////////////////////////////////////////////////////////////////////////////
template<> void RCC::clockEnable<SPI1>()
{
   SET (APB2en(), SPI1EN);
   while (IS_CLEAR (APB2en(), SPI1EN)) { }
}
template<> void RCC::clockEnable<PA>()
{
   SET (AHBen(), IOPAEN);
   while (IS_CLEAR (AHBen(), IOPAEN)) { }
}
template<> void RCC::clockEnable<PB>()
{
   SET (AHBen(), IOPBEN);
   while (IS_CLEAR (AHBen(), IOPBEN)) { }
}
template<> void RCC::clockEnable<PC>()
{
   SET (AHBen(), IOPCEN);
   while (IS_CLEAR (AHBen(), IOPCEN)) { }
}
template<> void RCC::clockEnable<PD>()
{
   SET (AHBen(), IOPDEN);
   while (IS_CLEAR (AHBen(), IOPDEN)) { }
}
template<> void RCC::clockEnable<PF>()
{
   SET (AHBen(), IOPFEN);
   while (IS_CLEAR (AHBen(), IOPFEN)) { }
}
template<> void RCC::clockEnable<DMA1>()
{
   SET (AHBen(), DMAEN);
   while (IS_CLEAR (AHBen(), DMAEN)) { }
}


#endif
