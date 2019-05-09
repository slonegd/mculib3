#pragma once
#include <utility>
#include "function.h"
#include "string_buffer.h"

template<class T, size_t n = 0> // n for unique with same T
struct Construct_wrapper {
    using type = T;
    T value;
    explicit Construct_wrapper (T value) : value{value} {}
};

using Up_publisher    = Construct_wrapper<Function<void(Callback<>)>>;
using Down_publisher  = Construct_wrapper<Function<void(Callback<>)>, 1>;
using Enter_publisher = Construct_wrapper<Function<void(Callback<>)>, 2>;
using Out_publisher   = Construct_wrapper<Function<void(Callback<>)>, 3>;
using Out_callback    = Construct_wrapper<Callback<>>;

struct Screen {
    virtual void init()   = 0; // первичная отрисовка + инит кнопок
    virtual void draw()   = 0; // текущие данные 
    virtual void deinit() = 0; // деинит кнопок
};

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
        , Up_publisher    up_publisher
        , Down_publisher  down_publisher
        , Enter_publisher enter_publisher
        , Out_publisher   out_publisher
        , Out_callback    out_callback
        , Line ... lines
    ) : lcd             {lcd} 
      , up_publisher    {up_publisher.value}
      , down_publisher  {down_publisher.value}
      , enter_publisher {enter_publisher.value}
      , out_publisher   {out_publisher.value}
      , out_callback    {out_callback.value}
      , lines           {lines...}
    {}

    void init() override {
        up_publisher    ([this]{ up();    });
        down_publisher  ([this]{ down();  });
        enter_publisher ([this]{ lines[line_n].callback(); });
        out_publisher   ([this]{ out_callback(); });
        redraw();
    }

    void deinit() override {
        up_publisher    (null_function);
        down_publisher  (null_function);
        enter_publisher (null_function);
        out_publisher   (null_function);
    }

    void draw() override {}

private:
    String_buffer&        lcd;
    Function<void(Callback<>)>    up_publisher;
    Function<void(Callback<>)>  down_publisher;
    Function<void(Callback<>)> enter_publisher;
    Function<void(Callback<>)>   out_publisher;
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