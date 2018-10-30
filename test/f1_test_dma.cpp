#define BOOST_TEST_MODULE f1_dma
#include <boost/test/unit_test.hpp>

#define STM32F103xB
#define TEST


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

BOOST_AUTO_TEST_SUITE_END()

