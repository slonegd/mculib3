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
    virtual void init() = 0; // первичная отрисовка
    virtual void draw() = 0; // текущие данные 
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
    }

    void enter() { lines[carriage_v].callback(); }

    void out() { 
        carriage = 0;
        carriage_v = 0;
        out_callback();
        return;
    }

    void up() {
        carriage_v--;
        scroll--;

        if (scroll < 0) {
            carriage--; 
            if (carriage < 0)
                scroll = qty - 4;
            else 
                scroll = 0;
        }
        if (carriage < 0) carriage = 3;
        if (carriage_v < 0) carriage_v = qty -1;
    }

    void down() {
        carriage_v++;
        scroll++;
        
        if (carriage_v >= qty) carriage_v = 0;
        if (scroll > qty - 4) {
            carriage++;
            if (carriage > 3)
                scroll = 0; 
            else 
                scroll = qty - 4; 
        }
        if (carriage > 3) carriage = 0;
    }

    // FIX нет смысла чистить экран, если ничего не изменилось
    // возможно надо добавить флаг, на перерисовку
    // либо перерисовывать прямо в методах up и down
    void draw() override
    {
        lcd.clear();
          
        for (int i = 0; i < 4; i++) {
            lcd.line(i) << lines[scroll + i].name;
            lcd.line(i).cursor(19) << " ";
            if (i == carriage)
                lcd.line(i).cursor(19) << "~";
        }
    }
private:
    String_buffer&        lcd;
    Function<void(Callback<>)>    up_publisher;
    Function<void(Callback<>)>  down_publisher;
    Function<void(Callback<>)> enter_publisher;
    Function<void(Callback<>)>   out_publisher;
    Callback<> out_callback;
    std::array<Line, qty> lines;

    std::array<int, 3> n;
    int carriage   {0};
    int carriage_v {0};
    int scroll     {0};
};


