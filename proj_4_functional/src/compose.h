#include <iostream>

template<typename Type, typename ... Args>
auto compose(const std::function<Type(Args...)> &f) {
    return f;
}

template<typename Type1, typename Type2, typename ...Args>
auto compose(std::function<Type1(Type2)> &f_1, std::function<Type2(Args...)> &f_2) {
    return [f_1, f_2](auto ...arg) { return f_1(f_2(arg...)); };;
}

template<typename Type1, typename Type2, typename Args1, typename ...Args2>
auto compose(std::function<Type1(Type2)> &f_1, std::function<Type2(Args1)> &f_2, Args2 ...args2) {
    auto res = [f_1, f_2](auto arg) { return f_1(f_2(arg)); };
    return compose(res, args2...);
}
