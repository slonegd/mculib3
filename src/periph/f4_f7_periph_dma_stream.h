#pragma once

#include "bits_dma_stream_f4_f7.h"
#include "periph_dma.h"

namespace mcu {

class DMA_stream {
   volatile DMA_bits::CR CR;     /*!< DMA stream x configuration register      */
   volatile uint32_t NDTR;   /*!< DMA stream x number of data register     */
   volatile uint32_t PAR;    /*!< DMA stream x peripheral address register */
   volatile uint32_t M0AR;   /*!< DMA stream x memory 0 address register   */
   volatile uint32_t M1AR;   /*!< DMA stream x memory 1 address register   */
   volatile uint32_t FCR;    /*!< DMA stream x FIFO control register       */

public:
   using CMSIS_type    = DMA_Stream_TypeDef;
   using DataSize      = DMA_bits::CR::DataSize;
   using Priority      = DMA_bits::CR::Priority;
   using DataDirection = DMA_bits::CR::DataDirection;
   using Channel       = DMA_bits::CR::Channel;

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   DMA_stream& enable       (){CR.EN = true; while(not CR.EN){} return *this;}
   DMA_stream& disable      (){CR.EN = false;  return *this;}
   bool        is_disable   (){return (not CR.EN);}
   DMA_stream& inc_memory   (){CR.MINC = true; return *this;}
   DMA_stream& inc_periph   (){CR.PINC = true; return *this;}
   DMA_stream& circular_mode(){CR.CIRC = true; return *this;}

   DMA_stream& set_memory_adr      (size_t v)  {M0AR  = v; return *this;}
   DMA_stream& set_periph_adr      (size_t v)  {PAR  = v; return *this;}
   DMA_stream& set_qty_transactions(uint16_t v){NDTR = v; return *this;}

   DMA_stream& direction  (DataDirection d){CR.DIR = d;   return *this;}
   DMA_stream& size_memory(DataSize d)     {CR.MSIZE = d; return *this;}
   DMA_stream& size_periph(DataSize d)     {CR.PSIZE = d; return *this;}

   static constexpr IRQn_Type IRQn (Periph);

   DMA_stream& enable_transfer_complete_interrupt(){CR.TCIE = true; return *this;}

   uint16_t qty_transactions_left(){return NDTR;}

   

};

#if not defined(USE_MOCK_DMA_STREAM)
SFINAE(DMA1_stream0,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream0_BASE);}
SFINAE(DMA1_stream1,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream1_BASE);}
SFINAE(DMA1_stream2,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream2_BASE);}
SFINAE(DMA1_stream3,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream3_BASE);}
SFINAE(DMA1_stream4,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream4_BASE);}
SFINAE(DMA1_stream5,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream5_BASE);}
SFINAE(DMA1_stream6,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream6_BASE);}
SFINAE(DMA1_stream7,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream7_BASE);}
SFINAE(DMA2_stream0,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream0_BASE);}
SFINAE(DMA2_stream1,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream1_BASE);}
SFINAE(DMA2_stream2,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream2_BASE);}
SFINAE(DMA2_stream3,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream3_BASE);}
SFINAE(DMA2_stream4,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream4_BASE);}
SFINAE(DMA2_stream5,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream5_BASE);}
SFINAE(DMA2_stream6,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream6_BASE);}
SFINAE(DMA2_stream7,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream7_BASE);}
#endif


constexpr IRQn_Type DMA_stream::IRQn(Periph v)
{
   return
      v == Periph::DMA1_stream0 ? DMA1_Stream0_IRQn :
      v == Periph::DMA1_stream1 ? DMA1_Stream1_IRQn :
      v == Periph::DMA1_stream2 ? DMA1_Stream2_IRQn :
      v == Periph::DMA1_stream3 ? DMA1_Stream3_IRQn :
      v == Periph::DMA1_stream4 ? DMA1_Stream4_IRQn :
      v == Periph::DMA1_stream5 ? DMA1_Stream5_IRQn :
      v == Periph::DMA1_stream6 ? DMA1_Stream6_IRQn :
      v == Periph::DMA1_stream7 ? DMA1_Stream7_IRQn :
      v == Periph::DMA2_stream0 ? DMA2_Stream0_IRQn :
      v == Periph::DMA2_stream1 ? DMA2_Stream1_IRQn :
      v == Periph::DMA2_stream2 ? DMA2_Stream2_IRQn :
      v == Periph::DMA2_stream3 ? DMA2_Stream3_IRQn :
      v == Periph::DMA2_stream4 ? DMA2_Stream4_IRQn :
      v == Periph::DMA2_stream5 ? DMA2_Stream5_IRQn :
      v == Periph::DMA2_stream6 ? DMA2_Stream6_IRQn :
      v == Periph::DMA2_stream7 ? DMA2_Stream7_IRQn :
      NonMaskableInt_IRQn;
}



} //namespace mcu
