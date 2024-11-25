#ifndef SET_H
#define SET_H

#ifdef __cplusplus
# include <algorithm>
# include <initializer_list>
# include <ostream>
# include <stdexcept>

#endif
template <typename T>
class Set {
private:
    enum Color {
        RED,
        BLACK
    };

    struct Node {
        T value;
        Node *left;
        Node *right;
        Node *parent;
        Color color;

        Node(T const &value, Color c = RED)
            : value(value),
              left(nullptr),
              right(nullptr),
              parent(nullptr),
              color(c) {}

        Node()
            : value(),
              left(nullptr),
              right(nullptr),
              parent(nullptr),
              color(RED) {}
    };

    Node *root;
    Node *nil;
    size_t _size;

    void rotate_left(Node *x) {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != nil) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rotate_right(Node *x) {
        Node *y = x->left;
        x->left = y->right;
        if (y->right != nil) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void insert_fix(Node *z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node *y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotate_left(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotate_right(z->parent->parent);
                }
            } else {
                Node *y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotate_right(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotate_left(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void erase_fix(Node *x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node *w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotate_left(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rotate_right(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    rotate_left(x->parent);
                    x = root;
                }
            } else {
                Node *w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotate_right(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        rotate_left(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rotate_right(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    Node *find_min(Node *node) const {
        while (node->left != nil) {
            node = node->left;
        }
        return node;
    }

    void transplant(Node *u, Node *v) {
        if (u->parent == nil) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void clear(Node *node) {
        if (node != nil) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    Node *copy(Node *node, Node *parent, Node *other_nil) {
        if (node == other_nil) {
            return nil;
        }
        Node *new_node = new Node(node->value, node->color);
        new_node->parent = parent;
        new_node->left = copy(node->left, new_node, other_nil);
        new_node->right = copy(node->right, new_node, other_nil);
        return new_node;
    }

    void inorder(Node *node, std::ostream &os) const {
        if (node != nil) {
            inorder(node->left, os);
            os << node->value;
            if (node->right != nil) {
                os << ' ';
            }
            inorder(node->right, os);
        }
    }

public:
    Set() : _size(0) {
        nil = new Node(T(), BLACK);
        root = nil;
    }

    Set(std::initializer_list<T> init) : Set() {
        for (auto const &value: init) {
            insert(value);
        }
    }

    Set(Set const &other) : Set() {
        root = copy(other.root, nil, other.nil);
        _size = other._size;
    }

    Set(Set &&other) noexcept
        : root(other.root),
          nil(other.nil),
          _size(other._size) {
        other.root = nullptr;
        other.nil = nullptr;
        other._size = 0;
    }

    ~Set() {
        clear(root);
        delete nil;
    }

    Set &operator=(Set const &other) {
        if (this != &other) {
            Set temp(other);
            std::swap(root, temp.root);
            std::swap(nil, temp.nil);
            std::swap(_size, temp._size);
        }
        return *this;
    }

    Set &operator=(Set &&other) noexcept {
        if (this != &other) {
            clear(root);
            delete nil;
            root = other.root;
            nil = other.nil;
            _size = other._size;
            other.root = nullptr;
            other.nil = nullptr;
            other._size = 0;
        }
        return *this;
    }

    void insert(T const &value) {
        Node *z = new Node(value);
        Node *y = nil;
        Node *x = root;

        while (x != nil) {
            y = x;
            if (value < x->value) {
                x = x->left;
            } else if (value > x->value) {
                x = x->right;
            } else {
                delete z;
                return;
            }
        }

        z->parent = y;
        if (y == nil) {
            root = z;
        } else if (value < y->value) {
            y->left = z;
        } else {
            y->right = z;
        }

        z->left = nil;
        z->right = nil;
        z->color = RED;

        insert_fix(z);
        _size++;
    }

    void erase(T const &value) {
        Node *z = root;
        while (z != nil) {
            if (value < z->value) {
                z = z->left;
            } else if (value > z->value) {
                z = z->right;
            } else {
                break;
            }
        }
        if (z == nil) {
            return;
        }

        Node *y = z;
        Node *x;
        Color y_original_color = y->color;

        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = find_min(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        if (y_original_color == BLACK) {
            erase_fix(x);
        }
        _size--;
    }

    bool contains(T const &value) const {
        Node *current = root;
        while (current != nil) {
            if (value < current->value) {
                current = current->left;
            } else if (value > current->value) {
                current = current->right;
            } else {
                return true;
            }
        }
        return false;
    }

    bool empty() const noexcept {
        return _size == 0;
    }

    size_t size() const noexcept {
        return _size;
    }

    void clear() {
        clear(root);
        root = nil;
        _size = 0;
    }

    friend std::ostream &operator<<(std::ostream &os, Set const &set) {
        os << "{";
        if (!set.empty()) {
            set.inorder(set.root, os);
        }
        os << "}";
        return os;
    }
};
#endif // !SET_H
