#pragma once

#include "flash.h"
#include "reflect_ext.h"

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
    Updater1 updater1;
    Updater2 updater2;
    Updater3 updater3;
    int current;
    void notify() override;
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
    : updater1 {}
    , updater2 {}
    , updater3 {}
{
    updater1.read_to(data);
    auto data2 = Data{};
    updater2.read_to(&data2);
    auto data3 = Data{};
    updater3.read_to(&data3);

    auto start = [&]{
        updater2.set_data(data);
        updater3.set_data(data);
        tick_subscribe();
        updater1.start();
        current = 1;
    };

    if (reflect::is_equal (*data, data2)) {
        start();
        return;
    }

    
    if (reflect::is_equal (*data, data3)) {
        start();
        return;
    }

    if (reflect::is_equal (data2, data3)) {
        std::memcpy(data, &data2, sizeof(Data));
        start();
        return;
    }

    // если нет равных, то всё попорчено
    *data = Data{};
    start();
}


template <class Data, class Updater1, class Updater2, class Updater3>
void Safe_flash_updater_impl<Data,Updater1,Updater2,Updater3>::notify()
{
    switch (current) {
        case 1:
            if (updater1.done()) {
                updater1.stop();
                updater2.start();
                current = 2;
            }
        break;
        case 2:
            if (updater2.done()) {
                updater2.stop();
                updater3.start();
                current = 3;
            }
        break;
        case 3:
            if (updater3.done()) {
                updater3.stop();
                updater1.start();
                current = 1;
            }
        break;
    }
}