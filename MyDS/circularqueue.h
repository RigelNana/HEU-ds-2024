#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#ifdef __cplusplus
# include <cstddef>
# include <ostream>
# include <stdexcept>
#endif
template <typename T>
class CircularQueue {
    CircularQueue() : CircularQueue(INITIAL_CAPACITY) {}

    explicit CircularQueue(size_t capacity)
        : elements(new T[capacity + 1]),
          _capacity(capacity + 1),
          _front(0),
          _back(0) {}

    CircularQueue(CircularQueue<T> const &other)
        : elements(new T[other._capacity]),
          _capacity(other._capacity),
          _front(other._front),
          _back(other._back) {
        for (size_t i = _front; i != _back; i = (i + 1) % _capacity) {
            elements[i] = other.elements[i];
        }
    }

    CircularQueue(CircularQueue<T> &&other) noexcept
        : elements(other.elements),
          _capacity(other._capacity),
          _front(other._front),
          _back(other._back) {
        other.elements = nullptr;
        other._capacity = 0;
        other._front = 0;
        other._back = 0;
    }

    CircularQueue<T> &operator=(CircularQueue<T> const &other) {
        if (this != &other) {
            CircularQueue<T> temp(other);
            swap(temp);
        }
        return *this;
    }

    CircularQueue<T> &operator=(CircularQueue<T> &&other) noexcept {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    ~CircularQueue() {
        delete[] elements;
    }

    bool empty() const noexcept {
        return _front == _back;
    }

    bool full() const noexcept {
        return (_back + 1) % _capacity == _front;
    }

    size_t size() const noexcept {
        return (_back - _front + _capacity) % _capacity;
    }

    void clear() noexcept {
        _front = _back = 0;
    }

    void enqueue(T const &value) {
        if (full()) {
            size_t new_capacity = 2 * (_capacity - 1) + 1;
            reserve(new_capacity);
        }
        elements[_back] = value;
        _back = (_back + 1) % _capacity;
    }

    void dequeue() {
        check_empty();
        _front = (_front + 1) % _capacity;
    }

    T &front() {
        check_empty();
        return elements[_front];
    }

    T const &front() const {
        check_empty();
        return elements[_front];
    }

    T &back() {
        check_empty();
        return elements[(_back - 1 + _capacity) % _capacity];
    }

    T const &back() const {
        check_empty();
        return elements[(_back - 1 + _capacity) % _capacity];
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > _capacity - 1) {
            T *new_elements = new T[new_capacity + 1];
            size_t j = 0;
            for (size_t i = _front; i != _back; i = (i + 1) % _capacity) {
                new_elements[j++] = elements[i];
            }
            delete[] elements;
            elements = new_elements;
            _front = 0;
            _back = j;
            _capacity = new_capacity + 1;
        }
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    CircularQueue<T> const &queue) {
        os << "[";
        if (!queue.empty()) {
            size_t i = queue._front;
            while (i != queue._back - 1) {
                os << queue.elements[i] << ' ';
                i = (i + 1) % queue._capacity;
            }
            os << queue.elements[i];
        }
        os << "]";
        return os;
    }

private:
    T *elements;
    size_t _capacity;
    size_t _front;
    size_t _back;
    static constexpr size_t INITIAL_CAPACITY = 16;

    void check_empty() const {
        if (empty()) {
            throw std::runtime_error("empty queue!");
        }
    }

    void swap(CircularQueue<T> &other) noexcept {
        std::swap(elements, other.elements);
        std::swap(_capacity, other._capacity);
        std::swap(_front, other._front);
        std::swap(_back, other._back);
    }
};
#endif
