#pragma once

// указатель для ясности: какой из аргументов меняется
// set_if_greater(&a, b)
//                 ^--этот
template <class T>
void set_if_greater (T* what, const T value)
{
    if (value > *what)
        *what = value;
}