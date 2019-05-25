#pragma once

template<class T>
class Hysteresis
{
public:
    Hysteresis(const T& current, const T min, const T max)
        : current{current}, min{min}, max{max}
    {}

    operator bool()
    {
        if (current > max)
            state = true;
        if (current < min)
            state = false;
        return state;
    }



private:
    const T& current;
    const T min;
    const T max;
    bool state {false};
};