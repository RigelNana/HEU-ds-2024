#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#ifdef __cplusplus
# include <cstddef>
# include <ostream>
# include <stdexcept>
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
};

#endif
