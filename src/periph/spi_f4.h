#pragma once

#include "bits_spi_f4.h"
#include "pin.h"

namespace mcu {

class SPI 
{
   volatile SPI_bits::CR1 CR1;     // SPI control register 1 (not used in I2S mode),      Address offset: 0x00
   volatile SPI_bits::CR2 CR2;     // SPI control register 2,                             Address offset: 0x04
   volatile SPI_bits::SR  SR;      // SPI status register,                                Address offset: 0x08
   volatile uint32_t      DR;      // SPI data register,                                  Address offset: 0x0C
   volatile uint32_t      CRCPR;   // SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10
   volatile uint32_t      RXCRCR;  // SPI RX CRC register (not used in I2S mode),         Address offset: 0x14
   volatile uint32_t      TXCRCR;  // SPI TX CRC register (not used in I2S mode),         Address offset: 0x18
   volatile uint32_t      I2SCFGR; // SPI_I2S configuration register,                     Address offset: 0x1C
   volatile uint32_t      I2SPR;   // SPI_I2S prescaler register,                         Address offset: 0x20
public:
   using CMSIS_type  = SPI_TypeDef;
   using Mode        = SPI_bits::CR1::Mode;
   using Edge        = SPI_bits::CR1::Edge;
   using First_bit   = SPI_bits::CR1::First_bit;
   using Data_size   = SPI_bits::CR1::Data_size;
   using Prescaler   = SPI_bits::CR1::Prescaler;
   using Polarity    = SPI_bits::CR1::Polarity;

   SPI& enable()            {CR1.SPE = true;     return *this;}
   SPI& disable()           {CR1.SPE = false;    return *this;}
   SPI& CRC_enable()        {CR1.CRCEN = true;   return *this;}
   SPI& DMA_tx_enable()     {CR2.TXDMAEN = true; return *this;}
   SPI& DMA_rx_enable()     {CR2.RXDMAEN = true; return *this;}
   SPI& Bidirection_enable(){CR1.BIDIMODE = true;return *this;}
   SPI& NSS_soft()          {CR1.SSM = true;     return *this;}
   SPI& NSS_high()          {CR1.SSI = true;     return *this;}
   SPI& set(Mode mode)      {CR1.MSTR = mode;    return *this;}
   SPI& set(Data_size size) {CR1.DFF = size;     return *this;}
   SPI& set(Prescaler div)  {CR1.BR = div;       return *this;}
   SPI& set(Polarity p)     {CR1.CPOL = p;       return *this;}
   SPI& set(Edge edge)      {CR1.CPHA = edge;    return *this;}
   SPI& set(First_bit bit)  {CR1.LSBFIRST = bit; return *this;}

   SPI& send(uint8_t data)  {DR = data; return *this;}

   SPI& enable_tx_complete_interrupt () {CR2.TXEIE  = true;  return *this;}
   SPI& disable_tx_complete_interrupt() {CR2.TXEIE  = false; return *this;}
   SPI& enable_rx_complete_interrupt () {CR2.RXNEIE = true;  return *this;}
   SPI& disable_rx_complete_interrupt() {CR2.RXNEIE = false; return *this;}
   SPI& enable_error_interrupt ()       {CR2.ERRIE  = true;  return *this;}
   SPI& disable_error_interrupt()       {CR2.ERRIE  = false; return *this;}

   bool is_tx_complete() {return SR.TXE;}
   bool is_rx_complete() {return SR.RXNE;}

   size_t receive_data_adr () {return reinterpret_cast<size_t>(&DR);}
   size_t transmit_data_adr() {return reinterpret_cast<size_t>(&DR);}

   template<class Pin> static constexpr PinMode pin_mode();
   template<Periph spi> static constexpr Periph TX_stream();
   template<Periph spi> static constexpr Periph RX_stream();

};

#if not defined(USE_MOCK_SPI)
SFINAE(SPI1,SPI) make_reference() {return *reinterpret_cast<SPI*>(SPI1_BASE);}
SFINAE(SPI2,SPI) make_reference() {return *reinterpret_cast<SPI*>(SPI2_BASE);}
SFINAE(SPI3,SPI) make_reference() {return *reinterpret_cast<SPI*>(SPI3_BASE);}
#endif




template <class Pin> constexpr PinMode SPI::pin_mode()
{
   if constexpr (std::is_same_v<Pin, PA4>  or std::is_same_v<Pin, PA5>  or
                 std::is_same_v<Pin, PA6>  or std::is_same_v<Pin, PA7>  or
                 std::is_same_v<Pin, PA15> or std::is_same_v<Pin, PB3>  or
                 std::is_same_v<Pin, PB4>  or std::is_same_v<Pin, PB5>  or
                 std::is_same_v<Pin, PA15> or std::is_same_v<Pin, PB3>  or
                 std::is_same_v<Pin, PB4>  or std::is_same_v<Pin, PB5>  or
                 std::is_same_v<Pin, PB9>  or std::is_same_v<Pin, PB10> or
                 std::is_same_v<Pin, PB12> or std::is_same_v<Pin, PB13> or
                 std::is_same_v<Pin, PB14> or std::is_same_v<Pin, PB15> or
                 std::is_same_v<Pin, PC2>  or std::is_same_v<Pin, PC3>) return PinMode::Alternate_5;
   else static_assert (always_false_v<Pin>, "неверный аргумент шаблона class Pin");
}

template<Periph spi> constexpr Periph SPI::TX_stream()
{
   if      constexpr (spi == Periph::SPI1) return Periph::DMA2_stream3;
   else if constexpr (spi == Periph::SPI2) return Periph::DMA1_stream4;
   else if constexpr (spi == Periph::SPI3) return Periph::DMA1_stream5;
}

template<Periph spi> constexpr Periph SPI::RX_stream()
{
   if      constexpr (spi == Periph::SPI1) return Periph::DMA2_stream0;
   else if constexpr (spi == Periph::SPI2) return Periph::DMA1_stream0;
   else if constexpr (spi == Periph::SPI3) return Periph::DMA1_stream3;
}

} // namespace mcu