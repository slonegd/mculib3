#pragma once

#include <array>
#include <algorithm>
#include <type_traits>

template <class T, size_t size_>
class Static_vector
{
    std::array<T, size_> vector;
    size_t end_i   {0};
public:
    template<class ... Args>
    constexpr Static_vector (const Args&... args) : vector{args...}{end_i = sizeof... (args);}
    void clear()  {end_i = 0;}
    constexpr auto cbegin() const {return vector.begin();}
    constexpr auto cend  () const {return vector.begin() + end_i;}
    constexpr auto data  () const {return vector.data();}
    constexpr auto size ()  {return end_i;}
    void set_size (size_t v) {end_i = std::min(v,size_);}
    constexpr void push_back(const T& value)
    {
        if (end_i < size_)
            vector[end_i++] = value;
    }
    void erase (const size_t index) 
    {
        for (auto i = index + 1; i < end_i; i++) {
            vector[i - 1] = vector[i];
        }
        end_i--;
    }

    constexpr T operator[] (const size_t index) const
    {
        return vector[index];
    }

    constexpr operator std::array<T,size_>() const
    {
        return vector;
    }

};

