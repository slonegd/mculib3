#pragma once

template <class T>
void set_if_greater (T& what, T value)
{
    if (value > what)
        what = value;
}