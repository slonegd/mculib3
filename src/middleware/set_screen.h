#pragma once
#include <utility>
#include "string_buffer.h"
#include "screen_common.h"
#include <limits>

constexpr std::string_view null_to_string (int i) {return std::string_view{};}

// to_string - функция, преобразующая объект типа T в строку для отображения на экране
template<class T, auto to_string = null_to_string>
class Set_screen : public Screen
{
public:
    Set_screen (
          String_buffer&  lcd
        , Up_event    up_event
        , Down_event  down_event
        , Enter_event enter_event
        , Out_event   out_event
        , Out_callback    out_callback
        , std::string_view name
        , T& var
        , T  min = std::numeric_limits<T>::min
        , T  max = std::numeric_limits<T>::max
    ) : lcd          {lcd} 
      , up_event     {up_event.value}
      , down_event   {down_event.value}
      , enter_event  {enter_event.value}
      , out_event    {out_event.value}
      , out_callback {out_callback.value}
      , name         {name}
      , var          {var}
      , tmp          {var}
      , min          {min}
      , max          {max}
    {}

    void init() override {
        up_event    ([this]{ up();    });
        down_event  ([this]{ down();  });
        enter_event ([this]{ var = tmp; });
        out_event   ([this]{ out_callback(); });
        lcd.line(0) << name << next_line;
        lcd << tmp << next_line;
        lcd << "Нажатие   " << "~" << "Сохран." << next_line;
        lcd << "Удержание " << "~" << "Отмена"  << next_line;
    }

    void deinit() override {
        up_event    (null_function);
        down_event  (null_function);
        enter_event (null_function);
        out_event   (null_function);
    }

    void draw() override {}

private:
    String_buffer&        lcd;
    Eventer    up_event;
    Eventer  down_event;
    Eventer enter_event;
    Eventer   out_event;
    Callback<> out_callback;
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



