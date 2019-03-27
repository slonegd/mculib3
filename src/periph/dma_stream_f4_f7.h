#pragma once

#include "bits_dma_stream_f4_f7.h"
#include "periph_dma.h"

namespace mcu {

class DMA_stream {
	volatile DMA_bits::CR CR;     // configuration register     
	volatile uint32_t     NDTR;   // number of data register    
	volatile uint32_t     PAR;    // peripheral address register
	volatile uint32_t     M0AR;   // memory 0 address register  
	volatile uint32_t     M1AR;   // memory 1 address register  
	volatile uint32_t     FCR;    // FIFO control register      

public:
	using CMSIS_type    = DMA_Stream_TypeDef;
	using DataSize      = DMA_bits::CR::DataSize;
	using Priority      = DMA_bits::CR::Priority;
	using Direction     = DMA_bits::CR::Direction;
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

	DMA_stream& set        (Direction d) {CR.DIR = d;   return *this;}
	DMA_stream& size_memory(DataSize  d) {CR.MSIZE = d; return *this;}
	DMA_stream& size_periph(DataSize  d) {CR.PSIZE = d; return *this;}

	DMA_stream& enable_transfer_complete_interrupt(){CR.TCIE = true; return *this;}

	uint16_t qty_transactions_left(){return NDTR;}

	template<Periph stream> static constexpr Periph dma_periph();
	template<Periph usart, Periph stream> static constexpr Channel channel();

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

template<Periph stream> constexpr Periph DMA_stream::dma_periph()
{
	if constexpr (stream == Periph::DMA1_stream0 or
					  stream == Periph::DMA1_stream1 or
					  stream == Periph::DMA1_stream2 or
					  stream == Periph::DMA1_stream3 or
					  stream == Periph::DMA1_stream4 or
					  stream == Periph::DMA1_stream5 or
					  stream == Periph::DMA1_stream6 or
					  stream == Periph::DMA1_stream7)
		return Periph::DMA1;
	else 
		return Periph::DMA2;
}

template<Periph periph, Periph stream> constexpr DMA_stream::Channel DMA_stream::channel() 
{
	if constexpr (stream == Periph::DMA1_stream0 or
					  stream == Periph::DMA1_stream1 or
					  stream == Periph::DMA1_stream2 or
					  stream == Periph::DMA1_stream3 or
					  stream == Periph::DMA1_stream4 or
					  stream == Periph::DMA1_stream5 or
					  stream == Periph::DMA1_stream6 or
					  stream == Periph::DMA1_stream7)
		return Channel::_4;
	else if constexpr (stream == Periph::DMA2_stream1)
		return Channel::_5;
	else if constexpr (stream == Periph::DMA2_stream2 or stream == Periph::DMA2_stream7) {
		if      constexpr (periph == Periph::USART1)
			return Channel::_4;
		else if constexpr (periph == Periph::USART6)
			return Channel::_5;
	} else if constexpr (stream == Periph::DMA2_stream5)
		return Channel::_4;
	else if constexpr (stream == Periph::DMA2_stream6) 
		return Channel::_5;

	//adc
	else if constexpr (stream == Periph::DMA2_stream0 or stream == Periph::DMA2_stream1 or 
							 stream == Periph::DMA2_stream2 or stream == Periph::DMA2_stream3 or
							 stream == Periph::DMA2_stream4)
		if constexpr (periph == Periph::ADC1)
			return Channel::_0;
		else if constexpr (periph == Periph::ADC2)
			return Channel::_1;
		else if constexpr (periph == Periph::ADC2)
			return Channel::_2;
}


} //namespace mcu
