#pragma once

#include "flash.h"

// Суть
// создать 3 объекта с одной структурой
// по очереди записывать изменения, но только всей структуры сразу
// как только один флэш отработал всю структуру переключаем на следующую
// 
// При инициализации читать структуры из всех трёх
// за действительное считать, если хотя бы в двух совпало
// если не совпало у всех структур, то берём наименьшее значение
template <class Data, FLASH_::Sector first, FLASH_::Sector last>
class Safe_flash_updater_impl : private TickSubscriber {
public:
    Safe_flash_updater_impl(Data*);
    ~Safe_flash_updater_impl() { tick_unsubscribe(); }

private:
    Data* data;
    void notify() override {}
};


// хелпер, чтобы явно не указывать параметр шаблона Data в Safe_flash_updater_impl
template<FLASH_::Sector first, FLASH_::Sector last>
struct Safe_flash_updater {
    template<class Data>
    static auto make(Data* data) {
        return Safe_flash_updater_impl<Data,first,last>{data};
    }
};




template <class Data, FLASH_::Sector first, FLASH_::Sector last>
Safe_flash_updater_impl<Data,first,last>::Safe_flash_updater_impl(Data* data)
    : data {data}
    , 
{
    constexpr auto sectors_qty = last - first + 1;
    static_assert(
          (sectors_qty % 3) == 0
        , "Количество секторов должно быть кратно трём"
    );
    static_assert(
          sectors_qty >= 6
        , "Количество секторов должно быть не менее 6"
    );
}