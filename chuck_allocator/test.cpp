#include <iostream>
#include <string>
#include <vector>
#include "allocator.h"

int main() {

    // allocate
    Allocator<int64_t> allocator_int_64;
    int64_t *array = allocator_int_64.allocate(5);

    //construct
    allocator_int_64.construct(array, int64_t(111111));
    allocator_int_64.construct(array + 1, int64_t(-222222));
    allocator_int_64.construct(array + 2, int64_t(-333333));
    allocator_int_64.construct(array + 3, int64_t(44444));
    allocator_int_64.construct(array + 4, int64_t(-55555));

    for (int64_t i = 0; i < 5; i++) {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;

    Allocator<int64_t> allocator;
    int64_t *another_array = allocator.allocate(5);

    allocator.construct(another_array, -5);
    allocator.construct(another_array + 1, -5);
    allocator.construct(another_array + 2, -5);
    allocator.construct(another_array + 3, -5);
    allocator.construct(another_array + 4, -5);
    allocator.construct(another_array + 5, -5);

    allocator.destroy(another_array);


    std::vector<int64_t , Allocator<int64_t>> vector;
    vector.push_back(111111);
    vector.push_back(222222);
    vector.push_back(333333);
    vector.push_back(444444);
    vector.push_back(555555);

    for (auto v : vector) {
        std::cout << v << " ";
    }
    std::cout << std::endl;


    decltype(allocator)::rebind<int64_t>::other rebinded_alloc;

    Allocator<int64_t> one_more;

    std::cout << one_more.get_counter() << " " << rebinded_alloc.get_counter() << std::endl;
    rebinded_alloc = one_more;
    std::cout << one_more.get_counter() << " " << rebinded_alloc.get_counter() << std::endl;

    Allocator<int64_t> alloc(one_more);
    std::cout << alloc.get_counter() << " " << alloc.get_counter() << "\n";

    return 0;
}
