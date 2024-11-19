#ifndef CIRCULAR_LIST_H

#define CIRCULAR_LIST_H
#include "linearlist.h"
#ifdef __cplusplus
# include <ostream>
#endif
template <typename T>
class CircularList : LinearList<T> {
    struct Node {
        T data;
        Node *next;
        Node *prev;
        Node(T const &value, Node *p = nullptr, Node *n = nullptr)
            : data(value),
              prev(p),
              next(n) {};
    };

    Node *head_;
    size_t size_;

    Node *locate(size_t index) const {
        Node *current = head_;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
        return current;
    }

    void swap(CircularList<T> &other) noexcept {
        std::swap(head_, other.head_);
        std::swap(size_, other.size_);
    }

public:
    CircularList() : head_(nullptr), size_(0) {}

    CircularList(CircularList<T> const &other) : head_(nullptr), size_(0) {
        if (!other.empty()) {
            Node *current = other.head_;
            push_back(current->data);
            current = current->next;
            while (current->next != other.head_) {
                push_back(current->data);
                current = current->next;
            }
        }
    }

    CircularList(CircularList<T> &&other)
        : head_(other.head_),
          size_(other.size_) {
        other.head_ = nullptr;
        other.size_ = 0;
    }

    CircularList<T> &operator=(CircularList<T> &other) {
        if (this != &other) {
            CircularList<T> temp(other);
            swap(temp);
        }
        return *this;
    }

    CircularList<T> &operator=(CircularList<T> &&other) {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    ~CircularList() override {
        clear();
    }

    bool empty() const noexcept override {
        return size_ == 0;
    }

    size_t size() const noexcept override {
        return size_;
    }

    void clear() noexcept override {
        if (!empty()) {
            Node *current = head_->next;
            while (current != head_) {
                Node *temp = current;
                current = current->next;
                delete temp;
            }
            delete head_;
            head_ = nullptr;
            size_ = 0;
        }
    }

    T &at(size_t index) override {
        this->check_index(index);
        return locate(index)->data;
    }

    T const &at(size_t index) const override {
        this->check_index(index);
        return locate(index)->data;
    }

    void insert(size_t index, T const &value) override {
        this->check_index(index, true);
        Node *new_node = new Node(value);
        if (empty()) {
            new_node->prev = new_node;
            new_node->next = new_node;
            head_ = new_node;
        } else {
            Node *current = (index == 0) ? head_ : locate(index);
            new_node->next = current;
            new_node->prev = current->prev;
            current->prev->next = new_node;
            current->prev = new_node;
            if (index == 0) {
                head_ = new_node;
            }
        }
        size_++;
    }

    void erase(size_t index) override {
        this->check_index(index);
        if (size_ == 1) {
            delete head_;
            head_ = nullptr;
        } else {
            Node *current = locate(index);
            current->prev->next = current->next;
            current->next->prev = current->prev;
            if (current == head_) {
                head_ = head_->next;
            }
            delete current;
        }
        size_--;
    }

    void push_back(T const &value) override {
        if (empty()) {
            insert(0, value);
        } else {
            Node *new_node = new Node(value, head_->prev, head_);
            head_->prev->next = new_node;
            head_->prev = new_node;
            size_++;
        }
    }

    void pop_back() override {
        this->check_empty();
        erase(size_ - 1);
    }

    T &front() override {
        this->check_empty();
        return head_->data;
    }

    T const &front() const override {
        this->check_empty();
        return head_->data;
    }

    T &back() override {
        this->check_empty();
        return head_->prev->data;
    }

    T const &back() const override {
        this->check_empty();
        return head_->prev->data;
    }

    void replace(size_t index, T const &value) override {
        this->check_index(index);
        locate(index)->data = value;
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    CircularList<T> const &list) {
        os << "[";
        if (!list.empty()) {
            Node *current = list.head_;
            os << current->data;
            current = current->next;
            while (current != list.head_) {
                os << " " << current->data;
                current = current->next;
            }
        }
        os << "]";
        return os;
    }
};
#endif // !CIRCULAR_LIST_H
