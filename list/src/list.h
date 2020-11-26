#pragma once

#include <iterator>
#include <list>

#include <vector>
#include <algorithm>

namespace task {


    template<class T, class Alloc = std::allocator<T>>
    class list {

        struct BaseNode {
            BaseNode *next;
            BaseNode *prev;

            BaseNode() : prev(nullptr), next(nullptr) {};

            void push_back(BaseNode *other) {
                next = other;
                other->prev = this;
            }
        };

        struct Node : public BaseNode {
            T data;

            Node() = default;

            template<class... Args>
            Node(Args &&... args): BaseNode(), data(std::forward<Args>(args)...) {

            }
        };

        typename Alloc::template rebind<Node>::other allocator;
        BaseNode first;
        BaseNode last;
        size_t length;

        void init(size_t new_size = 0);

        template<class ... Args>
        std::pair<Node *, Node *> create_nodes(size_t count, Args &&... args);

        void delete_node(BaseNode *base_node);

    public:
        class const_iterator;

        class iterator {
        public:
            using difference_type = ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator() {
                this->node = nullptr;
            }

            iterator(const iterator &other) {
                this->node = other.node;
            }

            iterator(BaseNode *node) {
                this->node = node;
            }

            operator const_iterator() const {
                return node;
            }

            iterator &operator=(const iterator &other) {
                this->node = other.node;

                return *this;
            }

            iterator &operator++() {
                this->node = this->node->next;

                return *this;
            }

            iterator operator++(int) {
                iterator it = *this;
                this->node = this->node->next;

                return it;
            }

            reference operator*() const {
                return static_cast<Node *>(node)->data;
            }

            pointer operator->() const {
                return &static_cast<Node *>(node)->data;
            }

            iterator &operator--() {
                this->node = this->node->prev;

                return *this;
            }

            iterator operator--(int) {
                iterator res(*this);
                this->node = this->node->prev;

                return res;
            }

            bool operator==(iterator other) const {
                return this->node == other.node;
            }

            bool operator!=(iterator other) const {
                return this->node != other.node;
            }

        private:
            friend class list;

            BaseNode *node;
        };

        class const_iterator {
        public:
            using difference_type = ptrdiff_t;
            using value_type = T;
            using pointer = const T *;
            using reference = const T &;
            using iterator_category = std::bidirectional_iterator_tag;

            const_iterator() {
                this->node = nullptr;
            }

            const_iterator(const const_iterator &other) {
                this->node = other.node;
            }

            const_iterator(BaseNode *node) {
                this->node = node;
            }

            const_iterator &operator=(const const_iterator &other) {
                this->node = other.node;

                return *this;
            }

            const_iterator &operator++() {
                this->node = this->node->next;

                return *this;
            }

            const_iterator operator++(int) {
                const_iterator it = *this;
                this->node = this->node->next;

                return it;
            }

            reference operator*() const {
                return static_cast<Node *>(node)->data;
            }

            pointer operator->() const {
                return &static_cast<Node *>(node)->data;
            }

            const_iterator &operator--() {
                this->node = this->node->prev;

                return *this;
            }

            const_iterator operator--(int) {
                const_iterator it = *this;
                this->node = this->node->prev;

                return it;
            }

            bool operator==(const_iterator other) const {
                return this->node == other.node;
            }

            bool operator!=(const_iterator other) const {
                return this->node != other.node;
            }

        private:
            friend class list;

            BaseNode *node;
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

    private:
        void insert_nodes(const_iterator pos, size_t count, BaseNode *first, BaseNode *last);
    };

    template<class T, class Alloc>
    list<T, Alloc>::list(): length(0), first(), last(), allocator() {
        init();
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(const Alloc &alloc): length(0), first(), last(), allocator(alloc) {
        init();
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(size_t count, const T &value, const Alloc &alloc): list(alloc) {
        while (length < count) {
            push_back(value);
        }
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(size_t count, const Alloc &alloc): list(alloc) {
        resize(count);
    }

    template<class T, class Alloc>
    list<T, Alloc>::~list() {
        clear();
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(const list &other): list() {
        for (const_iterator i = other.cbegin(); i != other.cend(); i++) {
            push_back(*i);
        }
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(list &&other): length(0), first(), last(), allocator(std::move(other.allocator)) {
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
        return *begin();
    }

    template<class T, class Alloc>
    const T &list<T, Alloc>::front() const {
        return *cbegin();
    }

    template<class T, class Alloc>
    T &list<T, Alloc>::back() {
        return *(--end());
    }

    template<class T, class Alloc>
    const T &list<T, Alloc>::back() const {
        return *(--cend());
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
        iterator it;
        it.node = this->first.next;
        return it;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::end() {
        return &last;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const {
        const_iterator it;
        it.node = first.next;
        return it;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const {
        return {const_cast<BaseNode *>(&last)};
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin() {
        return reverse_iterator(end());
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend() {
        return reverse_iterator(begin());
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crbegin() const {
        return const_reverse_iterator(cend());
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crend() const {
        return const_reverse_iterator(cbegin());
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
        return length;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::clear() {
        while (!empty()) {
            pop_front();
        }
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, const T &value) {
        auto[first, last] = create_nodes(1, value);
        insert_nodes(pos, 1, first, last);

        return first;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, T &&value) {
        auto[first, last] = create_nodes(1, std::move(value));
        insert_nodes(pos, 1, first, last);

        return first;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, size_t count, const T &value) {
        auto[first, last] = create_nodes(count, value);
        insert_nodes(pos, count, first, last);

        return first;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator pos) {
        auto node = pos.node;
        auto next = node->next;

        node->prev->push_back(next);
        length--;

        delete_node(node);
        return next;
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator first, const_iterator last) {
        while (first != last) {
            erase(first++);
        }

        return last.node;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::push_back(const T &value) {
        insert(cend(), value);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::push_back(T &&value) {
        insert(cend(), std::move(value));
    }

    template<class T, class Alloc>
    void list<T, Alloc>::pop_back() {
        erase(--cend());
    }

    template<class T, class Alloc>
    void list<T, Alloc>::push_front(const T &value) {
        insert(cbegin(), value);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::push_front(T &&value) {
        insert(cbegin(), std::move(value));
    }

    template<class T, class Alloc>
    void list<T, Alloc>::pop_front() {
        erase(cbegin());
    }

    template<class T, class Alloc>
    template<class... Args>
    typename list<T, Alloc>::iterator list<T, Alloc>::emplace(const_iterator pos, Args &&... args) {
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
        emplace(cend(), std::forward<Args>(args)...);
    }

    template<class T, class Alloc>
    template<class... Args>
    void list<T, Alloc>::emplace_front(Args &&... args) {
        emplace(cbegin(), std::forward<Args>(args)...);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::resize(size_t count) {
        if (length < count) {
            auto[first, last] = create_nodes(count - length);
            insert_nodes(cend(), count - length, first, last);
            length = count;
        }

        while (length > count) {
            pop_back();
        }
    }

    template<class T, class Alloc>
    void list<T, Alloc>::swap(list &other) {
        if (length == 0 && other.length == 0) {
            return;
        }

        std::swap(allocator, other.allocator);
        std::swap(length, other.length);
        std::swap(first, other.first);
        std::swap(last, other.last);

        if (other.length == 0) {
            other.init();
        } else {
            other.first.push_back(other.first.next);
            other.last.prev->push_back(&other.last);
        }

        if (length == 0) {
            init();
        } else {
            first.push_back(first.next);
            last.prev->push_back(&last);
        }

    }

    template<class T, class Alloc>
    void list<T, Alloc>::init(size_t new_size) {
        last.prev = &first;
        first.next = &last;
        length = new_size;
    }

    template<class T, class Alloc>
    template<class ... Args>
    std::pair<typename list<T, Alloc>::Node *, typename list<T, Alloc>::Node *>
    list<T, Alloc>::create_nodes(size_t count, Args &&... args) {
        Node *prev = nullptr;
        Node *first = nullptr;
        for (size_t i = 0; i < count; ++i) {
            Node *node = allocator.allocate(1);
            allocator.construct(node, std::forward<Args>(args)...);
            node->prev = prev;
            if (prev) {
                prev->push_back(node);
            } else {
                first = node;
            }

            prev = node;
        }

        return std::make_pair(first, prev);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::delete_node(BaseNode *base_node) {
        auto node = static_cast<Node *>(base_node);
        allocator.destroy(node);
        allocator.deallocate(node, 1);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::insert_nodes(const_iterator pos, size_t count, BaseNode *first, BaseNode *last) {
        auto tail_node = pos.node;
        auto head_node = tail_node->prev;

        head_node->push_back(first);
        last->push_back(tail_node);

        length += count;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::merge(list &other) {
        if (other.empty()) {
            return;
        }

        auto current = this->begin();
        auto iter = other.begin();

        BaseNode *prev = nullptr;
        BaseNode *begin = nullptr;

        while (current != this->end() || iter != other.end()) {
            BaseNode *node = nullptr;
            if (current != this->end() && iter != other.end()) {
                node = static_cast<Node *>(iter.node)->data < static_cast<Node *>(current.node)->data ?
                       (iter++).node : (current++).node;

            } else if (current != this->end()) {
                node = (current++).node;

            } else if (iter != other.end()) {
                node = (iter++).node;
            }

            if (prev == nullptr) {
                begin = node;
            } else {
                prev->push_back(node);
            }
            prev = node;
        }

        size_t size = length + other.length;
        init();
        other.init();

        insert_nodes(cend(), size, begin, prev);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::splice(const_iterator pos, list &other) {
        if (other.empty()) {
            return;
        }

        insert_nodes(pos, other.length, other.first.next, other.last.prev);
        other.init();
    }

    template<class T, class Alloc>
    void list<T, Alloc>::remove(const T &value) {

        iterator iter = begin();
        while (iter != end()) {
            if (*iter == value) {
                iter = erase(iter);
            } else {
                iter++;
            }
        }
    }

    template<class T, class Alloc>
    void list<T, Alloc>::reverse() {
        if (length < 2) {
            return;
        }

        for (auto node = first.next; node != &last; node = node->prev) {
            auto tmp = node->next;
            node->next = node->prev;
            node->prev = tmp;
        }

        auto begin = last.prev;
        auto end = first.next;

        first.push_back(begin);
        end->push_back(&last);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::unique() {
        auto left_node = cbegin();
        auto right_node = ++cbegin();

        while (right_node != cend()) {
            if (*right_node == *left_node) {
                right_node = erase(right_node);
            } else {
                left_node = right_node++;
            }
        }
    }


    template<class T, class Alloc>
    void list<T, Alloc>::sort() {
        if (length < 2) {
            return;
        }

        auto left_part = begin();
        list right_part;

        size_t middle = length / 2;
        size_t position = 0;

        while (position < middle - 1) {
            left_part++;
            position++;
        }

        right_part.insert_nodes(right_part.cend(), length - middle, left_part.node->next, last.prev);
        left_part.node->push_back(&last);

        length = middle;

        this->sort();
        right_part.sort();

        merge(right_part);
    }

}  // namespace task