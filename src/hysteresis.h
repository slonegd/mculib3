#pragma once

template<class T, class U>
class Hysteresis
{
public:
    Hysteresis(const T& current, const U min, const U max)
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
    const U min;
    const U max;
    bool state {false};
};