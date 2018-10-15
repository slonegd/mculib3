#pragma once


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
   void push_back  (T*);
   void push_front (T*);
   void remove     (T*);
   
   class Iterator
   {
      T* p {nullptr};
   public:
      Iterator (T*);
      Iterator() = default;

      T&        operator*  () const;
      bool      operator!= (const Iterator&) const;
      Iterator& operator++ ();
   };

   Iterator begin();
   Iterator end();
};





template<class T>
void List<T>::push_back (T* p)
{
   p->prev = last;
   if (last)
      last->next = p;
   last = p;
   if (not first)
      first = p;
}


template<class T>
void List<T>::push_front (T* p)
{
   // не реализован
}


template<class T>
void List<T>::remove (T* p)
{
   if (p->prev and p->next) {       // если в серединке
      p->prev->next = p->next;
      p->next->prev = p->prev;
   } else if (p->next) {            // если первый
      first = p->next;
      first->prev = nullptr;
      if (not first) last = nullptr;
   } else if (p->prev) {            // если последний
      last = p->prev;
      last->next = nullptr;
      if (not last) first = nullptr;
   } else {                         // если единственный 
      first = nullptr;
      last  = nullptr;
   }
   p->prev = nullptr;
   p->next = nullptr;

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
