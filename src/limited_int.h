#pragma once

template<size_t n, class Int = size_t>
class Limited_int {
    Int value {0};
    size_t m {n};
public:
    inline Int operator++(int)   { return value = (++value < m) ? value : 0; }
    inline operator Int() const  { return value; }
    inline Int operator= (Int v) { return value = v; }
    inline void change_limit(int i){ m = i; }
    inline bool last() {return value == (m - 1) ? true : false;}
};