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
private:
	struct Holder {
		virtual Return call(Args ... args) = 0;
	};

	template <class Function>
	class Function_holder : public Holder {
        const Function& func;
	public:
        Function_holder (const Function& func) : func{func} {}
		virtual Return call(Args ... args) override {
			return func(args ...);
		}
	};

	Holder* functor_pointer {nullptr};
};