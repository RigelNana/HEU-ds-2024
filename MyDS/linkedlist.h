#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "linearlist.h"
#include <ostream>

template <typename T>
class LinkedList : LinearList<T> {
private:
    struct Node {
        T data;
        Node *next;
        Node *prev;

        Node(T const &value, Node *p = nullptr, Node *n = nullptr)
            : data(value),
              next(n),
              prev(p) {}
    };

    Node *head_;
    Node *tail_;
    size_t size_;

    Node *locate(size_t index) const {
        Node *current;
        if (index <= size_ / 2) {
            current = head_;
            for (size_t i = 0; i < index; i++) {
                current = current->next;
            }
        } else {
            current = tail_;
            for (size_t i = size_ - 1; i > index; i--) {
                current = current->prev;
            }
        }
        return current;
    }

    void swap(LinkedList<T> &other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

public:
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {};

    LinkedList(LinkedList<T> const &other)
        : head_(nullptr),
          tail_(nullptr),
          size_(0) {
        Node *current = other.head_;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

    LinkedList(LinkedList<T> &&other)
        : head_(other.head_),
          tail_(other.tail_),
          size_(other.size_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    LinkedList<T> &operator=(LinkedList<T> const &other) {
        if (this != &other) {
            LinkedList<T> temp(other);
            swap(temp);
        }
        return *this;
    }

    LinkedList<T> &operator=(LinkedList<T> &&other) noexcept {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    ~LinkedList() override {
        clear();
    }

    bool empty() const noexcept override {
        return size_ == 0;
    }

    size_t size() const noexcept override {
        return size_;
    }

    void clear() noexcept override {
        while (head_ != nullptr) {
            Node *tmp = head_;
            head_ = head_->next;
            delete tmp;
        }
        tail_ = nullptr;
        size_ = 0;
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
        if (index == 0) {
            Node *new_node = new Node(value, nullptr, head_);
            if (head_) {
                head_->prev = new_node;
            } else {
                tail_ = new_node;
            }

        } else if (index == size_) {
            Node *new_node = new Node(value, tail_, nullptr);
            if (tail_) {
                tail_->next = new_node;
            } else {
                head_ = new_node;
            }
        } else {
            Node *current = locate(index);
            Node *new_node = new Node(value, current->prev, current);
            current->prev->next = new_node;
            current->prev = new_node;
        }
        ++size_;
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
        return tail_->data;
    }

    T const &back() const override {
        this->check_empty();
        return tail_->data;
    }

    void replace(size_t index, T const &value) override {
        this->check_index(index);
        locate(index)->data = value;
    }

    void push_back(T value) override {
        if (empty()) {
            head_ = tail_ = new Node(value);
        } else {
            Node *new_node = new Node(value, tail_, nullptr);
            tail_->next = new_node;
            tail_ = new_node;
        }
        ++size_;
    }

    void pop_back() override {
        this->check_empty();
        erase(size_ - 1);
    }

    void erase(size_t index) override {
        this->check_index(index);
        Node *delete_;
        if (index == 0) {
            delete_ = head_;
            head_ = head_->next;
            if (head_) {
                head_->prev = nullptr;
            } else {
                tail_ = nullptr;
            }
        } else if (index == size_ - 1) {
            delete_ = tail_;
            tail_ = tail_->prev;
            tail_->next = nullptr;
        } else {
            delete_ = locate(index);
            delete_->next->prev = delete_->prev;
            delete_->prev->next = delete_->next;
        }
        delete delete_;
        size_--;
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    LinkedList<T> const &other) {
        os << "[";
        if (!other.empty()) {
            Node *cur = other.head_;
            while (cur->next != nullptr) {
                os << cur->data << ' ';
                cur = cur->next;
            }
            os << cur->data;
        }
        os << "]";
        return os;
    }
};
#endif // !LINKED_LIST_H
