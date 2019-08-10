#pragma once

#include "function.h"
#include <functional>

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

using Eventer = std::function<void(Callback<>)>;
using Increment_eventer = std::function<void(Callback<int>)>;

using Up_event       = Construct_wrapper<Eventer>;
using Down_event     = Construct_wrapper<Eventer, 1>;
using Enter_event    = Construct_wrapper<Eventer, 2>;
using Out_event      = Construct_wrapper<Eventer, 3>;
using Out_callback   = Construct_wrapper<Callback<>>;
using Enter_callback = Construct_wrapper<Callback<>, 1>;

using Increment_up_event   = Construct_wrapper<Increment_eventer>;
using Increment_down_event = Construct_wrapper<Increment_eventer,1>;

struct Buttons_events {
    Eventer up;
    Eventer down;
    Eventer enter;
    Eventer out;

    Increment_eventer increment_up;
    Increment_eventer increment_down;

    Buttons_events (
          Up_event    up
        , Down_event  down
        , Enter_event enter
        , Out_event   out
        , Increment_up_event   increment_up
        , Increment_down_event increment_down
    ) : up    {up.value}
      , down  {down.value}
      , enter {enter.value}
      , out   {out.value}
      , increment_up   {increment_up.value}
      , increment_down {increment_down.value}
    {}
};