#include "../MyDS/arraylist.h"
#include "../MyDS/circularqueue.h"
#include "../MyDS/graph.h"
#include "../MyDS/map.h"
#include "../MyDS/set.h"
#include <iostream>
using namespace std;

struct state {
    int lm, lc, pos;

    state() : lm(0), lc(0), pos(0) {}

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

    bool operator>(state const &other) const {
        return other < *this;
    }

    bool operator==(state const &other) const {
        return lm == other.lm && lc == other.lc && pos == other.pos;
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
    boat() = default;

    boat(int m, int c) : m(m), c(c) {}
};

int main() {
    int n, c;
    cin >> n >> c;
    if (c < 2) {
        cout << "至少两人" << endl;
        return 0;
    }
    Map<state, state> prev;
    Map<state, boat> prev_boat;
    CircularQueue<state> q;
    Set<state> vis;
    bool flag = false;
    state st(n, n, 0);
    q.enqueue(st);
    vis.insert(st);
    while (!q.empty()) {
        state cur = q.front();
        q.dequeue();
        if (cur.lm == 0 && cur.lc == 0) {
            flag = true;
            break;
        }
        for (int m = 0; m <= c; m++) {
            for (int cc = 0; cc <= c - m; cc++) {
                if (!m && !cc) {
                    continue;
                }
                if (m + cc > c) {
                    continue;
                }
                state nxt = cur;
                if (cur.pos == 0) {
                    nxt.lm -= m;
                    nxt.lc -= cc;
                } else {
                    nxt.lm += m;
                    nxt.lc += cc;
                }
                nxt.pos ^= 1;
                if (!nxt.valid(n) || vis.contains(nxt)) {
                    continue;
                }
                vis.insert(nxt);

                prev[nxt] = cur;
                prev_boat[nxt] = boat(m, cc);
                q.enqueue(nxt);
            }
        }
    }
    ArrayList<Pair<state, boat>> path;
    state cur(0, 0, 1);
    while (cur.lm != n || cur.lc != n || cur.pos != 0) {
        path.push_back(Pair(cur, prev_boat[cur]));
        cur = prev[cur];
    }
    reverse(path.begin(), path.end());
    if (!flag) {
        cout << "无解\n";
        return 0;
    }
    cout << "\n找到解决方案，共" << (int)path.size() << "步：\n\n";
    cout << "初始状态：左岸" << "(" << n << "," << n << ")" << "右岸" << "("
         << 0 << "," << 0 << ")";

    for (int i = 0; i < path.size(); i++) {
        state s = path[i].first;
        boat m = path[i].second;
        cout << "\n步骤" << i + 1;
        if (s.pos) {
            cout << ":船载" << " (" << m.m << "," << m.c << ") "
                 << "从左岸到右岸\n";
        } else {
            cout << ":船载" << " (" << m.m << "," << m.c << ") "
                 << "从右岸到左岸\n";
        }
        cout << "状态：左岸" << "(" << s.lm << "," << s.lc << ") " << "右岸"
             << "(" << n - s.lm << "," << n - s.lc << ")";
    }
}
