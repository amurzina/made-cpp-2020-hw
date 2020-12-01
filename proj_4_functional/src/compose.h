#include <iostream>

template<typename F1, typename F2>
class Composite {
public:
    F1 f_1;
    F2 f_2;

    Composite(F1 f1, F2 f2) : f_1(f1), f_2(f2) {}

    template<typename input>
    auto operator()(input i) {
        return f_2(f_1(i));
    }
};


template<typename F>
auto compose(F f) {
    return [f](auto x){return f(x);};
}

template<typename F1, typename F2>
auto compose(F1 f_1, F2 f_2g) {
    return Composite<F1, F2>{f_1, f_2g};
}

template <class Tp, class... Us>
struct type {
    typedef typename Tp::result_type _type;
};

template<typename F1, typename... Types>
auto compose(F1 funcs, Types... args) {
    if (!std::is_same<typename F1::argument_type, typename type<Types...>::_type>::value) { // with the help of discord chat
        throw std::invalid_argument("# Expected the same types, but given different\n");
    }

    return compose(funcs, compose(args...));
}