#pragma once

#include <cstring> // std::memcpy
#include <algorithm>
#include <iterator>
#include "periph_flash.h"
#include "timers.h"

template<size_t n, class Int = size_t>
class SizedInt {
   Int value {0};
public:
   inline Int operator++(int)   { return value = (++value < n) ? value : 0; }
   inline operator Int() const  { return value; }
   inline Int operator= (Int v) { return value = v; }
};

namespace mcu {

// для STM32F0 sector на самом деле page из refmanual
template <class Data, FLASH::Sector sector>
class Flash_impl : public Data, private TickSubscriber
{
public:
   Flash_impl();
private:
   static constexpr auto sector_size    {FLASH::template size<sector>()};
   static constexpr auto sector_address {FLASH::template address<sector>()};
   struct Pair {
      uint8_t offset; 
      uint8_t value;
   };
   union Memory {
      Pair     pair[sector_size/2];
      uint16_t word[sector_size/2];
   };
   Memory& memory {*reinterpret_cast<Memory*>(sector_address)};

   FLASH&         flash    {make_reference<Periph::FLASH>()};
   uint8_t* const original {reinterpret_cast<uint8_t*>(static_cast<Data*>(this))};
   uint8_t        copy[sizeof(Data)];

   SizedInt<sector_size / sizeof(Pair)>  memory_offset {};
   enum State {
      CheckChanges,
      StartWrite,
      CheckWrite,
      Erase,
      CheckErase
   } state {CheckChanges};
   volatile uint8_t writed_data; // TODO: проверить без volatile
   SizedInt<sizeof(Data), uint8_t> data_offset {};

   Flash_impl (Data d) : Data{d} {}
   // возвращает true , если данные прочитаны
   //            false, если нет или данные не полные
   bool is_read();
   void notify() override;
};


/// алиасинг для более быстрой записи
// template<class Data, typename FLASH::Sector sector>
// using Flash = Flash_impl<Data,FLASH,sector>;















template <class Data, typename FLASH::Sector sector>
Flash_impl<Data,sector>::Flash_impl()
{
   static_assert (
      sizeof(Data) < 255,
      "Размер сохраняемой структуры должен быть менее 255 байт"
   );
   static_assert (
      std::is_trivially_copyable_v<Data>,
      "Можно сохранять только тривиально копируемую структуру"
   );
   flash.lock();
   if (not is_read())
      Flash_impl {Data{}};
   subscribe();
}



template <class Data, typename FLASH::Sector sector>
bool Flash_impl<Data,sector>::is_read()
{
   // обнуляем буфер перед заполнением
   std::fill (std::begin(copy), std::end(copy), 0xFF);

   // чтение данных в копию data в виде массива
   bool byte_readed[sizeof(Data)] {};
   for (auto& pair : memory.pair) {
      memory_offset++;
      if (pair.offset < sizeof(Data)) {
         copy[pair.offset] = pair.value;
         byte_readed[pair.offset] = true;
      } else if (pair.offset == 0xFF) {
         break;
      }
   }

   auto other_memory_cleared = std::all_of (
        std::begin(memory.word) + memory_offset
      , std::end(memory.word)
      , [](auto& word){ return word == 0xFFFF; }
   );
   if (not other_memory_cleared) {
      state = Erase;
      return false;
   }

   auto all_readed = std::all_of (std::begin(byte_readed), std::end(byte_readed), [](auto& v){return v;});
   if (all_readed) {
      std::memcpy (original, copy, sizeof(copy));
      return true;
   } else {
      return false;
   }
}



template <class Data, FLASH::Sector sector>
void Flash_impl<Data,sector>::notify()
{
   // реализация автоматом
   switch (state) {

   case CheckChanges:
      if (original[data_offset] == copy[data_offset]) 
         data_offset++;
      else
         state = StartWrite;
      break;

   case StartWrite:
      if ( not flash.is_busy() and flash.is_lock() ) {
         flash.unlock()
              .set_progMode();
         #if defined(STM32F4)
            flash.set (FLASH::ProgSize::x16)
                 .en_interrupt_endOfProg(); // без этого не работает
         #endif
         writed_data = original[data_offset];
         memory.pair[memory_offset] = Pair{data_offset, writed_data};
         state = CheckWrite;
      }
      break;

   case CheckWrite:
      if ( flash.is_endOfProg() ) {
         flash.clear_flag_endOfProg()
              .lock();
         copy[data_offset] = writed_data;
         memory_offset++;
         if (memory_offset)
            state = CheckChanges;
         else
            state = Erase;
            
      }
      break;

   case Erase:
      if ( not flash.is_busy() and flash.is_lock() ) {
         flash.unlock()
              .start_erase (sector);
         state = CheckErase;
      }
      break;

   case CheckErase:
      if ( flash.is_endOfProg() ) {
         flash.clear_flag_endOfProg()
              .lock();
         memset (copy, 0xFF, sizeof(copy));
         memory_offset = 0;
         state = CheckChanges;
      }
      break;
   } // switch
}









} // namespace mcu {