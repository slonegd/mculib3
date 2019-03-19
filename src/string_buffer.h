#pragma once

#include <array>
#include <cstring>
#include <cstddef>

class String_buffer
{
   size_t  number_line{0};
   size_t  width_size {0};
   size_t  position   {0};
   bool    align  {false};

   std::array<char, 80> screen;
   static constexpr size_t line_size = 20;
   static constexpr size_t screen_size = 80;

   using String_buffer_ref = String_buffer&(&) (String_buffer& string_buffer);
   friend String_buffer& next_line (String_buffer& s);

public:
   String_buffer (){screen.fill(' ');}
   String_buffer& operator<< (std::string_view string);
   String_buffer& operator<< (int number);
   String_buffer& operator<< (String_buffer_ref& function);
   String_buffer& line   (size_t string);
   String_buffer& width  (size_t width );
   String_buffer& cursor (size_t cursor);
   String_buffer& center ();
   auto& get_buffer (){return screen;}
   auto begin       (){return screen.begin();}
   auto end         (){return screen.end()  ;}
   auto get_position(){return position;}
   auto get_line    (){return number_line;}
   void clear       (){screen.fill(' ');}

};

String_buffer& next_line (String_buffer& s) 
{
   s.position = (++s.number_line) * s.line_size;
   return s;
}


String_buffer& String_buffer::operator<< (std::string_view string)
{
   size_t string_size = string.size();
   auto begin_line    = screen.begin() + number_line * line_size;
   auto end_line      = screen.begin() + number_line * line_size + line_size;

   // if (width_) {position += (width_size - string_size); width_ = false;}
   if (align){
      if (string_size < line_size) {
         auto begin_string = begin_line + (line_size - string_size)/2;
         auto end_string   = begin_string + string.size();

         std::fill(begin_line    , begin_string, ' ');
         std::copy(string.begin(), string.end(), begin_string);
         std::fill(end_string    , end_line    , ' ');
         align = false;
      } else {
         auto begin_string = string.begin() + (string_size - line_size)/2;
         auto end_string   = begin_string + line_size;

         std::copy(begin_string, end_string, begin_line);
         align = false;
      }
      position = number_line * line_size + line_size;
   } else {
      if (string_size < screen_size - position) {
         std::copy(string.begin(), string.end(), screen.begin() + position);
         position += string_size;
      } else {
         std::copy(string.begin(), string.begin() + (screen_size - position), screen.begin() + position);
         size_t rest_string = string_size - (screen_size - position);
         
         while (rest_string > screen_size) {
            auto begin_string = string.begin() + (string_size - rest_string);
            auto end_string   = begin_string + screen_size;
            std::copy(begin_string, end_string, screen.begin());
            rest_string -= screen_size;
         }
         std::copy(string.begin() + (string_size - rest_string), string.end(), screen.begin());
         position += rest_string;
      }
   }
   
   return *this;
}

String_buffer& String_buffer::operator<< (int number)
{
   char string[10];
   sprintf(string, "%d", number);
   std::string_view str (string);
   auto qty_space = (width_size > str.size()) ? (width_size - str.size()) : 0;

   while (qty_space) {
      *this << " ";
      qty_space--;
   }
   *this << string;
   width_size = 0;
   return *this;
}

String_buffer& String_buffer::operator<< (String_buffer_ref& function)
{
   return function(*this);
}

String_buffer& String_buffer::line(size_t string)
{
   number_line = string;
   position = number_line * line_size;
   return *this;
}

String_buffer& String_buffer::width(size_t width)
{
   width_size = width;
   return *this;
}

String_buffer& String_buffer::cursor(size_t cursor)
{
   position = (cursor + number_line * line_size) % screen_size;
   return *this;
}

String_buffer& String_buffer::center ()
{
   align = true;
   return *this;
}