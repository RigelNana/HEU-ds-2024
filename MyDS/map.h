#ifndef MAP_H
#define MAP_H
#include "arraylist.h"
#include "pair.h"
#include <stdexcept>
#ifdef __cplusplus
# include <ostream>
# include <utility>
#endif
template <typename K, typename V>
class Map {
public:
    using type = Pair<K, V>;
    Map() = default;

    Map(Map const &other) : data(other.data) {}

    Map(Map &&other) noexcept : data(std::move(other.data)) {}

    Map &operator=(Map const &other) {
        if (this != &other) {
            Map temp(other);
            swap(temp);
        }
        return *this;
    }

    Map &operator=(Map &&other) noexcept {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    ~Map() = default;

    bool empty() const noexcept {
        return data.empty();
    }

    size_t size() const noexcept {
        return data.size();
    }

    void clear() noexcept {
        data.clear();
    }

    void insert(K const &key, V const &value) {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].first == key) {
                data[i].second = value;
                return;
            }
        }
        data.push_back(type(key, value));
    }

    void insert(type const &pair) {
        insert(pair.first, pair.second);
    }

    void erase(K const &key) {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].first == key) {
                data.erase(i);
                return;
            }
        }
    }

    bool contains(K const &key) {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].first == key) {
                return true;
            }
        }
        return false;
    }
    bool contains(K const &key) const {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].first == key) {
                return true;
            }
        }
        return false;
    }

    V &at(K const &key) {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].first == key) {
                return data[i].second;
            }
        }
        throw std::out_of_range("Key not found");
    }

    V const &at(K const &key) const {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].first == key) {
                return data[i].second;
            }
        }
        throw std::out_of_range("Key not found");
    }

    V &operator[](K const &key) {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].first == key) {
                return data[i].second;
            }
        }
        data.push_back(type(key, V()));
        return data.back().second;
    }

    V const& operator[](K const &key) const {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].first == key) {
                return data[i].second;
            }
        }
        throw std::out_of_range("Key not found");
    }

    type *begin() noexcept {
        return data.begin();
    }

    type *end() noexcept {
        return data.end();
    }

    type const *begin() const noexcept {
        return data.begin();
    }

    type const *end() const noexcept {
        return data.end();
    }

    friend std::ostream &operator<<(std::ostream &os, Map const &map) {
        os << "{";
        for (size_t i = 0; i < map.data.size(); i++) {
            os << map.data[i];
            if (i != map.data.size() - 1) {
                os << ", ";
            }
        }
        os << "}";
        return os;
    }

private:
    ArrayList<type> data;

    void swap(Map &other) {
        std::swap(data, other.data);
    }

    void swap(Map &&other) {
        std::swap(data, other.data);
    }
};

#endif // !MAP_H
