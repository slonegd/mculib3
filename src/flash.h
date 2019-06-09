#pragma once

#include <cstring> // std::memcpy
#include <algorithm>
#include <iterator>
#include "periph_flash.h"
#include "timers.h"

#if defined(USE_MOCK_FLASH)
using FLASH_ = mock::FLASH;
using mock::FLASH;
#elif defined(PERIPH_FLASH_H_)
using FLASH_ = mcu::FLASH;
using mcu::FLASH;
#endif


template<size_t n, class Int = size_t>
class SizedInt {
    Int value {0};
public:
    inline Int operator++(int)   { return value = (++value < n) ? value : 0; }
    inline Int operator--(int)   { return value = (value == 0) ? n : value - 1; }
    inline operator Int() const  { return value; }
    inline Int operator= (Int v) { return value = v; }
};

struct Pair {
    uint8_t offset; 
    uint8_t value;
    operator uint16_t() { return uint16_t(value) << 8 | offset; }
};

template<FLASH_::Sector sector>
struct Traits {
    static constexpr auto sector_size     {FLASH_::template size<sector>()};
    static constexpr auto words_in_sector {sector_size / sizeof(uint16_t)};
    union Memory {
        Pair     pair[sector_size/2];
        uint16_t word[sector_size/2];
    };
    static constexpr Memory& memory { *reinterpret_cast<Memory*>(FLASH_::template address<sector>()) };
    static SizedInt<Traits<sector>::words_in_sector>  memory_offset;
};


// для STM32F0 sector на самом деле page из refmanual
template <class Data, FLASH_::Sector ... sector>
class Flash : public Data, private TickSubscriber
{
public:
    Flash();
    ~Flash() { tick_unsubscribe(); }
private:
    FLASH_&        flash   {mcu::make_reference<mcu::Periph::FLASH>()};
    uint8_t* const original {reinterpret_cast<uint8_t*>(static_cast<Data*>(this))};
    uint8_t        copy[sizeof(Data)];
    static constexpr auto sectors {std::array{sector...}} ;

    
    enum State {
      check_changes,
      start_write,
      check_write,
      erase,
      check_erase,
      rewrite
    };
    State state {check_changes};
    State return_state {check_changes};
    volatile uint8_t writed_data; // TODO: проверить без volatile
    SizedInt<sizeof(Data), uint8_t> data_offset {};

    // возвращает true , если данные прочитаны
    //            false, если нет или данные не полные
    bool is_read();
    void notify() override;
};













template <class Data, typename FLASH_::Sector ... sector>
Flash<Data,sector...>::Flash()
{
    static_assert (
      sizeof(Data) < 255,
      "Размер сохраняемой структуры должен быть менее 255 байт"
    );
    static_assert (
      std::is_trivially_copyable_v<Data>,
      "Можно сохранять только тривиально копируемую структуру"
    );
    Traits<sectors[0]>::memory_offset = 0;
    // flash.lock(); // check if need
    if (not is_read())
      *static_cast<Data*>(this) = Data{};
    tick_subscribe();
}



template <class Data, typename FLASH_::Sector ... sector>
bool Flash<Data,sector...>::is_read()
{
    // обнуляем буфер перед заполнением
    std::fill (std::begin(copy), std::end(copy), 0xFF);

    // чтение данных в копию data в виде массива
    bool byte_readed[sizeof(Data)] {};
    for (auto& pair : Traits<sectors[0]>::memory.pair) {
        Traits<sectors[0]>::memory_offset++;
        if (pair.offset < sizeof(Data)) {
            copy[pair.offset] = pair.value;
            byte_readed[pair.offset] = true;
        } else if (pair.offset == 0xFF) {
            Traits<sectors[0]>::memory_offset--;
            break;
        }
    }

    if (Traits<sectors[0]>::memory_offset == 0) {
      state = start_write;
      return_state = rewrite;
      return false;
    }

    auto other_memory_cleared = std::all_of (
          std::begin(Traits<sectors[0]>::memory.word) + Traits<sectors[0]>::memory_offset
        , std::end(Traits<sectors[0]>::memory.word)
        , [](auto& word){ return word == 0xFFFF; }
    );
    if (not other_memory_cleared) {
        state = erase;
        return false;
    }

    auto all_readed = std::all_of (std::begin(byte_readed), std::end(byte_readed), [](auto& v){return v;});
    if (all_readed) {
        std::memcpy (original, copy, sizeof(copy));
        return true;
    } else {
        state = erase;
        return false;
    }
}



template <class Data, FLASH_::Sector ... sector>
void Flash<Data,sector...>::notify()
{
    // реализация автоматом
    switch (state) {

    case check_changes:
      if (original[data_offset] == copy[data_offset]) 
         data_offset++;
      else
         state = start_write;
      break;

    case start_write:
      if ( not flash.is_busy() and flash.is_lock() ) {
         flash.unlock()
              .set_progMode();
         #if defined(STM32F4) or defined(STM32F7)
            flash.set (FLASH_::ProgSize::x16)
                 .en_interrupt_endOfProg(); // без этого не работает
         #endif
         writed_data = original[data_offset];
         Traits<sectors[0]>::memory.word[Traits<sectors[0]>::memory_offset] = Pair{data_offset, writed_data};
         state = check_write;
      }
      break;

    case check_write:
      if ( flash.is_endOfProg() ) {
         flash.clear_flag_endOfProg()
              .lock();
         copy[data_offset] = writed_data;
         Traits<sectors[0]>::memory_offset++;
         if (Traits<sectors[0]>::memory_offset)
            state = return_state;
         else
            state = erase;
            
      }
      break;

    case erase:
      if ( not flash.is_busy() and flash.is_lock() ) {
         flash.unlock()
              .start_erase<sectors[0]>();
         state = check_erase;
      }
      break;

    case check_erase:
      if ( flash.is_endOfProg() ) {
         flash.clear_flag_endOfProg()
              .lock();
         memset (copy, 0xFF, sizeof(copy));
         Traits<sectors[0]>::memory_offset = 0;
         data_offset   = 0;
         state = start_write;
         return_state = rewrite;
      }
      break;

    case rewrite:
      if (data_offset++) {
         state = start_write;
      } else {
         state = check_changes;
         return_state = check_changes;
      }
      break;
    } // switch
}









