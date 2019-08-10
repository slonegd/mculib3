#pragma once
#include <utility>
#include "string_buffer.h"
#include "screen_common.h"
#include <limits>

constexpr std::string_view null_to_string (int i) {return std::string_view{};}

template<class T>
using Max = Construct_wrapper<T>;

template<class T>
using Min = Construct_wrapper<T, 1>;

// to_string - функция, преобразующая объект типа T в строку для отображения на экране
template<class T, auto to_string = null_to_string>
class Set_screen : public Screen
{
public:
    Set_screen (
          String_buffer&   lcd
        , Buttons_events   eventers
        , std::string_view name
        , T& var
        , Min<T> min
        , Max<T> max
        , Out_callback     out_callback
        , Enter_callback   enter_callback = Enter_callback{nullptr}
    ) : min            {min.value}
      , max            {max.value}
      , lcd            {lcd} 
      , eventers       {eventers}
      , out_callback   {out_callback.value}
      , enter_callback {enter_callback.value}
      , name           {name}
      , var            {var}
      , tmp            {var}
      
    {}

    void init() override {
        eventers.up    ([this]{ up();   });
        eventers.down  ([this]{ down(); });
        eventers.increment_up   ([this](auto i){   up(i); });
        eventers.increment_down ([this](auto i){ down(i); });
        eventers.enter ([this]{
            var = tmp;
            if (enter_callback) {
                enter_callback();
                return;
            }
            out_callback();
        });
        eventers.out   ([this]{ out_callback(); });
        lcd.line(0) << name << next_line;
        tmp = var;
        if (to_string != null_to_string)
            lcd.line(1) << to_string(tmp) << next_line;
        else
            lcd << tmp << next_line;
        lcd << "Нажатие   " << '~' << "Сохран." << next_line;
        lcd << "Удержание " << '~' << "Отмена"  << next_line;
    }

    void deinit() override {
        eventers.up    (nullptr);
        eventers.down  (nullptr);
        eventers.enter (nullptr);
        eventers.out   (nullptr);

        eventers.increment_up  (nullptr);
        eventers.increment_down(nullptr);
    }

    void draw() override {}

    T min;
    T max;

private:
    String_buffer& lcd;
    Buttons_events eventers;
    Callback<>     out_callback;
    Callback<>     enter_callback;
    const std::string_view name;
    T& var;
    T tmp;

    void down (int increment = 1) 
    {
        tmp -= increment;
        if (tmp < min or tmp == std::numeric_limits<T>::max())
            tmp = max;
        if (to_string != null_to_string) {
            lcd.line(1) << to_string(tmp) << next_line;
            return;
        }
        lcd.line(1) << tmp << next_line;
    }

    void up (int increment = 1)
    {
        tmp += increment;
        if (tmp > max)
            tmp = min;
        if (to_string != null_to_string) {
            lcd.line(1) << to_string(tmp) << next_line;
            return;
        }
        lcd.line(1) << tmp << next_line;
    }
};

// additional deduction guide
template<class T, auto to_string>
Set_screen (
        String_buffer&   lcd
    , Buttons_events   eventers
    , std::string_view name
    , T& var
    , Min<T> min
    , Max<T> max
    , Out_callback     out_callback
    , Enter_callback   enter_callback = Enter_callback{nullptr}
) -> Set_screen<T, to_string>;


