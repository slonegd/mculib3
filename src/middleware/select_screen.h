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
        enter_publisher ([this]{ enter(); });
        out_publisher   ([this]{ out();   });

        // отрисовка
        auto begin_line = lines.begin() + line_n;
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

    void deinit() override {
        up_publisher    ([]{});
        down_publisher  ([]{});
        enter_publisher ([]{});
        out_publisher   ([]{});
    }

    void enter() { lines[line_n].callback(); }
    void out()   { out_callback(); }

    void up() {

        // carriage_v--;
        // scroll--;

        // if (scroll < 0) {
        //     carriage--; 
        //     if (carriage < 0)
        //         scroll = qty - 4;
        //     else 
        //         scroll = 0;
        // }
        // if (carriage < 0) carriage = 3;
        // if (carriage_v < 0) carriage_v = qty -1;
    }

    void down() {
        // carriage_v++;
        // scroll++;
        
        // if (carriage_v >= qty) carriage_v = 0;
        // if (scroll > qty - 4) {
        //     carriage++;
        //     if (carriage > 3)
        //         scroll = 0; 
        //     else 
        //         scroll = qty - 4; 
        // }
        // if (carriage > 3) carriage = 0;
    }

    // FIX нет смысла чистить экран, если ничего не изменилось
    // возможно надо добавить флаг, на перерисовку
    // либо перерисовывать прямо в методах up и down
    void draw() override
    {
        // lcd.clear();

        // std::for_each (lines.begin(), lines.end(), [&,i=0](auto& line)mutable{
        //     lcd.line(i) << line.name << next_line;
        //     if (i == carriage)
        //         lcd.line(i).cursor(19) << "~";
        //     i++;
        // });
          
        // for (auto i {0}; i < 4; i++) {
        //     if (scroll + i < int(lines.size()))
        //         lcd.line(i) << lines[scroll + i].name;
        //     lcd.line(i).cursor(19) << " ";
        //     if (i == carriage)
        //         lcd.line(i).cursor(19) << "~";
        // }
    }
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
    // int scroll          {0};
};


