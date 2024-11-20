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

struct Task {
    int id;          // 任务ID
    int cpu_time;    // 所需CPU时间
    int submit_time; // 提交时间
    int start_time;  // 开始时间
    int end_time;    // 结束时间
    int wait_time;   // 等待时间

    Task(int i, int c, int s)
        : id(i),
          cpu_time(c),
          submit_time(s),
          start_time(0),
          end_time(0),
          wait_time(0) {}

    Task()
        : id(0),
          cpu_time(0),
          submit_time(0),
          start_time(0),
          end_time(0),
          wait_time(0) {}
};

double average_waiting_time(ArrayList<Task> &tasks) {
    int current_time = 0;
    double total_wait_time = 0;
    for (auto &task: tasks) {
        current_time = max(current_time, task.submit_time);
        task.start_time = current_time;
        task.end_time = current_time + task.cpu_time;
        task.wait_time = task.start_time - task.submit_time;
        total_wait_time += task.wait_time;
        current_time = task.end_time;
    }
    return total_wait_time / tasks.size();
}

double SJF(ArrayList<Task> &tasks) {
    sort(tasks.begin(), tasks.end(),
         [](Task const &a, Task const &b) { return a.cpu_time < b.cpu_time; });
    return average_waiting_time(tasks);
}

double FCFS(ArrayList<Task> &tasks) {
    sort(tasks.begin(), tasks.end(), [](Task const &a, Task const &b) {
        if (a.submit_time == b.submit_time) {
            return a.id < b.id;
        }
        return a.submit_time < b.submit_time;
    });
    return average_waiting_time(tasks);
}

void print_task_schedule(ArrayList<Task> const &tasks,
                         string const &schedule_type) {
    double avg_wait = 0;
    for (auto const &task: tasks) {
        avg_wait += task.wait_time;
        cout << "Task " << task.id << ": Start=" << task.start_time
             << ", Submit=" << task.submit_time << ", CPU=" << task.cpu_time
             << ", End=" << task.end_time << ", Wait=" << task.wait_time
             << '\n';
    }
    cout << "Average Wait Time: " << avg_wait / tasks.size() << "\n\n";
}

int main() {
    ArrayList<Task> tasks = {Task{1, 10, 0}, Task{2, 8, 1}, Task{3, 2, 2},
                             Task{4, 1, 3}, Task{5, 3, 4}};

    // 创建副本用于不同的调度算法
    ArrayList<Task> fcfs_tasks = tasks;
    ArrayList<Task> sjf_tasks = tasks;

    // FCFS调度
    cout << "\n== First Come First Serve (FCFS) ==\n";
    FCFS(fcfs_tasks);
    print_task_schedule(fcfs_tasks, "FCFS");

    // SJF调度
    cout << "\n== Shortest Job First (SJF) ==\n";
    SJF(sjf_tasks);
    print_task_schedule(sjf_tasks, "SJF");

    return 0;
}

// int main() {
//     // string expr;
//     // getline(std::cin, expr);
//     // cout << "前缀：" << to_prefix(expr) << '\n';
//     // cout << "后缀：" << to_suffix(expr) << '\n';
//     // cout << expr << '=' << eval(expr);
//     // ArrayList<int> al = {1, 2, 3, 4, 5};
//     // cout << al;
//     // sort(al.begin(), al.end(), [](int a, int b) { return a > b; });
//     // cout << '\n' << al;
//
// }
