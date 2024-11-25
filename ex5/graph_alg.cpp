#include "../MyDS/arraylist.h"
#include "../MyDS/circularqueue.h"
#include "../MyDS/graph.h"
#include "../MyDS/map.h"
#include <iostream>
using namespace std;

struct state {
    int lm, lc, pos;

    state(int lm, int lc, int pos) : lm(lm), lc(lc), pos(pos) {}

    bool operator<(state const &other) const {
        if (lm != other.lm) {
            return lm < other.lm;
        }
        if (lc != other.lc) {
            return lc < other.lc;
        }
        return pos < other.pos;
    }

    bool valid(int n) const {
        int rm = n - lm, rc = n - lc;
        if (lm < 0 || lc < 0 || rm < 0 || rc < 0) {
            return false;
        }
        if (lm && lm < lc) {
            return false;
        }
        if (rm && rm < rc) {
            return false;
        }
        return true;
    }
};

struct boat {
    int m, c;

    boat(int m, int c) : m(m), c(c) {}
};

int main() {
    int n, m, c;
    cin >> n >> m >> c;
    Map<state, state> prev;
    Map<state, boat> prev_boat;
    CircularQueue<state> q;
}
