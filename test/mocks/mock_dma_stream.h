#pragma once

#define USE_MOCK_DMA
#include "periph_dma.h"
#include <iostream>

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
   std::ostream* process {nullptr};
   DMA_stream() = default;
public:
   template<mcu::Periph p>
   static DMA_stream& make()
   {
      static DMA_stream stream;
      return stream;
   }
   void set_stream (std::ostream& s) { process = &s; }

   #define BASE static_cast<mcu::DMA_stream*>(this)

   DMA_stream& enable()
   {
      if (process) *process << *this << ": Разрешение работы" << std::endl;
      BASE->enable();
      return *this;     
   }

   DMA_stream& disable()
   {
      if (process) *process << *this << ": Запрет работы" << std::endl;
      BASE->disable();
      return *this;     
   }

   DMA_stream& inc_memory()
   {
      if (process) *process << *this << ": Установка инкремента адреса памяти" << std::endl;
      BASE->inc_memory();
      return *this;     
   }

   DMA_stream& inc_periph()
   {
      if (process) *process << *this << ": Установка инкремента адреса переферии" << std::endl;
      BASE->inc_periph();
      return *this;     
   }

   DMA_stream& circular_mode()
   {
      if (process) *process << *this << ": Установка кольцевого режима" << std::endl;
      BASE->circular_mode();
      return *this;
   }

   DMA_stream& set_memory_adr (size_t v)
   {
      if (process) *process << *this << ": Установка адреса памяти: " << v << std::endl;
      BASE->set_memory_adr(v);
      return *this;
   }

   DMA_stream& set_periph_adr (size_t v)
   {
      if (process) *process << *this << ": Установка адреса переферии: " << v << std::endl;
      BASE->set_periph_adr(v);
      return *this;
   }

   DMA_stream& set_qty_transactions (uint16_t v)
   {
      if (process) *process << *this << ": Установка количества передач данных: " << v << std::endl;
      BASE->set_qty_transactions(v);
      return *this;
   }

   DMA_stream& direction (DataDirection v)
   {
      if (process) *process << *this << ": Установка направления " << v << std::endl;
      BASE->direction(v);
      return *this;
   }

   DMA_stream& size_memory (DataSize v)
   {
      if (process) *process << *this << ": Установка размера данных в памяти " << v << std::endl;
      BASE->size_memory(v);
      return *this;
   }

   DMA_stream& size_periph (DataSize v)
   {
      if (process) *process << *this << ": Установка размера данных в переферии " << v << std::endl;
      BASE->size_memory(v);
      return *this;
   }

   DMA_stream& enable_transfer_complete_interrupt()
   {
      if (process) *process << *this << ": Расрешение прерывания по концу передачи данных" << std::endl;
      BASE->enable_transfer_complete_interrupt();
      return *this;
   }

   #undef BASE
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
   SFINAE(DMA1_stream1,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream1_BASE);}
   SFINAE(DMA1_stream2,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream2_BASE);}
   SFINAE(DMA1_stream3,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream3_BASE);}
   SFINAE(DMA1_stream4,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream4_BASE);}
   SFINAE(DMA1_stream5,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream5_BASE);}
#if defined(STM32F1) or defined(STM32F4) or defined(STM32F7)  
   SFINAE(DMA1_stream6,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream6_BASE);}
   SFINAE(DMA1_stream7,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream7_BASE);}
#endif
#if defined(STM32F4) or defined(STM32F7)  
   SFINAE(DMA1_stream0,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Stream0_BASE);}
   SFINAE(DMA2_stream0,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream0_BASE);}
   SFINAE(DMA2_stream1,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream1_BASE);}
   SFINAE(DMA2_stream2,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream2_BASE);}
   SFINAE(DMA2_stream3,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream3_BASE);}
   SFINAE(DMA2_stream4,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream4_BASE);}
   SFINAE(DMA2_stream5,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream5_BASE);}
   SFINAE(DMA2_stream6,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream6_BASE);}
   SFINAE(DMA2_stream7,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA2_Stream7_BASE);}
#endif
}