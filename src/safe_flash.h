#pragma once

#include "flash.h"
#include "meta.h"

// Суть
// создать 3 объекта с одной структурой
// по очереди записывать изменения, но только всей структуры сразу
// как только один флэш отработал всю структуру переключаем на следующую
// 
// При инициализации читать структуры из всех трёх
// за действительное считать, если хотя бы в двух совпало
// если не совпало у всех структур, то делаем значение по умолчанию
template <class Data, class Updater1, class Updater2, class Updater3>
class Safe_flash_updater_impl : private TickSubscriber {
public:
    Safe_flash_updater_impl(Data*);
    ~Safe_flash_updater_impl() { tick_unsubscribe(); }

private:
    Data* data;
    Updater1 updater1;
    Updater2 updater2;
    Updater3 updater3;
    void notify() override {}
};

// используется только для decltype
template<class Data, FLASH_::Sector first, FLASH_::Sector ... sector>
Flash_updater_impl<Data,FLASH_::Sector(sector+first)...> type(std::integer_sequence<FLASH_::Sector,sector...>) {}

template<class Data, FLASH_::Sector first, int qty>
using Flash_updater_t = decltype(type<Data,first>(std::make_integer_sequence<FLASH_::Sector, FLASH_::Sector(qty)>()));

// хелпер, чтобы явно не указывать параметр шаблона Data в Safe_flash_updater_impl
template<FLASH_::Sector first, FLASH_::Sector last>
struct Safe_flash_updater {
    template<class Data>
    static auto make(Data* data) {
        constexpr auto sectors_qty = last - first + 1;
        static_assert(
            (sectors_qty % 3) == 0
            , "Количество секторов должно быть кратно трём"
        );
        static_assert(
            sectors_qty >= 6
            , "Количество секторов должно быть не менее 6"
        );
        using Updater1 = Flash_updater_t<Data,first,sectors_qty/3>;
        using Updater2 = Flash_updater_t<Data,FLASH_::Sector(first+sectors_qty/3),sectors_qty/3>;
        using Updater3 = Flash_updater_t<Data,FLASH_::Sector(first+sectors_qty/3*2),sectors_qty/3>;
        return Safe_flash_updater_impl<Data,Updater1,Updater2,Updater3>{data};
    }
};






template <class Data, class Updater1, class Updater2, class Updater3>
Safe_flash_updater_impl<Data,Updater1,Updater2,Updater3>::Safe_flash_updater_impl(Data* data)
    : data {data}
    , updater1 {data}
    , updater2 {}
    , updater3 {}
{
    auto data2 = Data{};
    updater2.read_to(&data2);
    if (meta::is_equal (*data, data2)) {
        updater2.set_data(data);
        updater3.set_data(data);
        return;
    }

    auto data3 = Data{};
    updater3.read_to(&data3);
    if (meta::is_equal (*data, data3)) {
        updater2.set_data(data);
        updater3.set_data(data);
        return;
    }

    if (meta::is_equal (data2, data3)) {
        std::memcpy(data, &data2, sizeof(Data));
        updater2.set_data(data);
        updater3.set_data(data);
        return;
    }

    // если нет равных, то всё попрочено
    *data = Data{};
    updater2.set_data(data);
    updater3.set_data(data);

}