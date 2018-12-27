#define BOOST_TEST_MODULE f4_test_periph_dma
#include <boost/test/unit_test.hpp>

#define F_CPU   168'000'000UL
#define STM32F405xx


#include <iostream>
#include <type_traits>
#include "periph_dma.h"


BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::DMA dma;
auto& CMSIS = dma.like_CMSIS();

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   auto same = std::is_same_v<std::remove_reference_t<decltype(CMSIS)>, DMA_TypeDef>;
   auto address_rcc = reinterpret_cast<size_t>(&dma);
   auto address_CMSIS = reinterpret_cast<size_t>(&CMSIS);
   BOOST_CHECK_EQUAL (address_rcc, address_CMSIS);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (clear_interrupt_flags)
{
   CMSIS.HIFCR = 0;
   CMSIS.LIFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_0);
   BOOST_CHECK_EQUAL (CMSIS.LIFCR,
        DMA_LIFCR_CFEIF0_Msk
      | DMA_LIFCR_CDMEIF0_Msk
      | DMA_LIFCR_CTEIF0_Msk
      | DMA_LIFCR_CHTIF0_Msk
      | DMA_LIFCR_CTCIF0_Msk
   );
   BOOST_CHECK_EQUAL (CMSIS.HIFCR,
        0
   );

   dma.clear_interrupt_flags(mcu::DMA::Channel::_2);
   BOOST_CHECK_EQUAL (CMSIS.LIFCR,
        DMA_LIFCR_CFEIF0_Msk
      | DMA_LIFCR_CDMEIF0_Msk
      | DMA_LIFCR_CTEIF0_Msk
      | DMA_LIFCR_CHTIF0_Msk
      | DMA_LIFCR_CTCIF0_Msk
      | DMA_LIFCR_CFEIF2_Msk
      | DMA_LIFCR_CDMEIF2_Msk
      | DMA_LIFCR_CTEIF2_Msk
      | DMA_LIFCR_CHTIF2_Msk
      | DMA_LIFCR_CTCIF2_Msk
   );
   BOOST_CHECK_EQUAL (CMSIS.HIFCR,
        0
   );

   dma.clear_interrupt_flags(mcu::DMA::Channel::_4);
   BOOST_CHECK_EQUAL (CMSIS.LIFCR,
        DMA_LIFCR_CFEIF0_Msk
      | DMA_LIFCR_CDMEIF0_Msk
      | DMA_LIFCR_CTEIF0_Msk
      | DMA_LIFCR_CHTIF0_Msk
      | DMA_LIFCR_CTCIF0_Msk
      | DMA_LIFCR_CFEIF2_Msk
      | DMA_LIFCR_CDMEIF2_Msk
      | DMA_LIFCR_CTEIF2_Msk
      | DMA_LIFCR_CHTIF2_Msk
      | DMA_LIFCR_CTCIF2_Msk
   );
   BOOST_CHECK_EQUAL (CMSIS.HIFCR,
        DMA_HIFCR_CFEIF4_Msk
      | DMA_HIFCR_CDMEIF4_Msk
      | DMA_HIFCR_CTEIF4_Msk
      | DMA_HIFCR_CHTIF4_Msk
      | DMA_HIFCR_CTCIF4_Msk
   );

   CMSIS.HIFCR = 0;
   CMSIS.LIFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_1);
   BOOST_CHECK_EQUAL (CMSIS.LIFCR,
        DMA_LIFCR_CFEIF1_Msk
      | DMA_LIFCR_CDMEIF1_Msk
      | DMA_LIFCR_CTEIF1_Msk
      | DMA_LIFCR_CHTIF1_Msk
      | DMA_LIFCR_CTCIF1_Msk
   );
   BOOST_CHECK_EQUAL (CMSIS.HIFCR,
        0
   );

   CMSIS.HIFCR = 0;
   CMSIS.LIFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_3);
   BOOST_CHECK_EQUAL (CMSIS.LIFCR,
        DMA_LIFCR_CFEIF3_Msk
      | DMA_LIFCR_CDMEIF3_Msk
      | DMA_LIFCR_CTEIF3_Msk
      | DMA_LIFCR_CHTIF3_Msk
      | DMA_LIFCR_CTCIF3_Msk
   );
   BOOST_CHECK_EQUAL (CMSIS.HIFCR,
        0
   );

   CMSIS.HIFCR = 0;
   CMSIS.LIFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_5);
   BOOST_CHECK_EQUAL (CMSIS.LIFCR,
        0
   );
   BOOST_CHECK_EQUAL (CMSIS.HIFCR,
        DMA_HIFCR_CFEIF5_Msk
      | DMA_HIFCR_CDMEIF5_Msk
      | DMA_HIFCR_CTEIF5_Msk
      | DMA_HIFCR_CHTIF5_Msk
      | DMA_HIFCR_CTCIF5_Msk
   );

   CMSIS.HIFCR = 0;
   CMSIS.LIFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_6);
   BOOST_CHECK_EQUAL (CMSIS.LIFCR,
        0
   );
   BOOST_CHECK_EQUAL (CMSIS.HIFCR,
        DMA_HIFCR_CFEIF6_Msk
      | DMA_HIFCR_CDMEIF6_Msk
      | DMA_HIFCR_CTEIF6_Msk
      | DMA_HIFCR_CHTIF6_Msk
      | DMA_HIFCR_CTCIF6_Msk
   );

   CMSIS.HIFCR = 0;
   CMSIS.LIFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_7);
   BOOST_CHECK_EQUAL (CMSIS.LIFCR,
        0
   );
   BOOST_CHECK_EQUAL (CMSIS.HIFCR,
        DMA_HIFCR_CFEIF7_Msk
      | DMA_HIFCR_CDMEIF7_Msk
      | DMA_HIFCR_CTEIF7_Msk
      | DMA_HIFCR_CHTIF7_Msk
      | DMA_HIFCR_CTCIF7_Msk
   );
}

BOOST_AUTO_TEST_CASE(is_transfer_complete_interrupt)
{
   CMSIS.LISR = DMA_LISR_TCIF0_Msk;
   CMSIS.HISR = 0;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_0), true);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_1), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_2), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_3), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_4), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_5), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_6), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_7), false);

   CMSIS.HISR = DMA_HISR_TCIF7_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_0), true);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_1), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_2), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_3), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_4), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_5), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_6), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_7), true);

   CMSIS.LISR = DMA_LISR_TCIF0_Msk | DMA_LISR_TCIF1_Msk;
   CMSIS.HISR = DMA_HISR_TCIF7_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_0), true);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_1), true);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_2), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_3), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_4), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_5), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_6), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_7), true);

   CMSIS.LISR = DMA_LISR_TCIF2_Msk;
   CMSIS.HISR = DMA_HISR_TCIF6_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_0), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_1), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_2), true);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_3), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_4), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_5), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_6), true);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_7), false);

   CMSIS.LISR = DMA_LISR_TCIF3_Msk;
   CMSIS.HISR = DMA_HISR_TCIF5_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_0), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_1), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_2), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_3), true);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_4), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_5), true);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_6), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_7), false);

   CMSIS.LISR = 0;
   CMSIS.HISR = DMA_HISR_TCIF4_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_0), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_1), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_2), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_3), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_4), true);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_5), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_6), false);
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt (dma.Channel::_7), false);
}


BOOST_AUTO_TEST_SUITE_END()