#pragma once

#include <array>
#include <type_traits>
#include "periph_spi.h"
#include "periph_dma.h"
#include "pin.h"
#include "net_buffer.h"
#include "interrupt.h"

template <size_t buffer_size = 255>
class SPI_sized
{
public:
   Net_buffer<buffer_size> buffer_in;
   Net_buffer<buffer_size> buffer_out;

   template <mcu::Periph spi_periph, class MOSI, class MISO, class SCK, class NSS, bool master = false>
   static auto &make();

   void send(uint8_t v) {buffer_in[0] = v;}

protected:
   using Data_size = SPI::Data_size;
   using Channel   = DMA_stream::Channel;
   using Prescaler = SPI::Prescaler;

   Interrupt& interrupt_spi;
   Pin &mosi;
   Pin &miso;
   Pin &sck;
   // Pin &nss;
   DMA &dma;
   SPI &spi;
   DMA_stream &TXstream;
   DMA_stream &RXstream;

   const mcu::Periph spi_periph;
   const Channel TX_channel;

   bool master {false};
   int i{0};

   SPI_sized (
      Interrupt& interrupt_spi, Pin &mosi, Pin &miso, Pin &sck, SPI &spi, DMA &dma, DMA_stream &TXstream, 
      DMA_stream &RXstream, mcu::Periph spi_periph, Channel TX_channel, bool master) 
      : interrupt_spi{interrupt_spi}, mosi{mosi}, miso{miso}, sck{sck}, dma{dma}, spi{spi}, TXstream{TXstream}, 
      RXstream{RXstream}, spi_periph{spi_periph}, TX_channel{TX_channel}, master{master}
   { 
   
   }

   void spiInterrupt()
   {
      if (spi.is_tx_complete()) {
         spi.send(buffer_out[0]);
         // if (i == 8) {spi.disable_tx_complete_interrupt(); i = 0;}
      }
   }

   using Parent = SPI_sized;

   struct spi_interrupt : Interrupting
   {
      Parent& parent;
      spi_interrupt (Parent& parent) : parent(parent) {
         parent.interrupt_spi.subscribe (this);
      }
      void interrupt() override {parent.spiInterrupt();} 
   } spi_ {*this};
};

using SPI_ = SPI_sized<>;


template <size_t buffer_size>
template <mcu::Periph spi_periph, class MOSI, class MISO, class SCK, class NSS, bool master>
auto &SPI_sized<buffer_size>::make()
{
   constexpr auto TX_stream = SPI::TX_stream<spi_periph>();
   constexpr auto RX_stream = SPI::RX_stream<spi_periph>();
   constexpr auto dma_periph = DMA_stream::dma_periph<TX_stream>();
   constexpr auto MOSIpin_mode = SPI::pin_mode<MOSI>();
   constexpr auto MISOpin_mode = SPI::pin_mode<MISO>();
   constexpr auto SCKpin_mode  = SPI::pin_mode<SCK>();

   static SPI_sized<buffer_size> spi{
      get_interrupt<spi_periph>(),
      Pin::make<MOSI, MOSIpin_mode>(), 
      Pin::make<MISO, MISOpin_mode>(),
      Pin::make<SCK,  SCKpin_mode>(),
      mcu::make_reference<spi_periph>(), 
      mcu::make_reference<dma_periph>(), 
      mcu::make_reference<TX_stream>(), 
      mcu::make_reference<RX_stream>(), 
      spi_periph, 
      DMA_stream::channel<TX_stream>(),
      master};

   auto &rcc = REF(RCC);
   rcc.clock_enable<spi_periph>();
   spi.spi.set_mode(master)
          .set(Data_size::_8bits)
          .NSS_soft()
          .NSS_high()
          .set(Prescaler::div32)
         //  .DMA_tx_enable()
         //  .DMA_rx_enable()
          .enable_tx_complete_interrupt()
          .enable_rx_complete_interrupt()
          .enable();
   get_interrupt<spi_periph>().enable();

   // rcc.clock_enable<dma_periph>();
   // spi.TXstream.set(Direction::to_periph)
   //     .set_memory_adr(size_t(spi.buffer_out.begin()))
   //     .set_periph_adr(spi.spi.transmit_data_adr())
   //     .inc_memory()
   //     .size_memory(DataSize::byte8)
   //     .size_periph(DataSize::byte8)
   //     .enable_transfer_complete_interrupt();
   // get_interrupt<TX_stream>().enable();

   // spi.RXstream.set(Direction::to_memory)
   //     .set_memory_adr(size_t(spi.buffer_in.begin()))
   //     .set_periph_adr(spi.spi.receive_data_adr())
   //     .set_qty_transactions(buffer_size)
   //     .inc_memory()
   //     .size_memory(DataSize::byte8)
   //     .size_periph(DataSize::byte8)
   //     .circular_mode();
   
   return spi;
}