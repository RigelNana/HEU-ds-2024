#ifndef ARRAY_STACK_H
#define ARRAY_STACK_H
#ifdef __cplusplus
# include <cstddef>
# include <ostream>
# include <stdexcept>
#endif
template <typename T>
class ArrayStack {
public:
    ArrayStack() : ArrayStack(INITIAL_CAPACITY) {}

    explicit ArrayStack(size_t capacity)
        : elements(new T[capacity]),
          _capacity(capacity),
          _size(0) {}

    ArrayStack(ArrayStack<T> const &other)
        : elements(new T[other._capacity]),
          _capacity(other._capacity),
          _size(other._size) {
        for (size_t i = 0; i < _size; i++) {
            elements[i] = other.elements[i];
        }
    }

    ArrayStack(ArrayStack<T> &&other) noexcept
        : elements(other.elements),
          _capacity(other._capacity),
          _size(other._size) {
        other.elements = nullptr;
        other._capacity = 0;
        other._size = 0;
    }

    ArrayStack<T> &operator=(ArrayStack<T> const &other) {
        if (this != &other) {
            ArrayStack<T> temp(other);
            swap(temp);
        }
        return *this;
    }

    ArrayStack<T> &operator=(ArrayStack<T> &&other) noexcept {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    ~ArrayStack() {
        delete[] elements;
    }

    bool empty() const noexcept {
        return _size == 0;
    }

    size_t size() const noexcept {
        return _size;
    }

    void clear() noexcept {
        _size = 0;
    }

    void push(T const &value) {
        if (_size == _capacity) {
            size_t new_capacity = (_capacity == 0) ? 1 : 2 * _capacity;
            reserve(new_capacity);
        }
        elements[_size++] = value;
    }

    void pop() {
        check_empty();
        --_size;
    }

    T &top() {
        check_empty();
        return elements[_size - 1];
    }

    T const &top() const {
        check_empty();
        return elements[_size - 1];
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > _capacity) {
            T *new_elements = new T[new_capacity];
            for (size_t i = 0; i < _size; i++) {
                new_elements[i] = elements[i];
            }
            delete[] elements;
            elements = new_elements;
            _capacity = new_capacity;
        }
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    ArrayStack<T> const &stack) {
        os << "[";
        if (!stack.empty()) {
            for (size_t i = 0; i < stack.size() - 1; i++) {
                os << stack.elements[i] << ' ';
            }
            os << stack.elements[stack.size() - 1];
        }
        os << "]";
        return os;
    }

private:
    T *elements;
    size_t _capacity;
    size_t _size;
    static constexpr size_t INITIAL_CAPACITY = 16;

    void check_empty() const {
        if (empty()) {
            throw std::runtime_error("empty stack!");
        }
    }

    void swap(ArrayStack<T> &other) noexcept {
        std::swap(elements, other.elements);
        std::swap(_capacity, other._capacity);
        std::swap(_size, other._size);
    }
};
#endif
