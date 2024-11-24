#ifndef PAIR_H
#define PAIR_H

#include <sys/signal.h>
#ifdef __cplusplus
# include <ostream>
# include <utility>
#endif
template <typename T, typename U>
class Pair {
public:
    T first;
    U second;

    Pair() = default;

    Pair(T const &f, U const &s) : first(f), second(s) {}

    Pair(T &&f, U &&s) noexcept : first(std::move(f)), second(std::move(s)) {}
    Pair(Pair const &other) : first(other.first), second(other.second) {}

    Pair(Pair &&other) noexcept
        : first(std::move(other.first)),
          second(std::move(other.second)) {}

    Pair &operator=(Pair const &other) {
        if (this != &other) {
            Pair temp(other);
            swap(temp);
        }
        return *this;
    }

    Pair &operator=(Pair &&other) noexcept {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    ~Pair() = default;

    bool operator==(Pair const &other) const {
        return first == other.first && second == other.second;
    }

    bool operator!=(Pair const &other) const {
        return !(*this == other);
    }

    bool operator<(Pair const &other) const {
        return first < other.first ||
               (!(other.first < first) && second < other.second);
    }

    bool operator>(Pair const &other) const {
        return other < *this;
    }

    bool operator<=(Pair const &other) const {
        return !(other < *this);
    }

    bool operator>=(Pair const &other) const {
        return !(*this < other);
    }

    friend std::ostream &operator<<(std::ostream &os, Pair const &p) {
        return os << '{' << p.first << ", " << p.second << '}';
    }

private:
    void swap(Pair &other) {
        std::swap(first, other.first);
        std::swap(second, other.second);
    }

    void swap(Pair &&other) noexcept {
        std::swap(first, other.first);
        std::swap(second, other.second);
    }
};

#endif // !PAIR_H
