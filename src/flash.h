#pragma once

// #include <string.h>
#include <algorithm>
#include <iterator>
#include "periph_flash.h"
#include "timers.h"


namespace mcu
{
   



// для STM32F0 sector на самом деле page из refmanual
template <class Data>//, class FLASH_, typename FLASH_::Sector sector>
class Flash : public Data, private TickSubscriber
{
   FLASH& flash;
   uint8_t* original = reinterpret_cast<uint8_t*>(static_cast<Data*>(this));
   uint8_t copy[sizeof(Data)];
   int32_t flashOffset {-1};
   bool needErase {false};
   struct Pair {
      uint8_t offset; 
      uint8_t value;
   };
   union Flash_t {
      Pair     pair;
      uint16_t word;
   };
   Flash_t* sector;
   size_t   size;
   enum State {
      CheckChanges,
      StartWrite,
      CheckWrite,
      Erase,
      CheckErase
   } state {CheckChanges};

   Flash (Data d) : Data{d} {}
   // возвращает true , если данные прочитаны
   //            false, если нет или данные не полные
   bool readFromFlash();
   void notify() override;

public:
   Flash (
        FLASH::Sector sector
      , FLASH& flash = make_reference<Periph::FLASH>()
   ) 
      : flash  {flash}
      , sector {reinterpret_cast<Flash_t*>FLASH::address(sector);}
      , size   {FLASH::size(sector)}
   {
      static_assert (
         sizeof(Data) < 255,
         "Размер сохраняемой структуры должен быть более 255 байт"
      );
      static_assert (
         std::is_trivially_copyable_v<Data>,
         "Можно сохранять только тривиально копируемую структуру"
      );
      flash.lock();
      if (not readFromFlash())
         Flash {Data{}};
      subscribe();
   }
};










template <class Data> bool Flash<Data>::readFromFlash()
{
   // обнуляем буфер перед заполнением
   memset (copy, 0xFF, sizeof(copy));

   // чтение данных в копию data в виде массива
   flashOffset = -1;
   bool indExist[sizeof(Data)] {};
   auto sector_ = *reinterpret_cast<Flash_t(*)[size]>(sector);
   for (auto& pair : sector_->pair) {
      flashOffset++;
      if (pair.offset < sizeof(Data)) {
         copy[pair.offset] = pair.value;
         indExist[pair.offset] = true;
      } else if (pair.offset == 0xFF) {
         break;
      }
   }

   // проверка остальной части сектора флэш
   if (not std::all_of (
              std::begin(sector_->word) + flashOffset
            , std::end(sector_->word)
            , [](auto& word){ return word == 0xFFFF; }
           )
   ) {
      needErase = true;
      return false;
   }

   // проверка, что все данные прочитаны
   if (std::all_of (std::begin(indExist), std::end(indExist), [](auto v){return v;})) {
      memcpy (original, copy, sizeof(copy));
      return true;
   } else {
      return false;
   }
}


/// ТУТ ЗАКОНЧИЛ
template <class Data> void Flash<Data>::notify()
{
   // реализация автоматом

   static volatile uint8_t dataWrite = 0;
   static uint8_t byteN = 0;

   switch (state) {

   case CheckChanges:
      if (needErase) {
         state = Erase;
      } else if (original[byteN] == copy[byteN]) {
         byteN++;
         if (byteN == sizeof(Data)) {
            byteN = 0;
         }
      } else {
         state = StartWrite;
      }
      break;

   case StartWrite:
      if ( not flash.is_busy() and flash.is_lock() ) {
         flash.unlock();
         flash.setProgMode();
         #if defined(STM32F4)
            flash.template set<flash.ProgSize::x16>();
            flash.endOfProgInterruptEn(); // уже не помню зачем это
         #endif

         dataWrite = original[byteN];
         flash->word[flashOffset] = (uint16_t)dataWrite << 8 | byteN;
         state = CheckWrite;
      }
      break;

   case CheckWrite:
      if ( flash.is_endOfProg() ) {
         flash.clearEndOfProgFlag();
         flash.lock();
         copy[byteN] = dataWrite;
         flashOffset++;
         if ( static_cast<size_t>(flashOffset) > sizeof(Flash_t) / sizeof(Pair) )
            needErase = true;
         state = CheckChanges;
      }
      break;

   case Erase:
      if ( not flash.is_busy() and flash.is_lock() ) {
         flash.unlock();
         flash.template startErase<sector>();
         state = CheckErase;
      }
      break;

   case CheckErase:
      if ( flash.is_endOfProg() ) {
         flash.clearEndOfProgFlag();
         flash.lock();
         needErase = false;
         memset (copy, 0xFF, sizeof(copy));
         flashOffset = 0;
         state = CheckChanges;
      }
      break;
   } // switch
}









} // mcu