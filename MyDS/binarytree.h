#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include "circularqueue.h"
#ifdef __cplusplus
# include <algorithm>
# include <cstddef>
# include <ostream>
#endif
template <typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node *left;
        Node *right;

        Node(T const &value) : data(value), left(nullptr), right(nullptr) {}

        Node(T const &value, Node *l, Node *r)
            : data(value),
              left(l),
              right(r) {}
    };

    Node *root;
    size_t size_;

    size_t height(Node const *node) const noexcept {
        if (node == nullptr) {
            return 0;
        }
        size_t left_height = height(node->left);
        size_t right_height = height(node->right);
        return std::max(left_height, right_height) + 1;
    }

    Node *copy_tree(Node const *node) {
        if (node == nullptr) {
            return nullptr;
        }
        return new Node(node->data, copy_tree(node->left),
                        copy_tree(node->right));
    }

    void destroy_tree(Node *node) noexcept {
        if (node != nullptr) {
            destroy_tree(node->left);
            destroy_tree(node->right);
            delete node;
        }
    }

public:
    BinaryTree() : root(nullptr), size_(0) {}

    BinaryTree(BinaryTree<T> const &other)
        : root(copy_tree(other.root)),
          size_(other.size_) {}

    BinaryTree(BinaryTree<T> &&other) : root(other.root), size_(other.size_) {
        other.root = nullptr;
        other.size_ = 0;
    }

    BinaryTree<T> &operator=(BinaryTree<T> const &other) {
        if (this != &other) {
            BinaryTree<T> temp(other);
            swap(temp);
        }
        return *this;
    }

    BinaryTree<T> &operator=(BinaryTree<T> &&other) {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    ~BinaryTree() {
        destroy_tree(root);
    }

    bool empty() const noexcept {
        return root == nullptr;
    }

    size_t size() const noexcept {
        return size_;
    }

    size_t height() const noexcept {
        return height(root);
    }

    void clear() noexcept {
        destroy_tree(root);
        root = nullptr;
        size_ = 0;
    }

    void pre_order_traverse(Node const *node, void (*visit)(T const &)) const {
        if (node != nullptr) {
            visit(node->data);
            pre_order_traverse(node->left, visit);
            pre_order_traverse(node->right, visit);
        }
    }

    void in_order_traverse(Node const *node, void (*visit)(T const &)) const {
        if (node != nullptr) {
            in_order_traverse(node->left, visit);
            visit(node->data);
            in_order_traverse(node->right, visit);
        }
    }

    void post_order_traverse(Node const *node, void (*visit)(T const &)) const {
        if (node != nullptr) {
            post_order_traverse(node->left, visit);
            post_order_traverse(node->right, visit);
            visit(node->data);
        }
    }

    void bfs(void (*visit)(T const &)) const {
        if (empty()) {
            return;
        }
        CircularQueue<Node const *> q;
        q.enqueue(root);
        while (!q.empty()) {
            Node const *curr = q.front();
            q.dequeue();

            visit(curr->data);
            if (curr->left) {
                q.enqueue(curr->left);
            }
            if (curr->right) {
                q.enqueue(curr->right);
            }
        }
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    BinaryTree<T> const &tree) {
        if (tree.empty()) {
            return os << "[]";
        }

        CircularQueue<Node const *> queue;
        queue.enqueue(tree.root);
        os << "[";
        bool first = true;

        while (!queue.empty()) {
            Node const *curr = queue.front();
            queue.dequeue();

            if (!first) {
                os << ' ';
            }
            first = false;

            if (curr) {
                os << curr->data;
                queue.enqueue(curr->left);
                queue.enqueue(curr->right);
            } else {
                os << '#';
            }
        }

        return os << "]";
    }

private:
    void swap(BinaryTree<T> &other) noexcept {
        std::swap(root, other.root);
        std::swap(size_, other.size_);
    }
};

#endif
