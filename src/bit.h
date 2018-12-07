#include <bitset>

template <class T>
bool is_high_bit(T number)
{
    constexpr int qty_bit = sizeof(T) * 8;
    std::bitset<qty_bit> bit (number);
    return bit[qty_bit - 1];
}