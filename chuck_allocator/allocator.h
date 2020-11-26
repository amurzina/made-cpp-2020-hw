#ifndef SIMPLE_EXAMPLE_ALLOCATOR_H
#define SIMPLE_EXAMPLE_ALLOCATOR_H
#pragma once

#define BATCH_SIZE 10000;

#include <iostream>

template<class T>
struct Batch {
    char *data;
    const size_t size = BATCH_SIZE;
    size_t used;

    Batch *prev;

    Batch() {
        data = new char[size];
        used = 0;
        prev = nullptr;
    }

    ~Batch() {
        delete[] data;
    }
};

template<class T>
class Allocator {

public:
    typedef T value_type;
    typedef T *pointer;
    typedef const pointer const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

private:
    Batch<value_type> *last;
    size_t counter;

public:
    Allocator() {
        counter = size_t(1);
        last = nullptr;
    }

    ~Allocator() {
        Batch<value_type> *prev;

        while (last != nullptr) {
            prev = last->prev;

            if (last->size == 1) {
                delete last;
            } else {
                last->used--;
            }
            last = prev;
        }

        last = nullptr;
    }

    Allocator(Allocator &other) {
        other.counter++;
        counter = other.counter;
        last = other.last;
    }

    Allocator &operator=(Allocator &allocator) {
        this->~Allocator();

        allocator.counter++;
        this->counter = allocator.counter;
        this->last = allocator.last;

        return *this;
    }

    pointer allocate(size_t size) {
        Batch<value_type> *iter = last;

        while (iter != nullptr && iter->used + size * sizeof(value_type) > iter->size) {
            iter = iter->prev;
        }

        if (iter == nullptr) {
            Batch<value_type> *newBatch = new Batch<value_type>();
            newBatch->prev = last;
            last = newBatch;
            iter = last;
        }

        pointer returned = (pointer) ((iter->data) + iter->used);
        iter->used += size * sizeof(value_type);
        return returned;
    }

    void deallocate(pointer address, size_t size) {
        this->~Allocator();
    }

    void construct(pointer address, value_type value) {
        new(reinterpret_cast<void *>(address)) value_type(value);
    }

    template<class... Args>
    void construct(pointer p, Args &&... args) {
        ::new(reinterpret_cast<void *>(p)) value_type(std::forward<Args>(args)...);
    }

    void destroy(pointer ptr) {
        ptr->~T();
    }

    size_t get_counter() const {
        return counter;
    }

    template<typename U>
    struct rebind {
        using other = Allocator<U>;
    };
};

#endif //SIMPLE_EXAMPLE_ALLOCATOR_H
