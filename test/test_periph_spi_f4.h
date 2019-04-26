BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::SPI spi {};
auto& cmsis = mcu::like_CMSIS (spi);

BOOST_AUTO_TEST_CASE (enable_disable)
{
	cmsis.CR1 = 0;
	spi.enable();
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_SPE_Msk);

   spi.disable();
   BOOST_CHECK_EQUAL (cmsis.CR1, not SPI_CR1_SPE_Msk);
}

BOOST_AUTO_TEST_CASE (CRC_enable)
{
	cmsis.CR1 = 0;
	spi.CRC_enable();
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_CRCEN_Msk);
}

BOOST_AUTO_TEST_CASE (DMA_tx_enable)
{
	cmsis.CR2 = 0;
	spi.DMA_tx_enable();
	BOOST_CHECK_EQUAL (cmsis.CR2, SPI_CR2_TXDMAEN_Msk);
}

BOOST_AUTO_TEST_CASE (DMA_rx_enable)
{
	cmsis.CR2 = 0;
	spi.DMA_rx_enable();
	BOOST_CHECK_EQUAL (cmsis.CR2, SPI_CR2_RXDMAEN_Msk);
}

BOOST_AUTO_TEST_CASE (Bidirection_enable)
{
	cmsis.CR1 = 0;
	spi.Bidirection_enable();
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_BIDIMODE_Msk);
}

BOOST_AUTO_TEST_CASE (set_mode)
{
	cmsis.CR1 = 0;
	spi.set(mcu::SPI::Mode::master);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_MSTR_Msk);

	spi.set(mcu::SPI::Mode::slave);
	BOOST_CHECK_EQUAL (cmsis.CR1, not SPI_CR1_MSTR_Msk);
}

BOOST_AUTO_TEST_CASE (set_size)
{
	cmsis.CR1 = 0;
	spi.set(mcu::SPI::Data_size::_16bits);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_DFF_Msk);

   spi.set(mcu::SPI::Data_size::_8bits);
	BOOST_CHECK_EQUAL (cmsis.CR1, not SPI_CR1_DFF_Msk);
}

BOOST_AUTO_TEST_CASE (set_prescaler)
{
	cmsis.CR1 = 0;
	spi.set(mcu::SPI::Prescaler::div256);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_BR_Msk);

	spi.set(mcu::SPI::Prescaler::div128);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_BR_2 | SPI_CR1_BR_1);

   spi.set(mcu::SPI::Prescaler::div64);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_BR_2 | SPI_CR1_BR_0);

   spi.set(mcu::SPI::Prescaler::div32);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_BR_2);

   spi.set(mcu::SPI::Prescaler::div16);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_BR_1 | SPI_CR1_BR_0);

   spi.set(mcu::SPI::Prescaler::div8);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_BR_1);

   spi.set(mcu::SPI::Prescaler::div4);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_BR_0);

   spi.set(mcu::SPI::Prescaler::div2);
	BOOST_CHECK_EQUAL (cmsis.CR1, 0);
}

BOOST_AUTO_TEST_CASE (set_polarity)
{
	cmsis.CR1 = 0;
	spi.set(mcu::SPI::Polarity::high_level);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_CPOL_Msk);

   spi.set(mcu::SPI::Polarity::low_level);
	BOOST_CHECK_EQUAL (cmsis.CR1, not SPI_CR1_CPOL_Msk);
}

BOOST_AUTO_TEST_CASE (set_edge)
{
	cmsis.CR1 = 0;
	spi.set(mcu::SPI::Edge::rear);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_CPHA_Msk);

   spi.set(mcu::SPI::Edge::front);
	BOOST_CHECK_EQUAL (cmsis.CR1, not SPI_CR1_CPHA_Msk);
}

BOOST_AUTO_TEST_CASE (set_first_bit)
{
	cmsis.CR1 = 0;
	spi.set(mcu::SPI::First_bit::low);
	BOOST_CHECK_EQUAL (cmsis.CR1, SPI_CR1_LSBFIRST_Msk);

   spi.set(mcu::SPI::First_bit::high);
	BOOST_CHECK_EQUAL (cmsis.CR1, not SPI_CR1_LSBFIRST_Msk);
}

BOOST_AUTO_TEST_CASE (tx_complete_interrupt)
{
	cmsis.CR2 = 0;
	spi.enable_tx_complete_interrupt();
	BOOST_CHECK_EQUAL (cmsis.CR2, SPI_CR2_TXEIE_Msk);

   spi.disable_tx_complete_interrupt();
   BOOST_CHECK_EQUAL (cmsis.CR2, not SPI_CR2_TXEIE_Msk);
}

BOOST_AUTO_TEST_CASE (rx_complete_interrupt)
{
	cmsis.CR2 = 0;
	spi.enable_rx_complete_interrupt();
	BOOST_CHECK_EQUAL (cmsis.CR2, SPI_CR2_RXNEIE_Msk);

   spi.disable_rx_complete_interrupt();
   BOOST_CHECK_EQUAL (cmsis.CR2, not SPI_CR2_RXNEIE_Msk);
}

BOOST_AUTO_TEST_CASE (error_interrupt)
{
	cmsis.CR2 = 0;
	spi.enable_error_interrupt();
	BOOST_CHECK_EQUAL (cmsis.CR2, SPI_CR2_ERRIE_Msk);

   spi.disable_error_interrupt();
   BOOST_CHECK_EQUAL (cmsis.CR2, not SPI_CR2_ERRIE_Msk);
}

BOOST_AUTO_TEST_CASE (is_tx_complete)
{
	cmsis.SR = 0;
	BOOST_CHECK_EQUAL (spi.is_tx_complete(), false);

   cmsis.SR = SPI_SR_TXE_Msk;
	BOOST_CHECK_EQUAL (spi.is_tx_complete(), true);
}

BOOST_AUTO_TEST_CASE (is_rx_complete)
{
	cmsis.SR = 0;
	BOOST_CHECK_EQUAL (spi.is_rx_complete(), false);

   cmsis.SR = SPI_SR_RXNE_Msk;
	BOOST_CHECK_EQUAL (spi.is_rx_complete(), true);
}

BOOST_AUTO_TEST_SUITE_END()