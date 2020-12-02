#include "smart_pointers.h"

namespace task {

    template<class T>
    ControlBlock<T>::ControlBlock(T *ptr) noexcept {
        this->ptr = ptr;
        this->counter = 1;
    }

    template<class T>
    void ControlBlock<T>::remove() noexcept {
        if (counter == 1) {
            delete this->ptr;
        }
        this->counter--;
    }

    template<class T>
    UniquePtr<T>::UniquePtr() noexcept {
        this->ptr = nullptr;
    }

    template<class T>
    UniquePtr<T>::UniquePtr(T *ptr) noexcept {
        this->ptr = ptr;
    }

    template<class T>
    UniquePtr<T>::UniquePtr(UniquePtr<T> &&uniquePtr) noexcept {
        this->ptr = std::move(uniquePtr.ptr);
        uniquePtr.ptr = nullptr;
    }

    template<class T>
    UniquePtr<T>::~UniquePtr<T>() {
        delete ptr;
    }

    template<class T>
    UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr<T> &&uniquePtr) noexcept {
        ptr = std::move(uniquePtr.ptr);
        uniquePtr.ptr = nullptr;

        return *this;
    }

    template<class T>
    T &UniquePtr<T>::operator*() const {
        return *get();
    }

    template<class T>
    T *UniquePtr<T>::operator->() const noexcept {
        return get();
    }

    template<class T>
    T *UniquePtr<T>::release() noexcept {
        T *buffer = ptr;
        ptr = nullptr;
        return buffer;
    }

    template<class T>
    void UniquePtr<T>::swap(UniquePtr<T> &uniquePtr) noexcept {
        std::swap(ptr, uniquePtr.ptr);
    }

    template<class T>
    T *UniquePtr<T>::get() const noexcept {
        return ptr;
    }

    template<class T>
    void UniquePtr<T>::reset(T *ptr) noexcept {
        this->ptr = ptr;
    }

    template<class T>
    SharedPtr<T>::SharedPtr() noexcept {
        this->сontroller = nullptr;
    }

    template<class T>
    SharedPtr<T>::SharedPtr(T *ptr) noexcept {
        this->сontroller = new ControlBlock(ptr);
    }

    template<class T>
    SharedPtr<T>::SharedPtr(const SharedPtr<T> &sharedPtr) noexcept {
        this->сontroller = sharedPtr.сontroller;

        if (this->сontroller) {
            this->сontroller->counter++;
        }
    }

    template<class T>
    SharedPtr<T>::SharedPtr(const WeakPtr<T> &weakPtr) noexcept {
        this->сontroller = weakPtr.lock();

        if (this->сontroller) {
            this->сontroller->counter++;
        }
    }

    template<class T>
    SharedPtr<T>::SharedPtr(SharedPtr<T> &&other) noexcept {
        this->сontroller = std::move(other.сontroller);
        other.сontroller = nullptr;
    }

    template<class T>
    SharedPtr<T>::~SharedPtr() {
        if (this->сontroller) {
            this->сontroller->remove();
        }
    }

    template<class T>
    SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<T> &other) noexcept {
        if (&other == this) {
            return *this;
        }

        if (this->сontroller) {
            this->сontroller->remove();
        }

        this->сontroller = other.сontroller;
        if (this->сontroller) {
            this->сontroller->counter++;
        }

        return *this;
    }

    template<class T>
    SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr<T> &&other) noexcept {
        if (this->сontroller) {
            this->сontroller->remove();
        }

        this->сontroller = std::move(other.сontroller);

        if (this->сontroller) {
            other.сontroller = nullptr;
        }

        return *this;
    }

    template<class T>
    T &SharedPtr<T>::operator*() const noexcept {
        return *get();
    }

    template<class T>
    T *SharedPtr<T>::operator->() const noexcept {
        return get();
    }

    template<class T>
    T *SharedPtr<T>::get() const noexcept {
        return this->сontroller ? this->сontroller->ptr : nullptr;
    }

    template<class T>
    void SharedPtr<T>::swap(SharedPtr<T> &other) noexcept {
        std::swap(this->сontroller, other.сontroller);
    }

    template<class T>
    void SharedPtr<T>::reset() noexcept {
        if (this->сontroller) {
            this->сontroller->remove();
        }
        this->сontroller = nullptr;
    }

    template<class T>
    void SharedPtr<T>::reset(T *ptr) noexcept {
        if (this->сontroller) {
            this->сontroller->remove();
        }
        this->сontroller = new ControlBlock(ptr);
    }

    template<class T>
    std::size_t SharedPtr<T>::use_count() const {
        return this->сontroller ? this->сontroller->counter : 0;
    }

    template<class T>
    WeakPtr<T>::WeakPtr() noexcept {
        this->сontroller = nullptr;
    }

    template<class T>
    WeakPtr<T>::WeakPtr(SharedPtr<T> &sharedPtr) noexcept {
        this->сontroller = sharedPtr.сontroller;
    }

    template<class T>
    WeakPtr<T>::WeakPtr(const WeakPtr<T> &other) noexcept {
        this->сontroller = other.сontroller;
    }

    template<class T>
    WeakPtr<T>::WeakPtr(WeakPtr<T> &&other) noexcept {
        this->сontroller = std::move(other.сontroller);
        other.сontroller = nullptr;
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr<T> &other) noexcept {
        if (&other == this) {
            return *this;
        }

        this->сontroller = other.сontroller;

        return *this;
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(WeakPtr<T> &&other) noexcept {
        this->сontroller = std::move(other.сontroller);
        other.ptr = nullptr;

        return *this;
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(SharedPtr<T> &other) noexcept {
        this->сontroller = other.сontroller;

        return *this;
    }

    template<class T>
    SharedPtr<T> WeakPtr<T>::lock() const noexcept {
        if (expired()) {
            return SharedPtr<T>();
        }
        SharedPtr<T> buffer;

        buffer.сontroller = this->сontroller;
        this->сontroller->counter++;

        return buffer;
    }

    template<class T>
    bool WeakPtr<T>::expired() const noexcept {
        return use_count() == 0;
    }

    template<class T>
    void WeakPtr<T>::reset() noexcept {
        this->сontroller = nullptr;
    }

    template<class T>
    std::size_t WeakPtr<T>::use_count() const noexcept {
        return this->сontroller ? this->сontroller->counter : 0;
    }
}