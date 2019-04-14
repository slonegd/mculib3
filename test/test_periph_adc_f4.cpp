#define BOOST_TEST_MODULE f4_test_periph_adc
#include <boost/test/unit_test.hpp>

#define F_CPU   168'000'000UL
#define STM32F405xx

#include <iostream>
#include <type_traits>
#include "catch_static_assert.h"
#include "periph_adc.h"
#undef static_assert


BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::ADC adc {};
auto& cmsis = mcu::like_CMSIS (adc);

BOOST_AUTO_TEST_CASE (enable_disable)
{
	cmsis.CR2 = 0;
	adc.enable();
	BOOST_CHECK_EQUAL (cmsis.CR2, ADC_CR2_ADON_Msk);

   adc.disable();
   BOOST_CHECK_EQUAL (cmsis.CR2, not ADC_CR2_ADON_Msk);
}

BOOST_AUTO_TEST_CASE (is_enable)
{
	cmsis.CR2 = 0;
	BOOST_CHECK_EQUAL (adc.is_enable(), false);
	adc.enable();
	BOOST_CHECK_EQUAL (adc.is_enable(), true);
}

BOOST_AUTO_TEST_CASE (start)
{
	cmsis.CR2 = 0;
	adc.start();
	BOOST_CHECK_EQUAL (cmsis.CR2, ADC_CR2_SWSTART_Msk);
}

BOOST_AUTO_TEST_CASE (dma_enable)
{
	cmsis.CR2 = 0;
	adc.dma_enable();
	BOOST_CHECK_EQUAL (cmsis.CR2, ADC_CR2_DMA_Msk);
}

BOOST_AUTO_TEST_CASE (resolution)
{
	cmsis.CR1 = 0;
	adc.set(decltype(adc)::Resolution::_6_bit);
	BOOST_CHECK_EQUAL (cmsis.CR1, ADC_CR1_RES_Msk);

   cmsis.CR1 = 0;
	adc.set(decltype(adc)::Resolution::_8_bit);
   BOOST_CHECK_EQUAL (cmsis.CR1, ADC_CR1_RES_1);

   cmsis.CR1 = 0;
	adc.set(decltype(adc)::Resolution::_10_bit);
   BOOST_CHECK_EQUAL (cmsis.CR1, ADC_CR1_RES_0);

   cmsis.CR1 = 0;
	adc.set(decltype(adc)::Resolution::_12_bit);
   BOOST_CHECK_EQUAL (cmsis.CR1, 0);
}

BOOST_AUTO_TEST_CASE (set_scan_mode)
{
	cmsis.CR1 = 0;
	adc.set_scan_mode();
	BOOST_CHECK_EQUAL (cmsis.CR1, ADC_CR1_SCAN);
}

BOOST_AUTO_TEST_CASE (set_continuous_mode)
{
	cmsis.CR2 = 0;
	adc.set_continuous();
	BOOST_CHECK_EQUAL (cmsis.CR2, ADC_CR2_CONT);
}

// FIX не понял к чему этот тест?
// BOOST_AUTO_TEST_CASE (set_circular_dma)
// {
// 	cmsis.CR2 = 0;
// 	adc.set_circular_dma();
// 	BOOST_CHECK_EQUAL (cmsis.CR2, ADC_CR2_DDS);
// }

BOOST_AUTO_TEST_CASE (Sample_time)
{
	cmsis.SMPR2 = 0;
	adc.set(0, decltype(adc)::Sample_time::_480CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR2, ADC_SMPR2_SMP0_Msk);

    cmsis.SMPR2 = 0;
	adc.set(1, decltype(adc)::Sample_time::_480CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR2, ADC_SMPR2_SMP1_Msk);

    cmsis.SMPR2 = 0;
	adc.set(2, decltype(adc)::Sample_time::_144CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR2, ADC_SMPR2_SMP2_1 | 
                                   ADC_SMPR2_SMP2_2);

    cmsis.SMPR2 = 0;
	adc.set(3, decltype(adc)::Sample_time::_112CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR2, ADC_SMPR2_SMP3_0 | 
                                   ADC_SMPR2_SMP3_2);

    cmsis.SMPR2 = 0;
	adc.set(4, decltype(adc)::Sample_time::_84CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR2, ADC_SMPR2_SMP4_2);

    cmsis.SMPR2 = 0;
	adc.set(5, decltype(adc)::Sample_time::_56CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR2, ADC_SMPR2_SMP5_0 |
                                   ADC_SMPR2_SMP5_1);

	cmsis.SMPR2 = 0;
	adc.set(6, decltype(adc)::Sample_time::_28CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR2, ADC_SMPR2_SMP6_1);

	cmsis.SMPR2 = 0;
	adc.set(7, decltype(adc)::Sample_time::_15CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR2, ADC_SMPR2_SMP7_0);

	cmsis.SMPR2 = 0;
	adc.set(8, decltype(adc)::Sample_time::_3CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR2, 0);

	cmsis.SMPR2 = 0;
	adc.set(9, decltype(adc)::Sample_time::Default);
	BOOST_CHECK_EQUAL (cmsis.SMPR2, ADC_SMPR2_SMP9_Msk);

	cmsis.SMPR1 = 0;
	adc.set(10, decltype(adc)::Sample_time::Default);
	BOOST_CHECK_EQUAL (cmsis.SMPR1, ADC_SMPR1_SMP10_Msk);

	cmsis.SMPR1 = 0;
	adc.set(11, decltype(adc)::Sample_time::_480CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR1, ADC_SMPR1_SMP11_Msk);

	cmsis.SMPR1 = 0;
	adc.set(12, decltype(adc)::Sample_time::_144CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR1, ADC_SMPR1_SMP12_1 | 
                                   ADC_SMPR1_SMP12_2);

	cmsis.SMPR1 = 0;
	adc.set(13, decltype(adc)::Sample_time::_112CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR1, ADC_SMPR1_SMP13_0 | 
                                   ADC_SMPR1_SMP13_2);

	cmsis.SMPR1 = 0;
	adc.set(14, decltype(adc)::Sample_time::_84CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR1, ADC_SMPR1_SMP14_2);

   cmsis.SMPR1 = 0;
	adc.set(15, decltype(adc)::Sample_time::_56CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR1, ADC_SMPR1_SMP15_0 |
                                   ADC_SMPR1_SMP15_1);

	cmsis.SMPR1 = 0;
	adc.set(16, decltype(adc)::Sample_time::_28CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR1, ADC_SMPR1_SMP16_1);

	cmsis.SMPR1 = 0;
	adc.set(17, decltype(adc)::Sample_time::_15CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR1, ADC_SMPR1_SMP17_0);

	cmsis.SMPR1 = 0;
	adc.set(18, decltype(adc)::Sample_time::_3CLK);
	BOOST_CHECK_EQUAL (cmsis.SMPR1, 0);
}

BOOST_AUTO_TEST_CASE (set_regular_sequence_order)
{
	cmsis.SQR3 = 0;
	adc.set_regular_sequence_order(1, 1);
	BOOST_CHECK_EQUAL (cmsis.SQR3, ADC_SQR3_SQ1_0);

	cmsis.SQR3 = 0;
	adc.set_regular_sequence_order(1, 18);
	BOOST_CHECK_EQUAL (cmsis.SQR3, ADC_SQR3_SQ1_1 |
											 ADC_SQR3_SQ1_4);

	cmsis.SQR3 = 0;
	adc.set_regular_sequence_order(2, 2);
	BOOST_CHECK_EQUAL (cmsis.SQR3, ADC_SQR3_SQ2_1);

	cmsis.SQR3 = 0;
	adc.set_regular_sequence_order(3, 4);
	BOOST_CHECK_EQUAL (cmsis.SQR3, ADC_SQR3_SQ3_2);

	cmsis.SQR3 = 0;
	adc.set_regular_sequence_order(4, 3);
	BOOST_CHECK_EQUAL (cmsis.SQR3, ADC_SQR3_SQ4_0 |
											 ADC_SQR3_SQ4_1);

	cmsis.SQR3 = 0;
	adc.set_regular_sequence_order(5, 17);
	BOOST_CHECK_EQUAL (cmsis.SQR3, ADC_SQR3_SQ5_0 |
											 ADC_SQR3_SQ5_4);

	cmsis.SQR3 = 0;
	adc.set_regular_sequence_order(6, 16);
	BOOST_CHECK_EQUAL (cmsis.SQR3, ADC_SQR3_SQ6_4);

	cmsis.SQR2 = 0;
	adc.set_regular_sequence_order(7, 15);
	BOOST_CHECK_EQUAL (cmsis.SQR2, ADC_SQR2_SQ7_0 |
											 ADC_SQR2_SQ7_1 |
											 ADC_SQR2_SQ7_2 |
											 ADC_SQR2_SQ7_3);

	cmsis.SQR2 = 0;
	adc.set_regular_sequence_order(8, 14);
	BOOST_CHECK_EQUAL (cmsis.SQR2, ADC_SQR2_SQ8_1 |
											 ADC_SQR2_SQ8_2 |
											 ADC_SQR2_SQ8_3);

	cmsis.SQR2 = 0;
	adc.set_regular_sequence_order(9, 13);
	BOOST_CHECK_EQUAL (cmsis.SQR2, ADC_SQR2_SQ9_0 |
											 ADC_SQR2_SQ9_2 |
											 ADC_SQR2_SQ9_3);

	cmsis.SQR2 = 0;
	adc.set_regular_sequence_order(10, 12);
	BOOST_CHECK_EQUAL (cmsis.SQR2, ADC_SQR2_SQ10_2 |
											 ADC_SQR2_SQ10_3);

	cmsis.SQR2 = 0;
	adc.set_regular_sequence_order(11, 11);
	BOOST_CHECK_EQUAL (cmsis.SQR2, ADC_SQR2_SQ11_0 |
											 ADC_SQR2_SQ11_1 |
											 ADC_SQR2_SQ11_3);

	cmsis.SQR2 = 0;
	adc.set_regular_sequence_order(12, 10);
	BOOST_CHECK_EQUAL (cmsis.SQR2, ADC_SQR2_SQ12_1 |
											 ADC_SQR2_SQ12_3);

	cmsis.SQR1 = 0;
	adc.set_regular_sequence_order(13, 9);
	BOOST_CHECK_EQUAL (cmsis.SQR1, ADC_SQR1_SQ13_0 |
											 ADC_SQR1_SQ13_3);

	cmsis.SQR1 = 0;
	adc.set_regular_sequence_order(14, 8);
	BOOST_CHECK_EQUAL (cmsis.SQR1, ADC_SQR1_SQ14_3);

	cmsis.SQR1 = 0;
	adc.set_regular_sequence_order(15, 7);
	BOOST_CHECK_EQUAL (cmsis.SQR1, ADC_SQR1_SQ15_0 |
											 ADC_SQR1_SQ15_1 |
											 ADC_SQR1_SQ15_2);

	cmsis.SQR1 = 0;
	adc.set_regular_sequence_order(16, 6);
	BOOST_CHECK_EQUAL (cmsis.SQR1, ADC_SQR1_SQ16_1 |
											 ADC_SQR1_SQ16_2);

}

BOOST_AUTO_TEST_CASE (set_regular_sequence_length)
{
	cmsis.SQR1 = 0;
	adc.set_regular_sequence_length(1);
	BOOST_CHECK_EQUAL (cmsis.SQR1, 0);

	cmsis.SQR1 = 0;
	adc.set_regular_sequence_length(16);
	BOOST_CHECK_EQUAL (cmsis.SQR1, ADC_SQR1_L_0 |
	                               ADC_SQR1_L_1 |
											 ADC_SQR1_L_2 |
											 ADC_SQR1_L_3);

	cmsis.SQR1 = 0;
	adc.set_regular_sequence_length(15);
	BOOST_CHECK_EQUAL (cmsis.SQR1, ADC_SQR1_L_1 |
	                               ADC_SQR1_L_2 |
											 ADC_SQR1_L_3);

	cmsis.SQR1 = 0;
	adc.set_regular_sequence_length(10);
	BOOST_CHECK_EQUAL (cmsis.SQR1, ADC_SQR1_L_0 |
	                               ADC_SQR1_L_3);

	cmsis.SQR1 = 0;
	adc.set_regular_sequence_length(7);
	BOOST_CHECK_EQUAL (cmsis.SQR1, ADC_SQR1_L_1 |
											 ADC_SQR1_L_2);

	cmsis.SQR1 = 0;
	adc.set_regular_sequence_length(4);
	BOOST_CHECK_EQUAL (cmsis.SQR1, ADC_SQR1_L_0 |
	                               ADC_SQR1_L_1);

	cmsis.SQR1 = 0;
	adc.set_regular_sequence_length(2);
	BOOST_CHECK_EQUAL (cmsis.SQR1, ADC_SQR1_L_0);

}



BOOST_AUTO_TEST_SUITE_END()