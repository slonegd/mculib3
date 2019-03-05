/**
 * куча, которая расположена на стеке,
 * размер задаётся макросом HEAP_SIZE
 * переопределены глобальные операторы new delete
 * удаление предполагается только для последнего объекта
 * иначе будет дефрагментация и в таком случае
 * new будет возвращать nullptr
 */
#pragma once

#include <array>

template<size_t heap_size>
class Heap
{
    std::array<uint8_t, heap_size> memory;
    size_t free_index {0};
    bool defragmented {false};
public:
    void* allocate (size_t size)
    {
        auto p = reinterpret_cast<void*>(memory.begin() + free_index);
        free_index += size;
        // если необходимо памяти больше, чем выделено, то тут будет HardFault
        // что определиться при первой же отладке
        return (free_index > heap_size or defragmented) ? nullptr : p;
    }
    void deallocate (void* p, size_t size)
    {
        if (p != memory.begin() + free_index - size) {
            defragmented = true;
            return;
        }
        free_index -= size;
    }

    inline void* operator_multinew (size_t size)
    {
            auto p = allocate(size + sizeof(size_t));
            if (p) {
                auto psize = reinterpret_cast<size_t*>(p); 
                *psize = size;
                return ++psize;
            } else {
                return nullptr;
            }
    }

    inline void operator_multidelete (void* p)
    {
            auto psize = reinterpret_cast<size_t*>(p);
            psize--;
            deallocate (psize, *psize + sizeof(size_t)); 
    }
};

// выделяем память под динамические объекты
#if not defined(HEAP_SIZE)
#define HEAP_SIZE 256
#endif

Heap<HEAP_SIZE> heap {};

