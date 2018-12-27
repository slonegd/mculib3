BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::DMA_stream dma_stream;
auto& CMSIS = dma_stream.like_CMSIS();

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   auto same = std::is_same_v<std::remove_reference_t<decltype(CMSIS)>, DMA_Stream_TypeDef>;
   auto address_rcc = reinterpret_cast<size_t>(&dma_stream);
   auto address_CMSIS = reinterpret_cast<size_t>(&CMSIS);
   BOOST_CHECK_EQUAL (address_rcc, address_CMSIS);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (enable)
{
   CMSIS.CR = 0;
   dma_stream.enable();
   BOOST_CHECK_EQUAL (CMSIS.CR, DMA_SxCR_EN_Msk);
}

BOOST_AUTO_TEST_CASE (disable)
{
   CMSIS.CR = DMA_SxCR_EN_Msk;
   dma_stream.disable();
   BOOST_CHECK_EQUAL (CMSIS.CR, 0);
}

BOOST_AUTO_TEST_CASE(is_disable)
{
   CMSIS.CR = 0;
   dma_stream.enable();
   BOOST_CHECK_EQUAL(dma_stream.is_disable(), false);
   dma_stream.disable();
   BOOST_CHECK_EQUAL(dma_stream.is_disable(), true);
}

BOOST_AUTO_TEST_CASE(inc_memory)
{
   CMSIS.CR = 0;
   dma_stream.inc_memory();
   BOOST_CHECK_EQUAL(CMSIS.CR, DMA_SxCR_MINC_Msk);
}

BOOST_AUTO_TEST_CASE(inc_periph)
{
   CMSIS.CR = 0;
   dma_stream.inc_periph();
   BOOST_CHECK_EQUAL(CMSIS.CR, DMA_SxCR_PINC_Msk);
}

BOOST_AUTO_TEST_CASE(circular_mode)
{
   CMSIS.CR = 0;
   dma_stream.circular_mode();
   BOOST_CHECK_EQUAL(CMSIS.CR, DMA_SxCR_CIRC_Msk);
}

BOOST_AUTO_TEST_CASE(set_memory_adr)
{
   CMSIS.M0AR = 0;
   dma_stream.set_memory_adr(0xFF);
   BOOST_CHECK_EQUAL(CMSIS.M0AR, 0xFF);
}

BOOST_AUTO_TEST_CASE(set_periph_adr)
{
   CMSIS.PAR = 0;
   dma_stream.set_periph_adr(0x100);
   BOOST_CHECK_EQUAL(CMSIS.PAR, 0x100);
}

BOOST_AUTO_TEST_CASE(set_qty_ransactions)
{
   CMSIS.NDTR = 0;
   dma_stream.set_qty_transactions(0xFF);
   BOOST_CHECK_EQUAL(CMSIS.NDTR, 0xFF);
}

BOOST_AUTO_TEST_CASE(direction)
{
   CMSIS.CR = 0;
   dma_stream.direction(mcu::DMA_stream::DataDirection::MemToMem);
   BOOST_CHECK_EQUAL(CMSIS.CR, DMA_SxCR_DIR_1);
   dma_stream.direction(mcu::DMA_stream::DataDirection::MemToPer);
   BOOST_CHECK_EQUAL(CMSIS.CR, DMA_SxCR_DIR_0);
   dma_stream.direction(mcu::DMA_stream::DataDirection::PerToMem);
   BOOST_CHECK_EQUAL(CMSIS.CR, 0);
}

BOOST_AUTO_TEST_CASE(size_memory)
{
   CMSIS.CR = 0;
   dma_stream.size_memory(mcu::DMA_stream::DataSize::dword32);
   BOOST_CHECK_EQUAL(CMSIS.CR, DMA_SxCR_MSIZE_1);

   dma_stream.size_memory(mcu::DMA_stream::DataSize::word16);
   BOOST_CHECK_EQUAL(CMSIS.CR, DMA_SxCR_MSIZE_0);

   dma_stream.size_memory(mcu::DMA_stream::DataSize::byte8);
   BOOST_CHECK_EQUAL(CMSIS.CR, 0);
}

BOOST_AUTO_TEST_CASE(size_periph)
{
   CMSIS.CR = 0;
   dma_stream.size_periph(mcu::DMA_stream::DataSize::dword32);
   BOOST_CHECK_EQUAL(CMSIS.CR, DMA_SxCR_PSIZE_1);

   dma_stream.size_periph(mcu::DMA_stream::DataSize::word16);
   BOOST_CHECK_EQUAL(CMSIS.CR, DMA_SxCR_PSIZE_0);

   dma_stream.size_periph(mcu::DMA_stream::DataSize::byte8);
   BOOST_CHECK_EQUAL(CMSIS.CR, 0);
}

BOOST_AUTO_TEST_CASE(IRQn)
{
   constexpr auto irq = dma_stream.IRQn(mcu::Periph::DMA1_stream0);
   BOOST_CHECK_EQUAL (irq, DMA1_Stream0_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA1_stream1), DMA1_Stream1_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA1_stream2), DMA1_Stream2_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA1_stream3), DMA1_Stream3_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA1_stream4), DMA1_Stream4_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA1_stream5), DMA1_Stream5_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA1_stream6), DMA1_Stream6_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA1_stream7), DMA1_Stream7_IRQn);

   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA2_stream0), DMA2_Stream0_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA2_stream1), DMA2_Stream1_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA2_stream2), DMA2_Stream2_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA2_stream3), DMA2_Stream3_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA2_stream4), DMA2_Stream4_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA2_stream5), DMA2_Stream5_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA2_stream6), DMA2_Stream6_IRQn);
   BOOST_CHECK_EQUAL (dma_stream.IRQn(mcu::Periph::DMA2_stream7), DMA2_Stream7_IRQn);

}

BOOST_AUTO_TEST_CASE(enable_transfer_complete_interrupt)
{
   CMSIS.CR = 0;
   dma_stream.enable_transfer_complete_interrupt();
   BOOST_CHECK_EQUAL(CMSIS.CR, DMA_SxCR_TCIE_Msk);
}

BOOST_AUTO_TEST_CASE(qty_transactions_left)
{
   CMSIS.NDTR = 5;
   BOOST_CHECK_EQUAL(dma_stream.qty_transactions_left(), 5);
   BOOST_CHECK_EQUAL(CMSIS.NDTR                        , 5);

   CMSIS.NDTR = 255;
   BOOST_CHECK_EQUAL(dma_stream.qty_transactions_left(), 255);
}

BOOST_AUTO_TEST_SUITE_END()