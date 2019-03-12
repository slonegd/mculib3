BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::DMA_stream dma_stream;
auto& CMSIS = dma_stream.like_CMSIS();

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   auto same = std::is_same_v<std::remove_reference_t<decltype(CMSIS)>, DMA_Channel_TypeDef>;
   auto address_rcc = reinterpret_cast<size_t>(&dma_stream);
   auto address_CMSIS = reinterpret_cast<size_t>(&CMSIS);
   BOOST_CHECK_EQUAL (address_rcc, address_CMSIS);
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
   CMSIS.CCR = DMA_CCR_EN_Msk;
   dma_stream.disable();
   BOOST_CHECK_EQUAL (CMSIS.CCR, 0);
}

BOOST_AUTO_TEST_CASE(is_disable)
{
   CMSIS.CCR = 0;
   dma_stream.enable();
   BOOST_CHECK_EQUAL(dma_stream.is_disable(), false);
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
   dma_stream.set_qty_transactions(0xFF);
   BOOST_CHECK_EQUAL(CMSIS.CNDTR, 0xFF);
}

BOOST_AUTO_TEST_CASE(set_direction)
{
   CMSIS.CCR = 0;
   dma_stream.set (mcu::DMA_stream::Direction::to_periph);
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_DIR_Msk);
   dma_stream.set (mcu::DMA_stream::Direction::to_memory);
   BOOST_CHECK_EQUAL(CMSIS.CCR, 0);
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

BOOST_AUTO_TEST_CASE(enable_transfer_complete_interrupt)
{
   CMSIS.CCR = 0;
   dma_stream.enable_transfer_complete_interrupt();
   BOOST_CHECK_EQUAL(CMSIS.CCR, DMA_CCR_TCIE_Msk);
}

BOOST_AUTO_TEST_CASE(qty_transactions_left)
{
   CMSIS.CNDTR = 5;
   BOOST_CHECK_EQUAL(dma_stream.qty_transactions_left(), 5);
   BOOST_CHECK_EQUAL(CMSIS.CNDTR                       , 5);

   CMSIS.CNDTR = 255;
   BOOST_CHECK_EQUAL(dma_stream.qty_transactions_left(), 255);
}

BOOST_AUTO_TEST_SUITE_END()