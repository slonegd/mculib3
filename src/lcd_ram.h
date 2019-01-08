#pragma once

#include <array>
#include <cstddef>

class LCD
{
   size_t line     {0};
   size_t position {0};
   bool centre {false};

   std::array<char, 80> screen;
   static constexpr size_t screen_size = 80;

public:
   LCD (){screen.fill(' ');}
   static auto& make(){static auto lcd = LCD{}; return lcd;}
   LCD& operator<< (std::string_view string);
   LCD& operator<< (size_t number);
   LCD& set_line   (size_t string);
   LCD& set_cursor (size_t cursor);
   LCD& central();
   auto begin() {return screen.begin();}
   auto end  () {return screen.end()  ;}
   auto get_position(){return position;}
   auto get_line    (){return line;}

};

LCD& LCD::operator<< (std::string_view string)
{
   size_t rest_string = string.size();
      
   if (centre){
      if (rest_string < 20) {
         std::fill(screen.begin() + line*20, screen.begin() + line*20 + 20, ' ');
         size_t pos = (20 - rest_string)/2;
         std::copy(string.begin(), string.end(), screen.begin() + (line*20 + pos));
         centre = false;
      } else {
         std::copy(string.begin() + (rest_string - 20)/2, string.begin() + (rest_string - 20)/2 + 20, screen.begin() + line*20);
         centre = false;
      }
   } else {
      if (rest_string < screen_size - position) {
         std::copy(string.begin(), string.end(), screen.begin() + position);
         position += rest_string;
      } else {
         std::copy(string.begin(), string.begin() + (screen_size - position), screen.begin() + position);
         rest_string -= (screen_size - position);
         while (rest_string > screen_size) {
            auto string_begin = string.begin() + (string.size() - rest_string);
            auto string_end   = string_begin + screen_size;
            std::copy(string_begin, string_end, screen.begin());
            rest_string -= screen_size;
         }
         std::copy(string.begin() + (string.size() - rest_string), string.end(), screen.begin());
         position += rest_string;
      }
   }
      
   return *this;
}

LCD& LCD::operator<< (size_t number)
{
   char ch [10];
   snprintf(ch, sizeof(ch), "%u", number);
   // __itoa(number, ch, 10);
   // std::string_view s (ch);
   *this << ch;
   return *this;
}

LCD& LCD::central ()
{
   centre = true;
   return *this;
}

LCD& LCD::set_line(size_t string)
{
   line = string;
   position = line * 20;
   return *this;
}

LCD& LCD::set_cursor(size_t cursor)
{
   if      (line == 0) {
      if (cursor < 80)
         position = cursor;
      else position = cursor - 80 - (cursor/80) * 80;
   } else if (line == 1) {
      if (cursor < 60)
         position = cursor + 20;
      else position = cursor - 60 - (cursor/80) * 80;
   } else if (line == 2) {
      if (cursor < 40)
         position = cursor + 40;
      else position = cursor - 40 - (cursor/80) * 80;
   } else if (line == 3) {
      if (cursor < 20)
         position = cursor + 60;
      else position = cursor - 20 - (cursor/80) * 80;
   }
   
   // position = line == 1 ? cursor :
   //            line == 2 ? cursor + 20 :
   //            line == 3 ? cursor + 40 : cursor + 60;
   return *this;
}