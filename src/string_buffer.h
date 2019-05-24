#pragma once

#include <array>
#include <cstring>
#include <cstddef>

class String_buffer
{
    size_t  position   {0};
    // только для пользовательского форматирования следующие переменные
    // !!! не использовать как инфу о состоянии буфера
    // size_t  number_line{0};
    size_t  width_size {0};
    bool    align_center  {false};

    std::array<char, 80> screen;
    static constexpr size_t line_size = 20;
    static constexpr size_t screen_size = 80;

    using String_buffer_ref = String_buffer&(&) (String_buffer& string_buffer);
    friend String_buffer& next_line (String_buffer& s);

public:
    String_buffer (){screen.fill(' ');}
    String_buffer& operator<< (std::string_view string);
    String_buffer& operator<< (int number);
    String_buffer& operator<< (char);
    String_buffer& operator<< (String_buffer_ref& function);
    String_buffer& line   (size_t string);
    String_buffer& width  (size_t width );
    String_buffer& cursor (size_t cursor);
    String_buffer& center ();
    auto& get_buffer  (){return screen;}
    auto begin        () const {return screen.begin();}
    auto end          () const {return screen.end()  ;}
    auto get_position () const {return position;}
    auto get_line     () const {return position / line_size;}
    void clear        (){screen.fill(' '); position = 0; }
    bool in_begin_line() const { return position % line_size == 0; }

};

String_buffer& next_line (String_buffer& s) 
{
    while (not s.in_begin_line())
        s << ' ';
    return s;
}

String_buffer& String_buffer::operator<< (char v)
{
    *(screen.begin() + position++) = v;
    return *this;
}


String_buffer& String_buffer::operator<< (std::string_view string)
{
    size_t string_size = string.size();
    auto begin_line    = screen.begin() + position;
    auto end_line      = screen.begin() + position + line_size;

    if (align_center){
        if (string_size < line_size) {
            auto begin_string = begin_line + (line_size - string_size)/2;
            auto end_string   = begin_string + string.size();

            std::fill(begin_line    , begin_string, ' ');
            std::copy(string.begin(), string.end(), begin_string);
            std::fill(end_string    , end_line    , ' ');
            align_center = false;
        } else {
            auto begin_string = string.begin() + (string_size - line_size)/2;
            auto end_string   = begin_string + line_size;

            std::copy(begin_string, end_string, begin_line);
            align_center = false;
        }
        position += line_size;
        position %= screen_size;
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
            position %= screen_size;
        }
    }

    align_center = false;
    
    return *this;
}

String_buffer& String_buffer::operator<< (int number)
{
    char string[10];
    sprintf(string, "%d", number);
    std::string_view str (string);
    auto qty_space = (width_size > str.size()) ? (width_size - str.size()) : 0;

    while (qty_space--) {
        *this << ' ';
    }
    *this << string;
    width_size = 0;
    return *this;
}

String_buffer& String_buffer::operator<< (String_buffer_ref& function)
{
    return function(*this);
}

String_buffer& String_buffer::line(size_t v)
{
    position = v * line_size;
    return *this;
}

// FIX dont work
String_buffer& String_buffer::width(size_t width)
{
    width_size = width;
    return *this;
}

String_buffer& String_buffer::cursor(size_t cursor)
{
    position = (cursor + get_line() * line_size) % screen_size;
    return *this;
}

String_buffer& String_buffer::center ()
{
    align_center = true;
    return *this;
}