#define BOOST_TEST_MODULE f0_test_periph_adc
#include <boost/test/unit_test.hpp>

#define F_CPU   48'000'000UL
#define STM32F030x6


// #include <iostream>
// #include <type_traits>
#include "periph_adc.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::ADC adc {};
auto& cmsis = mcu::like_CMSIS (adc);

BOOST_AUTO_TEST_CASE (enable)
{
   cmsis.CR = 0;
   adc.enable();
   BOOST_CHECK_EQUAL (cmsis.CR, ADC_CR_ADEN_Msk);
}

BOOST_AUTO_TEST_CASE (disable)
{
   cmsis.CR = 0;
   adc.disable();
   BOOST_CHECK_EQUAL (cmsis.CR, ADC_CR_ADDIS_Msk);
}

BOOST_AUTO_TEST_CASE (is_enable)
{
   cmsis.CR = 0;
   BOOST_CHECK_EQUAL (adc.is_enable(), false);
   adc.enable();
   BOOST_CHECK_EQUAL (adc.is_enable(), true);
}

BOOST_AUTO_TEST_CASE (start)
{
   cmsis.CR = 0;
   adc.start();
   BOOST_CHECK_EQUAL (cmsis.CR, ADC_CR_ADSTART_Msk);
}

BOOST_AUTO_TEST_CASE (dma_enable)
{
   cmsis.CFGR1 = 0;
   adc.dma_enable();
   BOOST_CHECK_EQUAL (cmsis.CFGR1, ADC_CFGR1_DMAEN_Msk);
}

BOOST_AUTO_TEST_CASE (set_dma_mode)
{
   cmsis.CFGR1 = 0;
   adc.set (decltype(adc)::DMA_mode::circular);
   BOOST_CHECK_EQUAL (cmsis.CFGR1, ADC_CFGR1_DMACFG_Msk);

   adc.set (decltype(adc)::DMA_mode::one_shot);
   BOOST_CHECK_EQUAL (cmsis.CFGR1, 0);
}

BOOST_AUTO_TEST_CASE (set_continuous)
{
   cmsis.CFGR1 = 0;
   adc.set_continuous();
   BOOST_CHECK_EQUAL (cmsis.CFGR1, ADC_CFGR1_CONT_Msk);
}

BOOST_AUTO_TEST_CASE (set_clock)
{
   cmsis.CFGR2 = 0;
   adc.set (decltype(adc)::Clock::PCLKdiv4);
   BOOST_CHECK_EQUAL (cmsis.CFGR2
      , ADC_CFGR2_CKMODE_1
   );

   adc.set (decltype(adc)::Clock::PCLKdiv2);
   BOOST_CHECK_EQUAL (cmsis.CFGR2
      , ADC_CFGR2_CKMODE_0
   );

   adc.set (decltype(adc)::Clock::dedicated_14MHz_clock);
   BOOST_CHECK_EQUAL (cmsis.CFGR2
      , 0
   );
}

BOOST_AUTO_TEST_CASE (set_resolution)
{
   cmsis.CFGR1 = 0;
   adc.set (decltype(adc)::Resolution::_6_bit);
   BOOST_CHECK_EQUAL (cmsis.CFGR1
      , ADC_CFGR1_RES_0
      | ADC_CFGR1_RES_1
   );

   adc.set (decltype(adc)::Resolution::_8_bit);
   BOOST_CHECK_EQUAL (cmsis.CFGR1
      , ADC_CFGR1_RES_1
   );

   adc.set (decltype(adc)::Resolution::_10_bit);
   BOOST_CHECK_EQUAL (cmsis.CFGR1
      , ADC_CFGR1_RES_0
   );

   adc.set (decltype(adc)::Resolution::_12_bit);
   adc.set (decltype(adc)::Resolution::_12_bit);
   BOOST_CHECK_EQUAL (cmsis.CFGR1
      , 0
   );
}

BOOST_AUTO_TEST_CASE (set_sample_time)
{
   cmsis.SMPR = 0;
   adc.set (decltype(adc)::Sample_time::_239_5_CLK);
   BOOST_CHECK_EQUAL (cmsis.SMPR
      , ADC_SMPR1_SMPR_0
      | ADC_SMPR1_SMPR_1
      | ADC_SMPR1_SMPR_2
   );

   adc.set (decltype(adc)::Sample_time::_71_5_CLK);
   BOOST_CHECK_EQUAL (cmsis.SMPR
      , ADC_SMPR1_SMPR_1
      | ADC_SMPR1_SMPR_2
   );

   adc.set (decltype(adc)::Sample_time::_55_5_CLK);
   BOOST_CHECK_EQUAL (cmsis.SMPR
      , ADC_SMPR1_SMPR_0
      | ADC_SMPR1_SMPR_2
   );

   adc.set (decltype(adc)::Sample_time::_41_5_CLK);
   BOOST_CHECK_EQUAL (cmsis.SMPR
      , ADC_SMPR1_SMPR_2
   );

   adc.set (decltype(adc)::Sample_time::_28_5_CLK);
   BOOST_CHECK_EQUAL (cmsis.SMPR
      , ADC_SMPR1_SMPR_0
      | ADC_SMPR1_SMPR_1
   );

   adc.set (decltype(adc)::Sample_time::_13_5_CLK);
   BOOST_CHECK_EQUAL (cmsis.SMPR
      , ADC_SMPR1_SMPR_1
   );

   adc.set (decltype(adc)::Sample_time::_7_5_CLK);
   BOOST_CHECK_EQUAL (cmsis.SMPR
      , ADC_SMPR1_SMPR_0
   );

   adc.set (decltype(adc)::Sample_time::_1_5_CLK);
   BOOST_CHECK_EQUAL (cmsis.SMPR
      , 0
   );

   BOOST_CHECK_EQUAL (decltype(adc)::Sample_time::Default, decltype(adc)::Sample_time::_239_5_CLK);
}

BOOST_AUTO_TEST_CASE (is_ready)
{
   cmsis.ISR = 0;
   BOOST_CHECK_EQUAL (adc.is_ready(), false);
   cmsis.ISR = ADC_ISR_ADRDY_Msk;
   BOOST_CHECK_EQUAL (adc.is_ready(), true);
}

BOOST_AUTO_TEST_CASE (set_busy)
{
   cmsis.ISR = 0;
   adc.set_busy();
   BOOST_CHECK_EQUAL (cmsis.ISR, ADC_ISR_ADRDY_Msk);
}

BOOST_AUTO_TEST_CASE (stop)
{
   cmsis.CR = 0;
   adc.stop();
   BOOST_CHECK_EQUAL (cmsis.CR, ADC_CR_ADSTP_Msk);
}

BOOST_AUTO_TEST_CASE (is_stoping)
{
   cmsis.CR = 0;
   BOOST_CHECK_EQUAL (adc.is_stoping(), false);
   cmsis.CR = ADC_CR_ADSTP_Msk;
   BOOST_CHECK_EQUAL (adc.is_stoping(), true);
}

BOOST_AUTO_TEST_CASE (data)
{
   auto& data = adc.data();
   BOOST_CHECK_EQUAL (&data, &cmsis.DR);

   cmsis.DR = 0;
   BOOST_CHECK_EQUAL (data, 0);

   cmsis.DR = 1;
   BOOST_CHECK_EQUAL (data, 1);
}

BOOST_AUTO_TEST_SUITE_END()
