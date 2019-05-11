#pragma once

#include "function.h"

struct Screen {
    virtual void init()   = 0; // первичная отрисовка + инит кнопок
    virtual void draw()   = 0; // текущие данные 
    virtual void deinit() = 0; // деинит кнопок
};

template<class T, size_t n = 0> // n for unique with same T
struct Construct_wrapper {
    using type = T;
    T value;
    explicit Construct_wrapper (T value) : value{value} {}
};

// struct Eventer {
//     Function<void(Callback<>)> event;
//     void set_callback (Callback<> v) { event(v); }
//     void clear_callback() {event = nullptr;}
// };

using Eventer = Function<void(Callback<>)>;

using Up_event     = Construct_wrapper<Eventer>;
using Down_event   = Construct_wrapper<Eventer, 1>;
using Enter_event  = Construct_wrapper<Eventer, 2>;
using Out_event    = Construct_wrapper<Eventer, 3>;
using Out_callback = Construct_wrapper<Callback<>>;