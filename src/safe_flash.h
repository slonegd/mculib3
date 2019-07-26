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
class Safe_flash : public Data, private TickSubscriber {
public:
    Safe_flash();
    ~Safe_flash() { tick_unsubscribe(); }

private:
    void notify() override {}
};



template <class Data, FLASH_::Sector first, FLASH_::Sector last>
Safe_flash<Data,first,last>::Safe_flash()
{
    constexpr auto sectors_qty = last - first + 1;
    static_assert(
          (sectors_qty % 6) == 0
        , "Количество секторов должно быть кратно трём"
    );
}