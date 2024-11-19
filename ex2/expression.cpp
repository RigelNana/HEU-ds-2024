#include "../MyDS/arraylist.h"
#include "../MyDS/arraystack.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
using namespace std;

int get_priority(char op) {
    switch (op) {
    case '+':
    case '-': return 1;
    case '*':
    case '/':
    case '%': return 2;
    default:  return 0;
    }
}

bool is_op(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '%';
}

int calc(int a, int b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (!b) {
            throw runtime_error("divided by 0!!!!!!");
        }
        return a / b;
    case '%':
        if (!b) {
            throw runtime_error("divided by 0!!!!!!");
        }
        return a % b;
    default: throw runtime_error("unknown operator");
    }
}

string to_suffix(string const &s) {
    string res;
    ArrayStack<char> op;
    for (int i = 0; i < (int)s.size(); ++i) {
        if (isspace(s[i])) {
            continue;
        }
        if (isdigit(s[i])) {
            while (i < (int)s.size() && isdigit(s[i])) {
                res += s[i++];
            }
            res += ' ';
            i--;
        } else if (s[i] == '(') {
            op.push(s[i]);
        } else if (s[i] == ')') {
            while (!op.empty() && op.top() != '(') {
                res += op.top();
                res += ' ';
                op.pop();
            }
            if (!op.empty()) {
                op.pop();
            }
        } else if (is_op(s[i])) {
            while (!op.empty() && op.top() != '(' &&
                   get_priority(op.top()) >= get_priority(s[i])) {
                res += op.top();
                res += ' ';
                op.pop();
            }
            op.push(s[i]);
        }
    }
    while (!op.empty()) {
        res += op.top();
        res += ' ';
        op.pop();
    }

    return res;
}

string to_prefix(string s) {
    reverse(s.begin(), s.end());
    for (char &c: s) {
        if (c == '(') {
            c = ')';
        } else if (c == ')') {
            c = '(';
        }
    }
    string prefix = to_suffix(s);
    reverse(prefix.begin(), prefix.end());
    return prefix;
}

int eval_suffix(string const &s) {
    ArrayStack<int> nums;
    string token;
    istringstream d(s);
    while (d >> token) {
        if (isdigit(token[0])) {
            nums.push(stoi(token));
        } else if (token.size() == 1 && is_op(token[0])) {
            if (nums.size() < 2) {
                throw runtime_error("Invalid expression");
            }
            int b = nums.top();
            nums.pop();
            int a = nums.top();
            nums.pop();
            nums.push(calc(a, b, token[0]));
        }
    }
    return nums.empty() ? 0 : nums.top();
}

int eval_prefix(string const &s) {
    ArrayList<string> tokens;
    string token;
    istringstream d(s);
    while (d >> token) {
        tokens.push_back(token);
    }

    ArrayStack<int> nums;
    for (int i = 0; i < tokens.size(); ++i) {
        if (isdigit(tokens[i][0])) {
            nums.push(stoi(tokens[i]));
        } else if (tokens[i].size() == 1 && is_op(tokens[i][0])) {
            if (nums.size() < 2) {
                throw runtime_error("Invalid expression");
            }
            int a = nums.top();
            nums.pop();
            int b = nums.top();
            nums.pop();
            nums.push(calc(a, b, tokens[i][0]));
        }
    }
    return nums.empty() ? 0 : nums.top();
}

int eval(string const &s) {
    return eval_suffix(to_suffix(s));
}

int main() {
    string expr;
    getline(std::cin, expr);
    cout << "前缀：" << to_prefix(expr) << '\n';
    cout << "后缀：" << to_suffix(expr) << '\n';
    cout << expr << '=' << eval(expr);
}
