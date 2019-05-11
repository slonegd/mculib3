#pragma once
#include <utility>
#include "string_buffer.h"
#include "screen_common.h"



struct Line {
    std::string_view name;
    Callback<> callback;
};

template <int qty>
class Select_screen : public Screen
{
public:
    template <class...Line> Select_screen (
          String_buffer&  lcd
        , Up_event    up_event
        , Down_event  down_event
        , Enter_event enter_event
        , Out_event   out_event
        , Out_callback    out_callback
        , Line ... lines
    ) : lcd             {lcd} 
      , up_event    {up_event.value}
      , down_event  {down_event.value}
      , enter_event {enter_event.value}
      , out_event   {out_event.value}
      , out_callback    {out_callback.value}
      , lines           {lines...}
    {}

    void init() override {
        up_event    ([this]{ up();    });
        down_event  ([this]{ down();  });
        enter_event ([this]{ lines[line_n].callback(); });
        out_event   ([this]{ out_callback(); });
        redraw();
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
    std::array<Line, qty> lines;

    int carriage_line   {0};
    int line_n          {0};

    void down();
    void up();
    void redraw();
};




template <int qty>
void Select_screen<qty>::down()
{
    if (line_n == int(lines.size() - 1))
        return;

    ++line_n;

    if (
        lines.size() <= 4                  // каретка двигается всегда
        or carriage_line < 2               // не двигается только на третьей
        or line_n == int(lines.size() - 1) // двигается на последнюю линию
    ) {
        lcd.line(carriage_line).cursor(19) << " ";
        ++carriage_line;
        lcd.line(carriage_line).cursor(19) << "~";
        return;
    }

    // если не двигается каретка то двигается список
    redraw();
}


template <int qty>
void Select_screen<qty>::up()
{
    if (line_n == 0)
        return;

    --line_n;

    if (
        lines.size() <= 4             // каретка двигается всегда
        or carriage_line > 1          // не двигается только на первой
        or line_n == 0                // двигается на первую линию
    ) {
        lcd.line(carriage_line).cursor(19) << " ";
        --carriage_line;
        lcd.line(carriage_line).cursor(19) << "~";
        return;
    }

    // если не двигается каретка то двигается список
    redraw();
}


template <int qty>
void Select_screen<qty>::redraw()
{
    auto begin_line = lines.begin() + line_n - carriage_line;
    auto end_line   = std::min(begin_line + 4, lines.end());
    lcd.line(0);
    std::for_each (
            begin_line
        , end_line
        , [&] (auto& line) mutable {
            lcd << line.name << next_line;
    });
    for (auto i{lcd.get_line()}; i < 4; i++)
        lcd.line(i) << ' ' << next_line;
    lcd.line(carriage_line).cursor(19) << "~";
}