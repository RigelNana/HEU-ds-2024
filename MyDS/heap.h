#ifndef HEAP_H
#define HEAP_H

#include "arraylist.h"
#include <initializer_list>
#include <stdexcept>

#ifdef __cplusplus
# include <functional>
# include <ostream>
# include <utility>
#endif
template <typename T, typename C = std::less<T>>
class Heap {
public:
    Heap() : comp(C()) {}

    explicit Heap(C const &comp) : comp(comp) {}

    Heap(std::initializer_list<T> list, C const &comp = C())
        : data(list),
          comp(comp) {
        build_heap();
    }

    Heap(ArrayList<T> const &list, C const &comp = C())
        : data(list),
          comp(comp) {
        build_heap();
    }

    Heap(Heap const &other) : data(other.data), comp(other.comp) {}

    Heap(Heap &&other) noexcept
        : data(std::move(other.data)),
          comp(std::move(other.comp)) {}

    Heap &operator=(Heap const &other) {
        if (this != &other) {
            Heap temp(other);
            swap(temp);
        }
        return *this;
    }

    Heap &operator=(Heap &&other) noexcept {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    ~Heap() = default;

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }

    void clear() {
        data.clear();
    }

    void push(T const &value) {
        data.push_back(value);
        up(data.size() - 1);
    }

    void pop() {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        data[0] = data.back();
        data.pop_back();
        if (!empty()) {
            down(0);
        }
    }

    T const &top() const {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return data[0];
    }

    friend std::ostream &operator<<(std::ostream &os, Heap const &heap) {
        os << '[';
        if (!heap.empty()) {
            for (size_t i = 0; i < heap.size() - 1; i++) {
                os << heap.data[i] << ' ';
            }
            os << heap.data[heap.size() - 1];
        }
        os << ']';
        return os;
    }

private:
    ArrayList<T> data;
    C comp;

    size_t parent(size_t i) const {
        return (i - 1) / 2;
    }

    size_t left_child(size_t i) const {
        return 2 * i + 1;
    }

    size_t right_child(size_t i) const {
        return 2 * i + 2;
    }

    void up(size_t index) {
        while (index > 0) {
            size_t p = parent(index);
            if (comp(data[index], data[p])) {
                std::swap(data[p], data[index]);
                index = p;
            } else {
                break;
            }
        }
    }

    void down(size_t index) {
        size_t largest = index;
        size_t l = left_child(index);
        size_t r = right_child(index);
        if (l < data.size() && comp(data[l], data[largest])) {
            largest = l;
        }
        if (r < data.size() && comp(data[r], data[largest])) {
            largest = r;
        }
        if (largest != index) {
            std::swap(data[index], data[largest]);
            down(largest);
        }
    }

    void build_heap() {
        if (data.empty()) {
            return;
        }
        for (int i = (data.size() / 2) - 1; i >= 0; i--) {
            down(i);
        }
    }
};

template <typename T>
Heap<T> make_max_heap(std::initializer_list<T> list) {
    return Heap<T>(list, std::less<T>());
}

template <typename T>
Heap<T> make_min_heap(std::initializer_list<T> list) {
    return Heap<T>(list, std::greater<T>());
}

#endif // !HEAP_H
