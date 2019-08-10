// за основу взял вот это https://habr.com/ru/post/159389/
// убрал всё, что связано с кучей
// первый вариант был со статическим переменной внутри конструктора
// , что привело к невозможности перезаписать указатель, если тип тот же
// , потому добавил стандартный оператор 
// new со своей кучей (закоментировано) вызывал утечку
// отладка показала, что весь класс Function работает не так
// как предполагается
// с std::function всё работает и утечек нет
// Вывод: круче чем в стандартной библиотеке не написать
// TODO https://www.youtube.com/watch?v=hbx0WCc5_-w 
#pragma once

#include <functional>


template<class...Args>
using Callback = std::function<void(Args...)>;

template<class Function, class...Args>
void execute (Function f, Args...args) {
    if (f)
        f(args...);
}

template<class Function, class...Args>
void execute_if (bool condition, Function f, Args...args) {
    if (condition)
        execute (f, args...);
}


