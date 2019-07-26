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
    inline Int operator++(int)   { auto v = value; value = (++value < n) ? value : 0; return v; }
    inline Int operator++()      { return value = (++value < n) ? value : 0; }
    inline Int operator--(int)   { return value = (value == 0) ? n : value - 1; }
    inline operator Int() const  { return value; }
    inline Int operator= (Int v) { return value = v; }
};

struct Pair {
    uint8_t offset; 
    uint8_t value;
    operator uint16_t() { return uint16_t(value) << 8 | offset; }
};

union Word {
    Pair     pair;
    uint16_t data;
};

struct Memory {
    Word*  pointer;
    size_t size;
    Memory (Word* pointer, size_t size)
        :  pointer {pointer}
        ,  size {size}
    {}

    class Iterator {
        Word* p {nullptr};
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type        = Word;
        using difference_type   = size_t;
        using pointer           = const Word*;
        using reference         = Word;
        Iterator (Word* p) : p{p} {}
        Iterator() = default;
        operator Word*() { return p; }
        Word&        operator*  () const { return *p; }
        Word*        operator-> () const { return p; }
        bool         operator!= (const Iterator& other) const {return p != other.p; }
        Iterator& operator++ () {
            p++;
            return *this;
        }
        Iterator& operator+ (int v) {
            p += v;
            return *this;
        }
    };

    Iterator begin() {return Iterator{pointer};}
    Iterator end()   {return Iterator{pointer + size};}
};



// для STM32F0 sector на самом деле page из refmanual
template <class Data, FLASH_::Sector ... sector>
class Flash_updater : private TickSubscriber
{
public:
    Flash_updater(Data&);
    ~Flash_updater() { tick_unsubscribe(); }
private:
    Data&          data;
    FLASH_&        flash   {mcu::make_reference<mcu::Periph::FLASH>()};
    uint8_t* const original {reinterpret_cast<uint8_t*>(&data)}; // TODO нужно ли?
    uint8_t        copy[sizeof(Data)];
    static constexpr auto sectors {std::array{sector...}} ;
    SizedInt<sizeof...(sector)> current {};
    bool need_erase[sizeof...(sector)] {};
    std::array<Memory, sizeof...(sector)> memory;
    Memory::Iterator memory_offset{nullptr};

    
    enum State {
      check_changes,
      start_write,
      check_write,
      erase,
      check_erase,
      rewrite
    };
    volatile State state {check_changes};
    volatile State return_state {check_changes};
    volatile uint8_t writed_data; // TODO: проверить без volatile
    SizedInt<sizeof(Data), uint8_t> data_offset {};

    // возвращает true , если данные прочитаны
    //            false, если нет или данные не полные
    bool is_read();
    void notify() override;
    bool is_need_erase();
};












template <class Data, typename FLASH_::Sector ... sector>
Flash_updater<Data,sector...>::Flash_updater(Data& data)
    : data {data}
    , memory { std::array{
        Memory (
              reinterpret_cast<Word*>(FLASH_::template address<sector>())
            , FLASH_::template size<sector>() / 2
        )...
    }}
    , memory_offset {memory[0].begin()}
{
    static_assert (
        sizeof(Data) < 255,
        "Размер сохраняемой структуры должен быть менее 255 байт"
    );
    static_assert (
        std::is_trivially_copyable_v<Data>,
        "Можно сохранять только тривиально копируемую структуру"
    );
    // flash.lock(); // check if need
    if (not is_read())
        data = Data{};
    tick_subscribe();
}



template <class Data, typename FLASH_::Sector ... sector>
bool Flash_updater<Data,sector...>::is_read()
{
    // обнуляем буфер перед заполнением
    std::fill (std::begin(copy), std::end(copy), 0xFF);

    // чтение данных в копию data в виде массива и поиск пустой ячейки
    bool byte_readed[sizeof(Data)] {};
    auto is_all_readed = [&]{ 
        return std::all_of (std::begin(byte_readed), std::end(byte_readed), [](auto& v){return v;});
    };
    flash.unlock();
    for (size_t i{0}; i < memory.size(); i++) {
        memory_offset = std::find_if(memory[i].begin(), memory[i].end()
            , [&](auto& word) bool {
                auto& pair = word.pair;
                if (pair.offset < sizeof(Data)) {
                    copy[pair.offset] = pair.value;
                    byte_readed[pair.offset] = true;
                    return false;
                }
                return word.data == 0xFFFF;
            }
        );
        if (memory_offset == memory[i].begin()) {
            current = i;
            continue;
        }
        if (memory_offset != memory[i].end()) {
            current = i;
            if (is_all_readed())
                break;
        } else {
            need_erase[i] = true;
        }
    }

    // прочитали всё но так и не нашли пустую ячейку
    if (memory_offset == memory[current].end()) {
        need_erase[current] = true;
        current = 0;
        memory_offset = memory[current].begin();
    }

    auto all_readed = is_all_readed();
    if (all_readed) {
        std::memcpy (original, copy, sizeof(copy));
        return_state = check_changes;
    } else {
        need_erase[current] = true;
        return_state = rewrite;
    }

    // проверить все пустые страницы, что они действительно пустые
    for (size_t i{0}; i < memory.size(); i++) {
        if (not need_erase[i] and i != current) {
            need_erase[i] = std::any_of (memory[i].begin(), memory[i].end()
                , [](auto& word){ return word.data != 0xFFFF; }
            );
        }
        
    }

    if (std::any_of(std::begin(need_erase), std::end(need_erase), [](auto& v){return v;})) {
        state = erase;
    }

    flash.lock();
    return all_readed;
}



template <class Data, FLASH_::Sector ... sector>
void Flash_updater<Data,sector...>::notify()
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
            flash.unlock();
                flash.set_progMode();
            #if defined(STM32F4) or defined(STM32F7)
                flash.set (FLASH_::ProgSize::x16)
                    .en_interrupt_endOfProg(); // без этого не работает
            #endif
            writed_data = original[data_offset];
            memory_offset->data = Pair{data_offset, writed_data};
            state = check_write;
        }
        break;

    case check_write:
        if ( flash.is_endOfProg() ) {
            flash.clear_flag_endOfProg()
                .lock();
            copy[data_offset] = writed_data;
            if (++memory_offset != memory[current].end()) {
                state = return_state;
            } else {
                need_erase[current] = true;
                current++;
                memory_offset = memory[current].begin();
                data_offset = 0;
                state = start_write;
                return_state = rewrite;
            }
        }
        break;

    case erase:
        if ( not flash.is_busy() and flash.is_lock() ) {
            auto it = std::find(std::begin(need_erase), std::end(need_erase), true);
            if (it == std::end(need_erase)) {
                state = return_state;
                break;
            }
            auto i = std::distance(std::begin(need_erase), it);
            flash.unlock()
                .start_erase(sectors[i]);
            need_erase[i] = false;
            state = check_erase;
        }
    break;

    case check_erase:
        if ( flash.is_endOfProg() ) {
            flash.clear_flag_endOfProg()
                 .lock();
            state = is_need_erase() ? erase : check_changes;
        }
    break;

    case rewrite:
        if (++data_offset) {
            state = start_write;
        } else {
            state = is_need_erase() ? erase : check_changes;
            return_state = check_changes;
        }
    break;
    } // switch
}



template <class Data, FLASH_::Sector ... sector>
bool Flash_updater<Data,sector...>::is_need_erase() {
    return std::any_of(std::begin(need_erase), std::end(need_erase), [](auto& v){return v;});
}




