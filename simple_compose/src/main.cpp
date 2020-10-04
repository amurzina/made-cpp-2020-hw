#include <iostream>
#include <functional>

typedef std::function<int(int)> Op;


Op compose(size_t n, Op ops[]) {
    return [n, ops](int x) {
        for (int i = n - 1; i >= 0; i--) {
            x = ops[i](x);
            //std::cout << x << " " << i << std::endl;
        }
        return x;
    };
}


int main() {
    /// Simple tests:

    Op op1 =
            [](int x) -> int {
                return x + 1;
            };

    auto op2 =
            [](int p) -> Op {
                return [p](int x) {
                    return p * x;
                };
            };

    {   //     11 (10+1) <- 10 (5*2) <- 5 (4+1) <- 4 (2*2)
        Op ops[4] = {op1, op2(2), op1, op2(2)};
        if (compose(4, ops)(2) != 11) {
            std::cout << "FAILED AT TEST 1" << std::endl;
            return 0;
        }
    }

    {   //            55 <- 54(27*2) <-27 <-26 <-25 <-24(12*2) <-12(6*2) <-6(5+1)
        Op ops[8] = {op1, op2(2), op1, op1, op1, op2(2), op2(2), op1};
        if (compose(8, ops)(5) != 55) {
            std::cout << "FAILED AT TEST 2" << std::endl;
            return 0;
        }
    }

    {
        Op ops[1] = {op1};
        if (compose(1, ops)(3) != 4) {
            std::cout << "FAILED AT TEST 3" << std::endl;
            return 0;
        }
    }

    {
        Op ops[0] = {};
        if (compose(0, ops)(4) != 4) {
            std::cout << "FAILED AT TEST 4" << std::endl;
            return 0;
        }
    }

    {
        Op ops[4] = {op2(2), op2(3), op2(4), op2(5)};
        if (compose(4, ops)(1) != 120) {
            std::cout << "FAILED AT TEST 5" << std::endl;
            return 0;
        }
    }
}
