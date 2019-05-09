// за основу взял вот это https://habr.com/ru/post/159389/
// убрал всё, что связано с кучей
// ТУДУ поддержка методов объектов (но пока не нужна)
#pragma once

template <class T>
class Function;

template <class Return, class ... Args>
class Function <Return (Args...)> {
public:
    Function() = default;
	template <class Function_>
	Function (Function_ f) {
        static Function_holder<Function_> holder (f);
        functor_pointer = &holder;
    }

	Return operator() (Args ... args) {
		return functor_pointer->call(args...);
	}

    operator bool() { return functor_pointer != nullptr; }
private:
	struct Holder {
		virtual Return call(Args ... args) = 0;
	};

	template <class Function>
	class Function_holder : public Holder {
        Function func;
	public:
        Function_holder (Function func) : func{func} {}
		virtual Return call(Args ... args) override {
			return func(args ...);
		}
	};

	Holder* functor_pointer {nullptr};
};

template<class...Args>
using Callback = Function<void(Args...)>;

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

void null_function() {}