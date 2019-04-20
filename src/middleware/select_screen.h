#pragma once
#include <utility>
#include "buttons.h"
#include "function.h"
#include "string_buffer.h"

struct Line {
    std::string_view name;
    Function<void()> callback;
};

template <int qty>
class Screen
{
public:
    template <class...Line> Screen (
          Button& up
        , Button& down
        , String_buffer& lcd
        , Line ... lines
    ) : up    {up}
      , down  {down}
      , lcd   {lcd}
      , lines {lines...}
    {}

    void operator() (Function<void()> f)
    {
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


        // if (qty <= 3) {
        //    for (int i = 0; i < qty; i++) {
        //       lcd.line(i) << lines[i].name;
        //    }
        // } else if (carriage <= 3) {
        //    for (int i = 0; i < 4; i++) {
        //       lcd.line(i) << lines[i].name;
        //    }
        // } else {
        //    for (int i = 0; i < 4; i++) {
        //       n[i] = carriage + (i - 3);
        //    }
        //    for (int i = 0; i < 4; i++) {
        //       lcd.line(i) << lines[n[i]].name;
        //    }
        // }

        // carriage = up.push()   ? carriage < 1 ? qty - 1  : carriage - 1 :
        //            down.push() ? carriage >= qty - 1 ? 0 : carriage + 1 :
        //            carriage;

        // if (carriage <= 3) {
        //    for (int i = 0; i < 4; i++)
        //    {
        //       lcd.line(i).cursor(19) << " ";
        //       if (i == carriage)
        //          lcd.line(i).cursor(19) << "~";
        //    }
        // } else {
        //    for (int i = 0; i < 4; i++)
        //    {
        //       lcd.line(i).cursor(19) << " ";
        //       if (n[i] == carriage)
        //          lcd.line(i).cursor(19) << "~";
        //    }
        // }

        if (next())
            lines[carriage_v].callback();
        if (back()) {
            f();
            carriage = 0;
            carriage_v = 0;
        }

    }
private:
    Button&               up;
    Button&               down;
    String_buffer&        lcd;
    std::array<Line, qty> lines;
    std::array<int, 3>    n;
    int carriage   {0};
    int carriage_v {0};
    int scroll     {0};

    bool back(){return (up and down).push_long();}
    bool next(){return (up and down).click();}
    

};


