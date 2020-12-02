#pragma once

namespace task {

    template<class T>
    class UniquePtr;

    template<class T>
    class SharedPtr;

    template<class T>
    class WeakPtr;

    template<class T>
    struct ControlBlock {
        T *ptr;
        std::size_t counter;

        explicit ControlBlock(T *) noexcept;

        void remove() noexcept;
    };

    template<class T>
    class UniquePtr {
    private:

        T *ptr;

    public:

        UniquePtr() noexcept;

        explicit UniquePtr(T *) noexcept;

        UniquePtr(UniquePtr<T> &&) noexcept;

        ~UniquePtr();

        UniquePtr &operator=(UniquePtr<T> &&) noexcept;

        T &operator*() const;

        T *operator->() const noexcept;

        T *release() noexcept;

        void swap(UniquePtr<T> &) noexcept;

        T *get() const noexcept;

        void reset(T *) noexcept;

    };

    template<class T>
    class SharedPtr {
        friend class WeakPtr<T>;

    private:

        ControlBlock<T> *сontroller;

    public:

        SharedPtr() noexcept;

        explicit SharedPtr(T *) noexcept;

        SharedPtr(const SharedPtr<T> &) noexcept;

        SharedPtr(const WeakPtr<T> &) noexcept;

        SharedPtr(SharedPtr<T> &&) noexcept;

        ~SharedPtr();

        SharedPtr &operator=(const SharedPtr<T> &) noexcept;

        SharedPtr &operator=(SharedPtr<T> &&) noexcept;

        T &operator*() const noexcept;

        T *operator->() const noexcept;

        T *get() const noexcept;

        void swap(SharedPtr<T> &) noexcept;

        void reset() noexcept;

        void reset(T *) noexcept;

        std::size_t use_count() const;

    };

    template<class T>
    class WeakPtr {
    private:

        ControlBlock<T> *сontroller;

    public:

        WeakPtr() noexcept;

        WeakPtr(SharedPtr<T> &) noexcept;

        WeakPtr(const WeakPtr<T> &) noexcept;

        WeakPtr(WeakPtr<T> &&) noexcept;

        ~WeakPtr() = default;

        WeakPtr &operator=(const WeakPtr<T> &) noexcept;

        WeakPtr &operator=(WeakPtr<T> &&) noexcept;

        WeakPtr &operator=(SharedPtr<T> &) noexcept;

        SharedPtr<T> lock() const noexcept;

        bool expired() const noexcept;

        void reset() noexcept;

        std::size_t use_count() const noexcept;

    };
}  // namespace task



#include "smart_pointers.tpp"