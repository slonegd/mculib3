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
        , Out_callback     out_callback
        , std::string_view name
        , T& var
        , Min<T> min = Min<T>{std::numeric_limits<T>::min}
        , Max<T> max = Max<T>{std::numeric_limits<T>::max}
    ) : lcd          {lcd} 
      , eventers     {eventers}
      , out_callback {out_callback.value}
      , name         {name}
      , var          {var}
      , tmp          {var}
      , min          {min.value}
      , max          {max.value}
    {}

    void init() override {
        eventers.up    ([this]{ up();    });
        eventers.down  ([this]{ down();  });
        eventers.enter ([this]{ var = tmp; out_callback(); });
        eventers.out   ([this]{ out_callback(); });
        lcd.line(0) << name << next_line;
        if (to_string != null_to_string)
            lcd.line(1) << to_string(tmp) << next_line;
        else
            lcd << tmp << next_line;
        lcd << "Нажатие   " << "~" << "Сохран." << next_line;
        lcd << "Удержание " << "~" << "Отмена"  << next_line;
    }

    void deinit() override {
        eventers.up    (null_function);
        eventers.down  (null_function);
        eventers.enter (null_function);
        eventers.out   (null_function);
    }

    void draw() override {}

private:
    String_buffer& lcd;
    Buttons_events eventers;
    Callback<>     out_callback;
    const std::string_view name;
    T& var;
    T tmp;
    T min;
    T max;

    void down() 
    {
        --tmp;
        if (tmp < min)
            tmp = max;
        if (to_string != null_to_string) {
            lcd.line(1) << to_string(tmp) << next_line;
            return;
        }
        lcd.line(1) << tmp << next_line;
    }

    void up() 
    {
        ++tmp;
        if (tmp > max)
            tmp = min;
        if (to_string != null_to_string) {
            lcd.line(1) << to_string(tmp) << next_line;
            return;
        }
        lcd.line(1) << tmp << next_line;
    }

};



