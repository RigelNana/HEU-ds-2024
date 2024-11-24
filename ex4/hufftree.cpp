#include "../MyDS/arraylist.h"
#include "../MyDS/binarytree.h"
#include "../MyDS/heap.h"
#include "../MyDS/map.h"
#include <fstream>
#include <iostream>
using namespace std;

class HuffmanTree {
public:
    struct Node {
        unsigned char data;
        size_t frequency;
        Node *left;
        Node *right;

        explicit Node(unsigned char d = '\0', size_t freq = 0)
            : data(d),
              frequency(freq),
              left(nullptr),
              right(nullptr) {}

        ~Node() {
            delete left;
            delete right;
        }

        bool operator<(Node const &other) const {
            return frequency < other.frequency;
        }

        bool operator==(Node const &other) const {
            return frequency == other.frequency && data == other.data;
        }
    };

    HuffmanTree() : root(nullptr) {}

    ~HuffmanTree() {
        delete root;
    }

    void build_tree(string const &filename) {
        auto freq = count_freq(filename);
        auto comp = [](Node *a, Node *b) {
            return a->frequency < b->frequency;
        };
        Heap<Node *, decltype(comp)> heap(comp);
        for (auto const &pair: freq) {
            heap.push(new Node(pair.first, pair.second));
        }

        while (heap.size() > 1) {
            Node *left = heap.top();
            heap.pop();
            Node *right = heap.top();
            heap.pop();
            Node *parent = new Node('\0', left->frequency + right->frequency);
            parent->left = left;
            parent->right = right;
            heap.push(parent);
        }
        root = heap.top();
        generate_codes(root, "");
    }

    void save_codes(string const &filename) {
        ofstream file(filename, ios::binary);
        for (auto const &pair: codes) {
            file << pair.first << ":" << pair.second << '\n';
        }
        file.close();
    }

    void compress(string const &input_file, string const &output_file) {
        ifstream in(input_file, ios::binary);
        ofstream out(output_file, ios::binary);

        unsigned char buf = 0;
        size_t count = 0;
        unsigned char ch;
        while (in.read(reinterpret_cast<char *>(&ch), sizeof(ch))) {
            string &cd = codes[ch];
            for (char bit: cd) {
                buf = (buf << 1) | (bit - '0');
                count++;
                if (count == 8) {
                    out.write(reinterpret_cast<char *>(&buf), sizeof(buf));
                    buf = 0;
                    count = 0;
                }
            }
        }
        if (count > 0) {
            buf <<= 8 - count;
            out.write(reinterpret_cast<char *>(&buf), sizeof(buf));
        }
        in.close();
        out.close();
    }

    void decompress(string const &input_file, string const &output_file) {
        ifstream in(input_file, ios::binary);
        ofstream out(output_file, ios::binary);
        Node *node = root;
        unsigned char buf;
        while (in.read(reinterpret_cast<char *>(&buf), sizeof(buf))) {
            for (int i = 7; i >= 0; i--) {
                bool bit = (buf >> i) & 1;
                node = bit ? node->right : node->left;
                if (node->left == nullptr && node->right == nullptr) {
                    out.write(reinterpret_cast<char const *>(&node->data),
                              sizeof(node->data));
                    node = root;
                }
            }
        }
        in.close();
        out.close();
    }

private:
    Node *root;

    Map<unsigned char, string> codes;

    Map<unsigned char, size_t> count_freq(string const &filename) {
        Map<unsigned char, size_t> freq;
        ifstream file(filename, ios::binary);
        unsigned char ch;
        while (file.read(reinterpret_cast<char *>(&ch), sizeof(ch))) {
            freq[ch]++;
        }

        file.close();
        return freq;
    }

    void generate_codes(Node *node, string const &code) {
        if (node == nullptr) {
            return;
        }
        if (node->left == nullptr && node->right == nullptr) {
            codes[node->data] = code;
            return;
        }
        generate_codes(node->left, code + "0");
        generate_codes(node->right, code + "1");
    }
};

int main() {
    HuffmanTree tree;
    ifstream test("input.txt");
    if (!test.good()) {
        cerr << "Error: Cannot open input.txt" << endl;
        return 1;
    }
    tree.build_tree("input.txt");
    tree.save_codes("codes.txt");
    tree.compress("input.txt", "output.txt");
    tree.decompress("output.txt", "decompressed.txt");
    return 0;
}
