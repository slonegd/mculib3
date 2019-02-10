#pragma once

#include <array>

// необходимо понимать, что в микромире работать с кучей надо особенно аккуратно
// постоянные выделения/освобождения памяти могут привести к дефрагментации
// от этого легко обезопаситься, если применять new к объектам
// , которые никогда не удаляются, а это практически все объекты
// , которые работают с переферией, инициализируются на старте
// и перестают работать только, когда пропадает питание

// Для собственного оператора нужна собственная куча
// самый простой способ, выделить память в самом начале в виде массива
// и выдавать указатель оператору, контролируя переполнение этого массива
// Способ посложнее, который не рассматриваю пока, прописать кучу в линкере
template<size_t heap_size>
class Heap
{
   std::array<uint8_t, heap_size> memory;
   size_t free_index {0};
   bool defragmented {false};
public:
   void* allocate (size_t size)
   {
      if (defragmented) {
         void* p = reinterpret_cast<void*>(memory.begin() + free_index);
         free_index += size;
         // если нам необходимо памяти больше, чем выделено, то тут будет HardFault
         // что определиться при первой же отладке
         return (free_index < heap_size) ? p : nullptr;
      } else {
         // аналогично, если была дефрагментация
         return nullptr; 
      }
   }
   void deallocate (void* p, size_t size)
   {
      if (p != memory.begin() + free_index - size) {
         defragmented = true;
         return;
      }
      free_index -= size;
   }
};

// выделяем память под динамические объекты
constexpr size_t heap_size {256};
Heap<heap_size> heap {};

// собственный глобальный оператор new, который работает с нашей кучей
void* operator new (size_t size)
{
   return heap.allocate(size);
}

void* operator new[] (size_t size)
{
   auto p = heap.allocate(size + sizeof(size_t));
   auto psize = reinterpret_cast<size_t*>(p); 
   *psize = size;
   return ++psize;
}

void operator delete[] (void* p)
{
   auto psize = reinterpret_cast<size_t*>(p);
   psize--;
   heap.deallocate (psize, *psize); 
}