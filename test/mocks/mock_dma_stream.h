#pragma once

#define USE_MOCK_DMA
#include "mock_rcc.h"
#include "process.h"

namespace mock {

std::ostream& operator<< (std::ostream& s, mcu::DMA_stream::DataDirection v)
{
   return
      v == mcu::DMA_stream::DataDirection::MemToPer ? s << "из памяти в переферию"   :
      v == mcu::DMA_stream::DataDirection::PerToMem ? s << "из переферии в память" : s;
}

std::ostream& operator<< (std::ostream& s, mcu::DMA_stream::DataSize v)
{
   return
      v == mcu::DMA_stream::DataSize::byte8   ? s << "байт (8бит)"            :
      v == mcu::DMA_stream::DataSize::word16  ? s << "слово (16бит)"          :
      v == mcu::DMA_stream::DataSize::dword32 ? s << "двойное слово (32бита)" : s;
}



class DMA_stream : public mcu::DMA_stream
{
   Process& process { Process::make() };
   DMA_stream() = default;
public:
   template<mcu::Periph p> static DMA_stream& make()
   {
      static DMA_stream stream;
      return stream;
   }
   friend std::ostream& operator<< (std::ostream& s, const DMA_stream& v);

   auto& base() { return *static_cast<mcu::DMA_stream*>(this); }

   DMA_stream& enable()
   {
      process << *this << ": Разрешение работы" << std::endl;
      base().enable();
      return *this;     
   }

   DMA_stream& disable()
   {
      process << *this << ": Запрет работы" << std::endl;
      base().disable();
      return *this;     
   }

   DMA_stream& inc_memory()
   {
      process << *this << ": Установка инкремента адреса памяти" << std::endl;
      base().inc_memory();
      return *this;     
   }

   DMA_stream& inc_periph()
   {
      process << *this << ": Установка инкремента адреса переферии" << std::endl;
      base().inc_periph();
      return *this;     
   }

   DMA_stream& circular_mode()
   {
      process << *this << ": Установка кольцевого режима" << std::endl;
      base().circular_mode();
      return *this;
   }

   DMA_stream& set_memory_adr (size_t v)
   {
      process << *this << ": Установка адреса памяти: " << v << std::endl;
      base().set_memory_adr(v);
      return *this;
   }

   DMA_stream& set_periph_adr (size_t v)
   {
      process << *this << ": Установка адреса переферии: " << v << std::endl;
      base().set_periph_adr(v);
      return *this;
   }

   DMA_stream& set_qty_transactions (uint16_t v)
   {
      process << *this << ": Установка количества передач данных: " << v << std::endl;
      base().set_qty_transactions(v);
      return *this;
   }

   DMA_stream& direction (DataDirection v)
   {
      process << *this << ": Установка направления " << v << std::endl;
      base().direction(v);
      return *this;
   }

   DMA_stream& size_memory (DataSize v)
   {
      process << *this << ": Установка размера данных в памяти " << v << std::endl;
      base().size_memory(v);
      return *this;
   }

   DMA_stream& size_periph (DataSize v)
   {
      process << *this << ": Установка размера данных в переферии " << v << std::endl;
      base().size_memory(v);
      return *this;
   }

   DMA_stream& enable_transfer_complete_interrupt()
   {
      process << *this << ": Разрешение прерывания по концу передачи данных" << std::endl;
      base().enable_transfer_complete_interrupt();
      return *this;
   }

};

std::ostream& operator<< (std::ostream& s, const DMA_stream& v)
{
   return
      &v == &DMA_stream::make<mcu::Periph::DMA1_stream1>() ? s << "DMA1_stream1" :
      &v == &DMA_stream::make<mcu::Periph::DMA1_stream2>() ? s << "DMA1_stream2" :
      &v == &DMA_stream::make<mcu::Periph::DMA1_stream3>() ? s << "DMA1_stream3" :
      &v == &DMA_stream::make<mcu::Periph::DMA1_stream4>() ? s << "DMA1_stream4" :
      &v == &DMA_stream::make<mcu::Periph::DMA1_stream5>() ? s << "DMA1_stream5" :
   #if defined(STM32F1) or defined(STM32F4) or defined(STM32F7)
      &v == &DMA_stream::make<mcu::Periph::DMA1_stream5>() ? s << "DMA1_stream6" :
      &v == &DMA_stream::make<mcu::Periph::DMA1_stream5>() ? s << "DMA1_stream7" :
   #endif
   #if defined(STM32F4) or defined(STM32F7)
      &v == &DMA_stream::make<mcu::Periph::DMA1_stream0>() ? s << "DMA1_stream0" :
      &v == &DMA_stream::make<mcu::Periph::DMA2_stream0>() ? s << "DMA2_stream0" :
      &v == &DMA_stream::make<mcu::Periph::DMA2_stream1>() ? s << "DMA2_stream1" :
      &v == &DMA_stream::make<mcu::Periph::DMA2_stream2>() ? s << "DMA2_stream2" :
      &v == &DMA_stream::make<mcu::Periph::DMA2_stream3>() ? s << "DMA2_stream3" :
      &v == &DMA_stream::make<mcu::Periph::DMA2_stream4>() ? s << "DMA2_stream4" :
      &v == &DMA_stream::make<mcu::Periph::DMA2_stream5>() ? s << "DMA2_stream5" :
      &v == &DMA_stream::make<mcu::Periph::DMA2_stream6>() ? s << "DMA2_stream6" :
      &v == &DMA_stream::make<mcu::Periph::DMA2_stream7>() ? s << "DMA2_stream7" :
   #endif
      s;
}

} // namespace mock {

namespace mcu {
   SFINAE(DMA1_stream1,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA1_stream1>();}
   SFINAE(DMA1_stream2,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA1_stream2>();}
   SFINAE(DMA1_stream3,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA1_stream3>();}
   SFINAE(DMA1_stream4,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA1_stream4>();}
   SFINAE(DMA1_stream5,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA1_stream5>();}
#if defined(STM32F1) or defined(STM32F4) or defined(STM32F7)  
   SFINAE(DMA1_stream6,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA1_stream6>();}
   SFINAE(DMA1_stream7,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA1_stream7>();}
#endif
#if defined(STM32F4) or defined(STM32F7)  
   SFINAE(DMA1_stream0,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA1_stream0>();}
   SFINAE(DMA2_stream0,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA2_stream0>();}
   SFINAE(DMA2_stream1,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA2_stream1>();}
   SFINAE(DMA2_stream2,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA2_stream2>();}
   SFINAE(DMA2_stream3,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA2_stream3>();}
   SFINAE(DMA2_stream4,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA2_stream4>();}
   SFINAE(DMA2_stream5,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA2_stream5>();}
   SFINAE(DMA2_stream6,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA2_stream6>();}
   SFINAE(DMA2_stream7,mock::DMA_stream) make_reference() {return mock::DMA_stream::make<mcu::Periph::DMA2_stream7>();}
#endif
}

namespace mock {
   auto& dma1_stream1 = REF(DMA1_stream1);
   auto& dma1_stream2 = REF(DMA1_stream2);
   auto& dma1_stream3 = REF(DMA1_stream3);
   auto& dma1_stream4 = REF(DMA1_stream4);
   auto& dma1_stream5 = REF(DMA1_stream5);
#if defined(STM32F1) or defined(STM32F4) or defined(STM32F7) 
   auto& dma1_stream6 = REF(DMA1_stream6);
   auto& dma1_stream7 = REF(DMA1_stream7);
#endif
#if defined(STM32F4) or defined(STM32F7)  
   auto& dma1_stream0 = REF(DMA1_stream0);
   auto& dma2_stream0 = REF(DMA2_stream0);
   auto& dma2_stream1 = REF(DMA2_stream1);
   auto& dma2_stream2 = REF(DMA2_stream2);
   auto& dma2_stream3 = REF(DMA2_stream3);
   auto& dma2_stream4 = REF(DMA2_stream4);
   auto& dma2_stream5 = REF(DMA2_stream5);
   auto& dma2_stream6 = REF(DMA2_stream6);
   auto& dma2_stream7 = REF(DMA2_stream7);
#endif
}