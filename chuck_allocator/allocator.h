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
    Batch<T> *last;
    size_t counter;

public:
    typedef T value_type;
    typedef T* pointer;
    typedef const pointer const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    Allocator() {
        counter = size_t(1);
        last = nullptr;
    }

    ~Allocator() {
        Batch<T>* prev;

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

    T *allocate(size_t size) {
        Batch<T> *iter = last;

        while (iter != nullptr && iter->used + size * sizeof(T) > iter->size) {
            iter = iter->prev;
        }

        if (iter == nullptr) {
            Batch<T> *newBatch = new Batch<T>();
            newBatch->prev = last;
            last = newBatch;
            iter = last;
        }

        T *returned = (T *) ((iter->data) + iter->used);
        iter->used += size * sizeof(T);
        return returned;
    }

    void deallocate(T *address, size_t size) {
        this->~Allocator();
    }

    void construct(T *address, T value) {
        new(reinterpret_cast<void *>(address)) T(value);
    }

    template<class... Args>
    void construct(T *p, Args &&... args) {
        ::new(reinterpret_cast<void *>(p)) T(std::forward<Args>(args)...);
    }

    void destroy(T *ptr) {
        ptr->~T();
    }

    size_t get_counter() const {
        return counter;
    }

    template <typename U>
    struct rebind {
        using other = Allocator<U>;
    };
};


#endif //SIMPLE_EXAMPLE_ALLOCATOR_H
