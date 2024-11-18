#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#include "linearlist.h"

#ifdef __cplusplus

# include <ostream>

#endif
template <typename T>
class ArrayList : public LinearList<T> {
public:
    ArrayList() : ArrayList(INITIAL_CAPACITY) {}

    explicit ArrayList(size_t capacity)
        : elements(new T[capacity]),
          _capacity(capacity),
          _size(0) {}

    ArrayList(ArrayList<T> const &other)
        : elements(new T[other._capacity]),
          _capacity(other._capacity),
          _size(other._size) {
        for (size_t i = 0; i < _size; i++) {
            elements[i] = other.elements[i];
        }
    }

    ArrayList(ArrayList<T> &&other)
        : elements(other.elements),
          _capacity(other._capacity),
          _size(other._size) {
        other.elements = nullptr;
        other._capacity = 0;
        other._size = 0;
    }

    ArrayList<T> &operator=(ArrayList<T> &other) {
        if (this != &other) {
            ArrayList<T> temp(other);
            swap(temp);
        }
        return *this;
    }

    ArrayList<T> &operator=(ArrayList<T> &&other) noexcept {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    ~ArrayList() override {
        delete[] elements;
    }

    bool empty() const noexcept override {
        return _size == 0;
    }

    size_t size() const noexcept override {
        return _size;
    }

    void clear() noexcept override {
        _size = 0;
    }

    T &at(size_t index) override {
        this->check_index(index);
        return elements[index];
    }

    T const &at(size_t index) const override {
        this->check_index(index);
        return elements[index];
    }

    void insert(size_t index, T const &value) override {
        this->check_index(index, true);
        if (_size == _capacity) {
            size_t new_capacity = (_capacity == 0) ? 1 : 2 * _capacity;
            reserve(new_capacity);
        }
        for (size_t i = _size; i > index; i--) {
            elements[i] = elements[i - 1];
        }
        elements[index] = value;
        ++_size;
    }

    void replace(size_t index, T const &value) override {
        this->check_index(index);
        elements[index] = value;
    }

    void erase(size_t index) override {
        this->check_index(index);
        for (size_t i = index; i < _size - 1; i++) {
            elements[i] = elements[i + 1];
        }
        --_size;
    }

    void push_back(T const &value) override {
        insert(_size, value);
    }

    void pop_back() override {
        this->check_empty();
        --_size;
    }

    T &front() override {
        this->check_empty();
        return elements[0];
    }

    T const &front() const override {
        this->check_empty();
        return elements[0];
    }

    T &back() override {
        this->check_empty();
        return elements[_size - 1];
    }

    T const &back() const override {
        this->check_empty();
        return elements[_size - 1];
    }

    void swap(ArrayList<T> &other) {
        swap(other);
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
                                    ArrayList<T> const &list) {
        os << "[";
        if (!list.empty()) {
            for (size_t i = 0; i < list.size() - 1; i++) {
                os << list.elements[i] << ' ';
            }
            os << list.elements[list.size() - 1];
        }
        os << "]";
        return os;
    }

private:
    T *elements;
    size_t _capacity;
    size_t _size;
    static constexpr size_t INITIAL_CAPACITY = 16;

    void swap(ArrayList<T> &&other) noexcept {
        std::swap(elements, other.elements);
        std::swap(_capacity, other._capacity);
        std::swap(_size, other._size);
    }
};

#endif // !ARRAY_LIST_H
