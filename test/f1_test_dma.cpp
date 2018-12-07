#define BOOST_TEST_MODULE f1_dma
#include <boost/test/unit_test.hpp>

#define STM32F103xB
#define TEST
#define F_CPU 72000000


#include <iostream>
#include <type_traits>
#include "periph.h"


struct MockRCC {
   bool good {false};
   template <mcu::Periph p> void clock_enable(){good = true;}
}mockRcc;
namespace mcu {
template <Periph p> std::enable_if_t<p == Periph::TEST_RCC, MockRCC&> make_reference() {return mockRcc;}
}

#include "dma.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::DMA dma;
auto& CMSIS = *reinterpret_cast<mcu::DMA::CMSIS_type*>(&dma);

BOOST_AUTO_TEST_CASE (make_reference)
{
   auto& dma {mcu::make_reference<mcu::Periph::DMA1>()};
   auto address = reinterpret_cast<size_t>(&dma);
   auto same = std::is_same_v<std::remove_reference_t<decltype(dma)>, mcu::DMA>;
   BOOST_CHECK_EQUAL (address, DMA1_BASE);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (clock_enable)
{
   dma.clock_enable<mcu::Periph::DMA1, mcu::Periph::TEST_RCC>();
   BOOST_CHECK_EQUAL (mockRcc.good, true);
}

BOOST_AUTO_TEST_CASE (clear_interrupt_flags)
{
   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_1);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF1_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_2);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF2_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_3);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF3_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_4);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF4_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_5);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF5_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_6);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF6_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_7);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF7_Msk);
}

BOOST_AUTO_TEST_CASE(is_transfer_complete_interrupt)
{
   CMSIS.ISR = DMA_ISR_TCIF1_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_1), true);

   CMSIS.ISR = DMA_ISR_TCIF2_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_2), true);

   CMSIS.ISR = DMA_ISR_TCIF3_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_3), true);

   CMSIS.ISR = DMA_ISR_TCIF4_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_4), true);

   CMSIS.ISR = DMA_ISR_TCIF5_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_5), true);

   CMSIS.ISR = DMA_ISR_TCIF6_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_6), true);

   CMSIS.ISR = DMA_ISR_TCIF7_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_7), true);
}


BOOST_AUTO_TEST_SUITE_END()

