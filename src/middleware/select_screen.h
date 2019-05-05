#pragma once
#include <utility>
#include "button_old.h"
#include "function.h"
#include "string_buffer.h"

template<class T, size_t n = 0> // n for unique with same T
struct Construct_wrapper {
    using type = T;
    T value;
    explicit Construct_wrapper (T value) : value{value} {}
};

using Out_callback = Construct_wrapper<Callback<>>;

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
          Button& up
        , Button& down
        , String_buffer& lcd
        , Out_callback out_callback 
        , Line ... lines
    ) : up           {up}
      , down         {down}
      , lcd          {lcd}
      , out_callback {out_callback.value}
      , lines        {lines...}
    {}

    void init() override {}
    void draw() override
    {
        if (next()) {
            lines[carriage_v].callback();
            return;
        }
        if (back()) {
            carriage = 0;
            carriage_v = 0;
            out_callback();
            return;
        }

        lcd.clear();

        if (qty <= 4) {
            for (int i = 0; i < qty; i++) {
                lcd.line(i) << lines[i].name;
                lcd.line(i).cursor(19) << " ";
                if (i == carriage)
                    lcd.line(i).cursor(19) << "~";
            }

            carriage_v = carriage =
                up.push()   ? carriage < 1        ? qty - 1  : carriage - 1 :
                down.push() ? carriage >= qty - 1 ? 0        : carriage + 1 : carriage;

        } else {
            
            for (int i = 0; i < 4; i++) {
                lcd.line(i) << lines[scroll + i].name;
                lcd.line(i).cursor(19) << " ";
                if (i == carriage)
                    lcd.line(i).cursor(19) << "~";
            }
            
            if (up.push()) {
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
            if (down.push()) {
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
        }





    }
private:
    Button&               up;
    Button&               down;
    String_buffer&        lcd;
    Function<void()>      out_callback;
    std::array<Line, qty> lines;
    std::array<int, 3>    n;
    int carriage   {0};
    int carriage_v {0};
    int scroll     {0};

    bool back(){return (up and down).push_long();}
    bool next(){return (up and down).click();}
    

};


