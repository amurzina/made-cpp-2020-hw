#include <functional>
#include <iostream>
#include <assert.h>

#include "compose.h"

std::function<int(int)> increase = [](int x) -> int { return x + 1; };

std::function<bool(int)> is_positive = [](int x) -> bool { return x > 0; };

std::function<std::string(int)> int_to_str = [](int x) -> std::string { return std::to_string(x); };

std::function<int(std::string)> str_to_num = [](const std::string &x) -> int { return std::stoi(x); };

int main() {
    int int_result;

    for (int i = 0; i < 10; i++) {
        int_result = compose(increase)(i);
        assert(int_result == i + 1);
    }

    for (int i = 0; i < 10; i++) {
        auto res_func = compose(increase, is_positive);
        assert(res_func(i) == true);
    }

    {
        try {
            auto res_func = compose(is_positive, increase, int_to_str);
            assert(res_func(0) == "1");
        } catch (const std::exception &ex) {
            std::cout << ex.what() << std::endl;
            assert(1 == 0);
        }
    }

    {
        try {
            auto res_func = compose(str_to_num, is_positive, increase, int_to_str, increase);
            res_func("0");
            assert(1 == 0);
        } catch (const std::exception &ex) {
            std::cout << ex.what() << std::endl;
            assert(1 == 1);
        }
    }

    return 0;
}