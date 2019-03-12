#pragma once

#include <algorithm>


template<class T> struct Listable
{
   T* prev {nullptr};
   T* next {nullptr};
};

template<class T> class List
{
   T* first {nullptr};
   T* last  {nullptr};
public:
   class Iterator
   {
      T* p {nullptr};
   public:
      using iterator_category = std::input_iterator_tag;
      using value_type        = T;
      using difference_type   = T;
      using pointer           = const T*;
      using reference         = T;
      Iterator (T*);
      Iterator() = default;
      operator T*() { return p; }
      T&        operator*  () const;
      T*        operator-> () const { return p; }
      bool      operator!= (const Iterator&) const;
      Iterator& operator++ ();
   };

   void clear_subscribe(){first = nullptr; last = nullptr;}
   void push_back  (T&);
   void push_front (T&);
   void remove     (T&);
   void insert  (Iterator, T&);
   Iterator begin();
   Iterator end();
};






template<class T>
void List<T>::push_back (T& v)
{
   v.prev = last;
   if (last)
      last->next = &v;
   last = &v;
   if (not first)
      first = &v;
}


template<class T>
void List<T>::push_front (T& v)
{
   v.next = first;
   if (first)
      first->prev = &v;
   first = &v;
   if (not last)
      last = &v;
}


template<class T>
void List<T>::remove (T& v)
{
   if (v.prev and v.next) {         // если в серединке
      v.prev->next = v.next;
      v.next->prev = v.prev;
   } else if (v.next) {             // если первый
      first = v.next;
      first->prev = nullptr;
      if (not first) last = nullptr;
   } else if (v.prev) {             // если последний
      last = v.prev;
      last->next = nullptr;
      if (not last) first = nullptr;
   } else {                         // если единственный 
      first = nullptr;
      last  = nullptr;
   }
   v.prev = nullptr;
   v.next = nullptr;

}



template<class T>
void List<T>::insert (typename List<T>::Iterator it, T& v)
{
   if (it == begin()) {
      push_front (v);
   } else if (it == end()) {
      push_back (v);
   } else {
      v.prev = it->prev;
      v.next = it;
      it->prev = &v;
      v.prev->next = &v;
   }
}


template<class T>
typename List<T>::Iterator List<T>::begin()
{
   return List<T>::Iterator {first};
}


template<class T>
typename List<T>::Iterator List<T>::end()
{
   return List<T>::Iterator();
}



/// Iterator
template<class T>
List<T>::Iterator::Iterator (T* other) : p {other} {}

template<class T>
T& List<T>::Iterator::operator* () const 
{
   return *p;
}

template<class T>
bool List<T>::Iterator::operator!= (const List<T>::Iterator& other) const
{
   return p != other.p;
}

template<class T>
typename List<T>::Iterator& List<T>::Iterator::operator++ ()
{
   p = p->next;
   return *this;
}
