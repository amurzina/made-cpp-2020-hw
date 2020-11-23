#pragma once

#include <iterator>
#include <memory>

namespace task {

    template<class T, class Alloc = std::allocator <T>>
    class list {

    private:

        class Node {
        public:
            T data;
            Node *next;
            Node *prev;

            Node(Node *n, Node *p) : next(n), prev(p) {};

            Node(const T &d, Node *n, Node *p) : data(d), next(n), prev(p) {};

            Node(T &&d, Node *n, Node *p) : data(std::move(d)), next(n), prev(p) {};
        };

        using allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
        allocator_type allocator;
        Node *first;
        Node *last;
        size_t length;

    public:
        class iterator {
        public:
            using difference_type = ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator();

            iterator(const iterator &);

            iterator &operator=(const iterator &);

            iterator &operator++();

            iterator operator++(int);

            reference operator*() const;

            pointer operator->() const;

            iterator &operator--();

            iterator operator--(int);

            bool operator==(iterator other) const;

            bool operator!=(iterator other) const;

            friend class list;

        private:
            Node *node;
        };

        class const_iterator {
        public:
            using difference_type = ptrdiff_t;
            using value_type = T;
            using pointer = const T *;
            using reference = const T &;
            using iterator_category = std::bidirectional_iterator_tag;

            const_iterator();

            const_iterator(const const_iterator &);

            const_iterator(const iterator &);

            const_iterator &operator=(const const_iterator &);

            const_iterator &operator++();

            const_iterator operator++(int);

            reference operator*() const;

            pointer operator->() const;

            const_iterator &operator--();

            const_iterator operator--(int);

            bool operator==(const_iterator other) const;

            bool operator!=(const_iterator other) const;

            friend class list;

        private:
            const Node *node;
        };

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;


        list();

        explicit list(const Alloc &alloc);

        list(size_t count, const T &value, const Alloc &alloc = Alloc());

        explicit list(size_t count, const Alloc &alloc = Alloc());

        ~list();

        list(const list &other);

        list(list &&other);

        list &operator=(const list &other);

        list &operator=(list &&other);

        Alloc get_allocator() const;


        T &front();

        const T &front() const;

        T &back();

        const T &back() const;


        iterator begin();

        iterator end();

        const_iterator cbegin() const;

        const_iterator cend() const;

        reverse_iterator rbegin();

        reverse_iterator rend();

        const_reverse_iterator crbegin() const;

        const_reverse_iterator crend() const;


        bool empty() const;

        size_t size() const;

        size_t max_size() const;

        void clear();

        iterator insert(const_iterator pos, const T &value);

        iterator insert(const_iterator pos, T &&value);

        iterator insert(const_iterator pos, size_t count, const T &value);

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);


        void push_back(const T &value);

        void push_back(T &&value);

        void pop_back();

        void push_front(const T &value);

        void push_front(T &&value);

        void pop_front();

        template<class... Args>
        iterator emplace(const_iterator pos, Args &&... args);

        template<class... Args>
        void emplace_back(Args &&... args);

        template<class... Args>
        void emplace_front(Args &&... args);

        void resize(size_t count);

        void swap(list &other);

        void merge(list &other);

        void splice(const_iterator pos, list &other);

        void remove(const T &value);

        void reverse();

        void unique();

        void sort();
    };

    template<class T, class Alloc>
    list<T, Alloc>::iterator::iterator() {
        this->node = nullptr;
    }

    template<class T, class Alloc>
    list<T, Alloc>::iterator::iterator(const iterator &other) {
        this->node = other.node;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator &
    list<T, Alloc>::iterator::operator=(const task::list<T, Alloc>::iterator &other) {
        this->node = other.node;
        return *this;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator++() {
        this->node = this->node->next;
        return *this;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator++(int) {
        iterator iter = *this;
        this->node = this->node->next;
        return iter;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator::reference list<T, Alloc>::iterator::operator*() const {
        return this->node->data;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator::pointer list<T, Alloc>::iterator::operator->() const {
        return &(this->node->data);
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator--() {
        this->node = this->node->prev;
        return *this;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator--(int) {
        iterator iter = *this;
        this->node = this->node->prev;
        return iter;
    }

    template<class T, class Alloc>
    bool list<T, Alloc>::iterator::operator==(iterator other) const {
        return this->node == other.node;
    }

    template<class T, class Alloc>
    bool list<T, Alloc>::iterator::operator!=(iterator other) const {
        return this->node != other.node;
    }

    template<class T, class Alloc>
    list<T, Alloc>::const_iterator::const_iterator() {
        this->node = nullptr;
    }

    template<class T, class Alloc>
    list<T, Alloc>::const_iterator::const_iterator(const const_iterator &other) {
        this->node = other.node;
    }

    template<class T, class Alloc>
    list<T, Alloc>::const_iterator::const_iterator(const iterator &other) {
        this->node = other.node;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator &
    list<T, Alloc>::const_iterator::operator=(const task::list<T, Alloc>::const_iterator &other) {
        this->node = other.node;
        return *this;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator &list<T, Alloc>::const_iterator::operator++() {
        this->node = this->node->next;
        return *this;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::const_iterator::operator++(int) {
        const_iterator it = *this;
        this->node = this->node->next;
        return it;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator::reference list<T, Alloc>::const_iterator::operator*() const {
        return this->node->data;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator::pointer list<T, Alloc>::const_iterator::operator->() const {
        return &(this->node->data);
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator &list<T, Alloc>::const_iterator::operator--() {
        this->node = this->node->prev;
        return *this;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::const_iterator::operator--(int) {
        iterator iter = *this;
        this->node = this->node->prev;
        return iter;
    }

    template<class T, class Alloc>
    bool list<T, Alloc>::const_iterator::operator==(const_iterator other) const {
        return this->node == other.node;
    }

    template<class T, class Alloc>
    bool list<T, Alloc>::const_iterator::operator!=(const_iterator other) const {
        return this->node != other.node;
    }

    template<class T, class Alloc>
    list<T, Alloc>::list() : length(0) {
        first = allocator.allocate(1);
        last = allocator.allocate(1);

        first->prev = nullptr;
        first->next = last;
        last->prev = first;
        last->next = nullptr;
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(const Alloc &alloc) : list<T, Alloc>() {
        allocator = alloc;
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(size_t count, const T &value, const Alloc &alloc) : list<T, Alloc>(alloc) {
        while (length < count) {
            push_back(value);
        }
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(size_t count, const Alloc &alloc) : list<T, Alloc>(alloc) {
        resize(count);
    }

    template<class T, class Alloc>
    list<T, Alloc>::~list() {
        clear();

        allocator.deallocate(first, 1);
        allocator.deallocate(last, 1);
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(const list &other) : list<T, Alloc>() {
        for (const_iterator i = other.cbegin(); i != other.cend(); i++) {
            push_back(*i);
        }
    }


    template<class T, class Alloc>
    list<T, Alloc>::list(list &&other) : list<T, Alloc>() {
        first->next = other.first->next;
        first->next->prev = first;

        last->prev = other.last->prev;
        last->prev->next = last;
        length = other.length;

        other.first->next = other.last;
        other.last->prev = other.first;
        other.length = 0;
    }

    template<class T, class Alloc>
    list<T, Alloc> &list<T, Alloc>::operator=(const list &other) {
        clear();

        for (const_iterator i = other.cbegin(); i != other.cend(); i++) {
            push_back(*i);
        }

        return *this;
    }

    template<class T, class Alloc>
    list<T, Alloc> &list<T, Alloc>::operator=(list &&other) {
        clear();

        first->next = other.first->next;
        first->next->prev = first;

        last->prev = other.last->prev;
        last->prev->next = last;

        length = other.length;

        other.first->next = other.last;
        other.last->prev = other.first;
        other.length = 0;

        return *this;
    }

    template<class T, class Alloc>
    Alloc list<T, Alloc>::get_allocator() const {
        return allocator;
    }

    template<class T, class Alloc>
    T &list<T, Alloc>::front() {
        return first->next->data;
    }

    template<class T, class Alloc>
    const T &list<T, Alloc>::front() const {
        return first->next->data;
    }

    template<class T, class Alloc>
    T &list<T, Alloc>::back() {
        return last->prev->data;
    }

    template<class T, class Alloc>
    const T &list<T, Alloc>::back() const {
        return last->prev->data;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
        iterator it;
        it.node = first->next;
        return it;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::end() {
        iterator it;
        it.node = last;
        return it;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const {
        const_iterator it;
        it.node = first->next;
        return it;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const {
        const_iterator it;
        it.node = last;
        return it;
    }

    template<class T, class Alloc>
    typename task::list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin() {
        reverse_iterator it;
        it.node = last->prev;
        return it;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend() {
        reverse_iterator it;
        it.node = first;
        return it;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crbegin() const {
        const_reverse_iterator it(cend());
        return it;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crend() const {
        const_reverse_iterator it(cbegin());
        return it;
    }

    template<class T, class Alloc>
    bool list<T, Alloc>::empty() const {
        return length == 0;
    }

    template<class T, class Alloc>
    size_t list<T, Alloc>::size() const {
        return length;
    }

    template<class T, class Alloc>
    size_t list<T, Alloc>::max_size() const {
        return allocator.max_size();
    }


    template<class T, class Alloc>
    void list<T, Alloc>::clear() {
        while (!empty()) {
            pop_front();
        }
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, const T &value) {
        Node *prev = pos.node->prev;
        Node *node = allocator.allocate(1);

        allocator.construct(node, value, prev->next, prev);
        prev->next->prev = node;
        prev->next = node;

        iterator iter;
        iter.node = node;
        length++;

        return iter;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, T &&value) {
        Node *prev = pos.node->prev;
        Node *node = allocator.allocate(1);

        allocator.construct(node, std::move(value), prev->next, prev);
        prev->next->prev = node;
        prev->next = node;

        iterator iter;
        iter.node = node;
        length++;

        return iter;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, size_t count, const T &value) {
        iterator iter;

        for (int i = 0; i < count; i++) {
            iter = insert(pos, value);
        }

        return iter;
    }


    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator pos) {
        Node *deleted_el = const_cast<Node *>(pos.node);
        iterator it;
        it.node = deleted_el->next;

        it.node->prev = deleted_el->prev;
        deleted_el->prev->next = deleted_el->next;

        length--;

        allocator.destroy(deleted_el);
        allocator.deallocate(deleted_el, 1);

        return it;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator first, const_iterator last) {
        const_iterator c_iter = first;

        while (c_iter != last) {
            c_iter = erase(c_iter);
        }

        iterator iter;
        iter.node = const_cast<Node *>(c_iter.node);
        return iter;
    }


    template<class T, class Alloc>
    void list<T, Alloc>::push_back(const T &value) {
        Node *node = allocator.allocate(1);
        allocator.construct(node, value, last, last->prev);

        last->prev = node;
        last->prev->next = node;

        length++;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::push_back(T &&value) {
        Node *node = allocator.allocate(1);
        allocator.construct(node, std::move(value), last, last->prev);

        last->prev = node;
        last->prev->next = node;

        length++;
    }


    template<class T, class Alloc>
    void list<T, Alloc>::pop_back() {
        if (empty()) {
            return;
        }

        Node *node = last->prev;

        node->prev->next = node->next;
        node->next->prev = node->prev;

        allocator.destroy(node);
        allocator.deallocate(node, 1);

        length++;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::push_front(const T &value) {
        Node *node = allocator.allocate(1);
        allocator.construct(node, value, first->next, first);

        first->next = node;
        first->next->prev = node;

        length++;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::push_front(T &&value) {
        Node *node = allocator.allocate(1);
        allocator.construct(node, std::move(value), first->next, first);

        first->next = node;
        first->next->prev = node;

        length++;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::pop_front() {
        if (empty()) {
            return;
        }

        Node *node = first->next;

        node->prev->next = node->next;
        node->next->prev = node->prev;

        allocator.destroy(node);
        allocator.deallocate(node, 1);

        length--;
    }

    template<class T, class Alloc>
    template<class... Args>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::emplace(typename list<T, Alloc>::const_iterator pos, Args &&... args) {
        Node *prev = pos.node->prev;
        Alloc data_allocator;

        Node *node = allocator.allocate(1);
        T *data = data_allocator.allocate(1);

        data_allocator.construct(data, std::forward<Args>(args)...);
        allocator.construct(node, std::move(*data), prev->next, prev);

        data_allocator.destroy(data);
        data_allocator.deallocate(data, 1);

        prev->next->prev = node;
        prev->next = node;

        iterator iter;
        iter.node = pos.node->prev;

        length++;
        return iter;
    }

    template<class T, class Alloc>
    template<class... Args>
    void list<T, Alloc>::emplace_back(Args &&... args) {
        emplace(end(), std::forward<Args>(args)...);
    }

    template<class T, class Alloc>
    template<class... Args>
    void list<T, Alloc>::emplace_front(Args &&... args) {
        emplace(begin(), std::forward<Args>(args)...);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::resize(size_t count) {
        while (length < count) {
            Node *node = allocator.allocate(1);
            allocator.construct(node, last, last->prev);

            last->prev = node;
            last->prev->next = node;

            length++;
        }

        while (length > count) {
            pop_back();
        }
    }

    template<class T, class Alloc>
    void list<T, Alloc>::swap(list &other) {
        Node *temp_node = first;
        first = other.first;
        other.first = temp_node;

        temp_node = other.last;
        other.last = last;
        last = temp_node;

        size_t temp_size = length;
        length = other.length;
        other.length = temp_size;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::merge(list &other) {
        if (other.empty()) {
            return;
        }

        Node *iter = other.first->next;
        Node *current = first->next;

        while (iter != other.last) {
            while (current != last && iter->data > current->data) {
                current = current->next;
            }

            Node *next = iter->next;
            iter->prev = current->prev;
            iter->next = current;
            current->prev->next = iter;
            current->prev = iter;
            iter = next;
        }

        other.first->next = other.last;
        other.last->prev = other.first;

        length += other.length;
        other.length = 0;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::splice(const_iterator pos, list &other) {
        if (other.empty()) {
            return;
        }

        Node *temp = const_cast<Node *>(pos.node);
        temp->prev->next = other.first->next;
        other.first->next->prev = temp->prev;

        temp->prev = other.last->prev;
        other.last->prev->next = temp;
        other.first->next = other.last;
        other.last->prev = other.first;

        length += other.length;
        other.length = 0;
    }

    template<class T, class Alloc>
    void task::list<T, Alloc>::remove(const T &value) {
        list<iterator> deleted_iters;

        iterator iter = begin();
        while (iter != end()) {
            if (*iter == value) {
                deleted_iters.push_back(iter);
            }
            iter++;
        }

        for (iterator i = deleted_iters.begin(); i != deleted_iters.end(); i++) {
            erase(*i);
        }
    }


    template<class T, class Alloc>
    void task::list<T, Alloc>::reverse() {
        Node *node = first->next;

        while (node != last) {
            Node *next = node->next;
            node->next = node->prev;
            node->prev = next;
            node = next;
        }


        first->prev = first->next;
        first->next = nullptr;

        last->next = last->prev;
        last->prev = nullptr;

        node = first;
        first = last;

        last = node;
    }

    template<class T, class Alloc>
    void task::list<T, Alloc>::unique() {
        Node *left_node = first;
        Node *right_node = nullptr;

        for (size_t i(0); i < length; ++i) {
            right_node = left_node->next;

            for (size_t j(i); j < length; ++j) {
                if (left_node->data == right_node->data) {
                    left_node->next = right_node->next;

                    if (right_node->next) {
                        right_node->next->prev = left_node;
                    }

                    erase(right_node);
                    length--;
                    right_node = left_node->next;

                } else {
                    break;
                }
                right_node = right_node->next;
            }
            left_node = left_node->next;
        }
    }

    template<class T, class Alloc>
    void task::list<T, Alloc>::sort() {
        if (length < 2) {
            return;
        }

        Node *left_node = first;
        Node *right_node = nullptr;

        for (size_t i(0); i < length; ++i) {
            right_node = left_node->next;
            for (size_t j(i + 1); j < length; ++j) {

                if (left_node->data > right_node->data) {
                    std::swap(left_node->data, right_node->data);
                }

                right_node = right_node->next;
            }
            left_node = left_node->next;
        }
    }

}  // namespace task