#include <cstddef>
#include <stdexcept>
#ifndef LINEAR_LIST_H
template <typename T>
class LinearList {
public:
    virtual ~LinearList() = default;

    virtual bool empty() const noexcept = 0;
    virtual size_t size() const noexcept = 0;
    virtual void clear() noexcept = 0;
    virtual T &at(size_t index) = 0;
    virtual T const &at(size_t index) const = 0;

    virtual void insert(size_t index, T const &value) = 0;
    virtual void erase(size_t index) = 0;
    virtual void push_back(T const &value) = 0;
    virtual void pop_back() = 0;
    virtual T &front() = 0;
    virtual T const &front() const = 0;
    virtual T &back() = 0;
    virtual T const &back() const = 0;

    virtual void replace(size_t index, T const &value) = 0;

protected:
    void check_index(size_t index, bool end = false) {
        if ((!end && index >= size()) || (end && index > size())) {
            throw std::out_of_range("out of range");
        }
    }

    void check_empty() const {
        if (empty()) {
            throw std::runtime_error("empty!");
        }
    }
};

#endif // !LINEAR_LIST_H
