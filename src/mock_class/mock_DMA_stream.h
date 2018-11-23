#pragma once

#include <type_traits>
#include "test_periph.h"
#include "string_result.h"

namespace mcu {

class DMA_stream 
{
   enum Stream {TX_stream, RX_stream};
   static int counter;
   Stream stream;
   
public:
   enum DataSize      { byte8 = 0b00, word16, dword32 };
   enum Priority      { low = 0b00, medium, high, veryHi = 0b11 };
   enum DataDirection { PerToMem = 0b0, MemToPer };
   
   DMA_stream(){
      ++counter; 
      if      (counter == 1) stream = TX_stream;
      else if (counter == 2) stream = RX_stream;
   }

   template <Periph dma = Periph::TEST_DMA> DMA_stream& clock_enable(){result << "Включаем тактирование переферии DMA" << '\n'; return *this;}

   DMA_stream& is_transfer_complete_interrupt(){return *this;}
   
   DMA_stream& enable()
   {
      stream == TX_stream ? result << "Разрешаем работу TX_stream" << '\n':
      stream == RX_stream ? result << "Разрешаем работу RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& disable()
   {
      stream == TX_stream ? result << "Запрещаем работу TX_stream" << '\n':
      stream == RX_stream ? result << "Запрещаем работу RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& direction(DataDirection d)
   {
      stream == TX_stream ? (
         d == DataDirection::MemToPer ? result << "Задаем направление TX_stream в переферию" << '\n' :
         d == DataDirection::PerToMem ? result << "Задаем направление TX_stream в память" << '\n' :
         result << "Невыбрано направление"     << '\n') :
      stream == RX_stream ? (
         d == DataDirection::MemToPer ? result << "Задаем направление RX_stream в переферию" << '\n' :
         d == DataDirection::PerToMem ? result << "Задаем направление RX_stream в память" << '\n' :
         result << "Невыбрано направление"     << '\n') :
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& set_memory_adr(uint32_t)
   {
      stream == TX_stream ? result << "Задаем адрес памяти TX_stream" << '\n':
      stream == RX_stream ? result << "Задаем адрес памяти RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
       return *this;
   }
   DMA_stream& set_periph_adr(uint32_t)
   {
      stream == TX_stream ? result << "Задаем адрес переферии TX_stream" << '\n':
      stream == RX_stream ? result << "Задаем адрес переферии RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& inc_memory()
   {
      stream == TX_stream ? result << "Инкременнтируем адрес памяти TX_stream" << '\n':
      stream == RX_stream ? result << "Инкременнтируем адрес памяти RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& size_memory(DataSize v)
   {
      v == DataSize::byte8 ? (
         stream == TX_stream ? result << "Задаем размер для памяти TX_stream 8 бит" << '\n':
         stream == RX_stream ? result << "Задаем размер для памяти RX_stream 8 бит" << '\n':
         result << "Невыбран поток"   << '\n') :
      result << "Невыбран размер"  << '\n';
      return *this;
   }
   DMA_stream& size_periph(DataSize v)
   {
      v == DataSize::byte8 ? (
         stream == TX_stream ? result << "Задаем размер для переферии TX_stream 8 бит" << '\n':
         stream == RX_stream ? result << "Задаем размер для переферии RX_stream 8 бит" << '\n':
         result << "Невыбран поток"   << '\n') :
      result << "Невыбран размер"  << '\n';
      return *this;
   }
   DMA_stream& enable_transfer_complete_interrupt()
   {
      stream == TX_stream ? result << "Разрешаем прерывание TX_stream по окончании отправки" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& set_qty_transactions(uint16_t v)
   {
      stream == TX_stream ? result << "Устанавливаем колв-во транзакций для TX_stream" << '\n':
      stream == RX_stream ? result << "Устанавливаем колв-во транзакций для RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;}

   IRQn_Type IRQn(Periph dma_stream)
   {
      if (stream == TX_stream) {
         result << "Определяем номер прерывания TX_stream" << '\n'; return DMA1_Channel1_IRQn;
      } else if (stream == RX_stream) {
         result << "Определяем номер прерывания RX_stream" << '\n'; return DMA1_Channel2_IRQn;
      }

      return Error;
   }

   uint16_t qty_transactions_left()
   {
      return 0;
   }
};

int DMA_stream::counter{0};

DMA_stream tx_stream;
DMA_stream rx_stream;

template <Periph dma_stream> std::enable_if_t<dma_stream == Periph::TEST_DMA_stream1, DMA_stream&> make_reference()
{
   result << "Создали ссылку на переферию tx stream" << '\n';
   return tx_stream;
}
template <Periph dma_stream> std::enable_if_t<dma_stream == Periph::TEST_DMA_stream2, DMA_stream&> make_reference() 
{
   result << "Создали ссылку на переферию rx stream" << '\n';
   return rx_stream;
}





} //namespace mcu