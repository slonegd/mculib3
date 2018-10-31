#define BOOST_TEST_MODULE f1_dma
#include <boost/test/unit_test.hpp>

#define STM32F103xB
#define TEST


#include <iostream>
#include <type_traits>
#include "periph.h"
#include "f1_bits_dma_stream.h"

struct MockDMA {
   bool good {false};
   bool clear_flags {false};
   template <mcu::Periph p> void clock_enable(){good = true;}
   void clear_interrupt_flags(mcu::DMA_bits::Channel channel) {clear_flags = true;}
   bool is_transfer_complete_interrupt(mcu::DMA_bits::Channel channel) {return true;}
}mockDMA;
namespace mcu {
template <Periph p> std::enable_if_t<p == Periph::TEST_DMA, MockDMA&> make_reference() {return mockDMA;}
}

#include "dma.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::DMA_stream dma_stream;
auto& CMSIS = *reinterpret_cast<mcu::DMA_stream::CMSIS_type*>(&dma_stream);

BOOST_AUTO_TEST_CASE (make_reference)
{
   auto& dma_stream1 {mcu::make_reference<mcu::Periph::DMA1_stream1>()};
   auto address = reinterpret_cast<size_t>(&dma_stream1);
   auto same = std::is_same_v<std::remove_reference_t<decltype(dma_stream1)>, mcu::DMA_stream>;
   BOOST_CHECK_EQUAL (address, DMA1_Channel1_BASE);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (enable)
{
   CMSIS.CCR = 0;
   dma_stream.enable();
   BOOST_CHECK_EQUAL (CMSIS.CCR, DMA_CCR_EN_Msk);
}

BOOST_AUTO_TEST_CASE (disable)
{
   dma_stream.disable();
   BOOST_CHECK_EQUAL (CMSIS.CCR, 0);
}

BOOST_AUTO_TEST_CASE(is_disable)
{
   CMSIS.CCR = 0;
   dma_stream.enable();
   dma_stream.disable();
   BOOST_CHECK_EQUAL(dma_stream.is_disable(), true);
}

BOOST_AUTO_TEST_CASE(inc_memory)
{
   CMSIS.CCR = 0;
   dma_stream.inc_memory();
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_MINC_Msk);
}

BOOST_AUTO_TEST_CASE(inc_periph)
{
   CMSIS.CCR = 0;
   dma_stream.inc_periph();
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_PINC_Msk);
}

BOOST_AUTO_TEST_CASE(circular_mode)
{
   CMSIS.CCR = 0;
   dma_stream.circular_mode();
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_CIRC_Msk);
}

BOOST_AUTO_TEST_CASE(set_memory_adr)
{
   CMSIS.CMAR = 0;
   dma_stream.set_memory_adr(0xFF);
   BOOST_CHECK_EQUAL(CMSIS.CMAR, 0xFF);
}

BOOST_AUTO_TEST_CASE(set_periph_adr)
{
   CMSIS.CPAR = 0;
   dma_stream.set_periph_adr(0x100);
   BOOST_CHECK_EQUAL(CMSIS.CPAR, 0x100);
}

BOOST_AUTO_TEST_CASE(set_qty_ransactions)
{
   CMSIS.CNDTR = 0;
   dma_stream.set_qty_ransactions(0xFF);
   BOOST_CHECK_EQUAL(CMSIS.CNDTR, 0xFF);
}

BOOST_AUTO_TEST_CASE(direction)
{
   CMSIS.CCR = 0;
   dma_stream.direction(mcu::DMA_stream::DataDirection::MemToPer);
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_DIR_Msk);
   dma_stream.direction(mcu::DMA_stream::DataDirection::PerToMem);
   BOOST_CHECK_EQUAL(CMSIS.CCR, not DMA_CCR_DIR_Msk);
}

BOOST_AUTO_TEST_CASE(size_memory)
{
   CMSIS.CCR = 0;
   dma_stream.size_memory(mcu::DMA_stream::DataSize::dword32);
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_MSIZE_1);

   dma_stream.size_memory(mcu::DMA_stream::DataSize::word16);
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_MSIZE_0);

   dma_stream.size_memory(mcu::DMA_stream::DataSize::byte8);
   BOOST_CHECK_EQUAL(CMSIS.CCR, 0);
}

BOOST_AUTO_TEST_CASE(size_periph)
{
   CMSIS.CCR = 0;
   dma_stream.size_periph(mcu::DMA_stream::DataSize::dword32);
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_PSIZE_1);

   dma_stream.size_periph(mcu::DMA_stream::DataSize::word16);
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_PSIZE_0);

   dma_stream.size_periph(mcu::DMA_stream::DataSize::byte8);
   BOOST_CHECK_EQUAL(CMSIS.CCR, 0);
}

BOOST_AUTO_TEST_CASE(channel)
{
   auto channel = mcu::DMA_stream::Channel::_1;
   auto channel1 = dma_stream.channel(mcu::Periph::DMA1_stream1);
   BOOST_CHECK_EQUAL(int (channel1), int (channel));

   channel = mcu::DMA_stream::Channel::_2;
   channel1 = dma_stream.channel(mcu::Periph::DMA1_stream2);
   BOOST_CHECK_EQUAL(int (channel1), int (channel));

   channel = mcu::DMA_stream::Channel::_3;
   channel1 = dma_stream.channel(mcu::Periph::DMA1_stream3);
   BOOST_CHECK_EQUAL(int (channel1), int (channel));

   channel = mcu::DMA_stream::Channel::_4;
   channel1 = dma_stream.channel(mcu::Periph::DMA1_stream4);
   BOOST_CHECK_EQUAL(int (channel1), int (channel));

   channel = mcu::DMA_stream::Channel::_5;
   channel1 = dma_stream.channel(mcu::Periph::DMA1_stream5);
   BOOST_CHECK_EQUAL(int (channel1), int (channel));

   channel = mcu::DMA_stream::Channel::_6;
   channel1 = dma_stream.channel(mcu::Periph::DMA1_stream6);
   BOOST_CHECK_EQUAL(int (channel1), int (channel));

   channel = mcu::DMA_stream::Channel::_7;
   channel1 = dma_stream.channel(mcu::Periph::DMA1_stream7);
   BOOST_CHECK_EQUAL(int (channel1), int (channel));
}

BOOST_AUTO_TEST_CASE(IRQn)
{
   auto irq = DMA1_Channel1_IRQn;
   auto irq1 = dma_stream.IRQn(mcu::Periph::DMA1_stream1);
   BOOST_CHECK_EQUAL(int (irq1), int (irq));

   irq = DMA1_Channel2_IRQn;
   irq1 = dma_stream.IRQn(mcu::Periph::DMA1_stream2);
   BOOST_CHECK_EQUAL(int (irq1), int (irq));

   irq = DMA1_Channel3_IRQn;
   irq1 = dma_stream.IRQn(mcu::Periph::DMA1_stream3);
   BOOST_CHECK_EQUAL(int (irq1), int (irq));

   irq = DMA1_Channel4_IRQn;
   irq1 = dma_stream.IRQn(mcu::Periph::DMA1_stream4);
   BOOST_CHECK_EQUAL(int (irq1), int (irq));

   irq = DMA1_Channel5_IRQn;
   irq1 = dma_stream.IRQn(mcu::Periph::DMA1_stream5);
   BOOST_CHECK_EQUAL(int (irq1), int (irq));

   irq = DMA1_Channel6_IRQn;
   irq1 = dma_stream.IRQn(mcu::Periph::DMA1_stream6);
   BOOST_CHECK_EQUAL(int (irq1), int (irq));

   irq = DMA1_Channel7_IRQn;
   irq1 = dma_stream.IRQn(mcu::Periph::DMA1_stream7);
   BOOST_CHECK_EQUAL(int (irq1), int (irq));
}

BOOST_AUTO_TEST_CASE(enable_transfer_complete_interrupt)
{
   CMSIS.CCR = 0;
   dma_stream.enable_transfer_complete_interrupt();
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_TCIE_Msk);
}

BOOST_AUTO_TEST_CASE(clear_interrupt_flags)
{
   dma_stream.clear_interrupt_flags<mcu::Periph::TEST_DMA>(mcu::DMA_stream::Channel::_1);
   BOOST_CHECK_EQUAL(mockDMA.clear_flags, true);
}

BOOST_AUTO_TEST_CASE(is_transfer_complete_interrupt)
{
   BOOST_CHECK_EQUAL(dma_stream.is_transfer_complete_interrupt<mcu::Periph::TEST_DMA>(mcu::DMA_stream::Channel::_2), true);
}



BOOST_AUTO_TEST_SUITE_END()