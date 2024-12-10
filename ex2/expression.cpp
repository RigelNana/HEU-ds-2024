/*
 * 表达式计算器和进程调度模拟程序
 * 包含以下主要功能：
 * 1. 算术表达式的中缀、前缀、后缀转换与计算
 * 2. 进程调度算法的模拟(FCFS和SJF)
 */

#include "../MyDS/arraylist.h"  // 引入自定义动态数组类
#include "../MyDS/arraystack.h" // 引入自定义栈类
#include <cctype>     // 用于字符类型判断
#include <iostream>   // 标准输入输出
#include <sstream>    // 字符串流处理
#include <stdexcept>  // 异常处理
#include <string>     // 字符串类
using namespace std;

/**
 * 获取操作符优先级
 * @param op 操作符
 * @return 优先级值(0-2)
 * 乘除模运算优先级为2
 * 加减运算优先级为1
 * 其他运算符优先级为0
 */
int get_priority(char op) {
    switch (op) {
    case '+':
    case '-': return 1;  // 加减法优先级为1
    case '*':
    case '/':
    case '%': return 2;  // 乘除模运算优先级为2
    default:  return 0;  // 其他字符优先级为0
    }
}

/**
 * 判断字符是否为操作符
 * @param op 待判断的字符
 * @return 是操作符返回true，否则返回false
 */
bool is_op(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '%';
}

/**
 * 执行基本算术运算
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @param op 运算符
 * @return 运算结果
 * @throws runtime_error 当除数为0或运算符未知时抛出异常
 */
int calc(int a, int b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (!b) throw runtime_error("divided by 0!!!!!!");
        return a / b;
    case '%':
        if (!b) throw runtime_error("divided by 0!!!!!!");
        return a % b;
    default: throw runtime_error("unknown operator");
    }
}

/**
 * 中缀表达式转后缀表达式(逆波兰表示法)
 * 算法步骤：
 * 1. 遇到数字直接输出
 * 2. 遇到左括号压入栈中
 * 3. 遇到右括号，弹出栈中元素直到遇到左括号
 * 4. 遇到运算符，和栈顶运算符比较优先级：
 *    - 如果栈为空或栈顶为左括号，直接入栈
 *    - 否则将栈顶运算符弹出，直到遇到优先级更低的运算符
 */
string to_suffix(string const &s) {
    string res;  // 存储后缀表达式结果
    ArrayStack<char> op;  // 运算符栈
    
    // 遍历中缀表达式的每个字符
    for (int i = 0; i < (int)s.size(); ++i) {
        if (isspace(s[i])) continue;  // 跳过空白字符
        
        // 处理数字
        if (isdigit(s[i])) {
            while (i < (int)s.size() && isdigit(s[i])) {
                res += s[i++];  // 将连续的数字字符加入结果
            }
            res += ' ';  // 数字之后添加空格分隔
            i--;  // 回退一个位置，因为for循环会自增
        }
        // 处理左括号
        else if (s[i] == '(') {
            op.push(s[i]);
        }
        // 处理右括号
        else if (s[i] == ')') {
            // 将栈中运算符弹出，直到遇到左括号
            while (!op.empty() && op.top() != '(') {
                res += op.top();
                res += ' ';
                op.pop();
            }
            if (!op.empty()) op.pop();  // 弹出左括号
        }
        // 处理运算符
        else if (is_op(s[i])) {
            // 将栈中优先级大于等于当前运算符的运算符弹出
            while (!op.empty() && op.top() != '(' &&
                   get_priority(op.top()) >= get_priority(s[i])) {
                res += op.top();
                res += ' ';
                op.pop();
            }
            op.push(s[i]);
        }
    }
    
    // 将栈中剩余运算符弹出
    while (!op.empty()) {
        res += op.top();
        res += ' ';
        op.pop();
    }
    
    return res;
}

/**
 * 中缀表达式转前缀表达式
 * 算法步骤：
 * 1. 将中缀表达式反转
 * 2. 交换左右括号
 * 3. 转换为后缀表达式
 * 4. 将结果再次反转得到前缀表达式
 */
string to_prefix(string s) {
    reverse(s.begin(), s.end());  // 反转字符串
    // 交换左右括号
    for (char &c: s) {
        if (c == '(') c = ')';
        else if (c == ')') c = '(';
    }
    string prefix = to_suffix(s);  // 转换为后缀表达式
    reverse(prefix.begin(), prefix.end());  // 再次反转
    return prefix;
}

/**
 * 计算后缀表达式的值
 * 算法步骤：
 * 1. 遇到数字压入栈中
 * 2. 遇到运算符，弹出两个操作数进行计算，结果压回栈中
 * 3. 最后栈顶元素即为表达式的值
 */
int eval_suffix(string const &s) {
    ArrayStack<int> nums;  // 操作数栈
    string token;
    istringstream d(s);  // 用于分割字符串
    
    // 读取每个标记(数字或运算符)
    while (d >> token) {
        // 处理数字
        if (isdigit(token[0])) {
            nums.push(stoi(token));
        }
        // 处理运算符
        else if (token.size() == 1 && is_op(token[0])) {
            if (nums.size() < 2) {
                throw runtime_error("Invalid expression");
            }
            int b = nums.top(); nums.pop();
            int a = nums.top(); nums.pop();
            nums.push(calc(a, b, token[0]));
        }
    }
    return nums.empty() ? 0 : nums.top();
}

/**
 * 直接计算中缀表达式的值
 * 实现方法：先转换为后缀表达式，再计算后缀表达式的值
 * @param s 中缀表达式字符串
 * @return 表达式计算结果
 */
int eval(string const &s) {
    return eval_suffix(to_suffix(s));
}

/**
 * 任务(进程)结构体
 * 用于模拟操作系统进程调度的基本数据结构
 * 包含进程的各项基本属性和时间信息
 */
struct Task {
    int id;          // 任务唯一标识符
    int cpu_time;    // 任务需要的CPU执行时间
    int submit_time; // 任务提交时间
    int start_time;  // 任务开始执行时间
    int end_time;    // 任务结束时间
    int wait_time;   // 任务等待时间(开始时间-提交时间)

    /**
     * 任务构造函数
     * @param i 任务ID
     * @param c CPU时间
     * @param s 提交时间
     */
    Task(int i, int c, int s)
        : id(i),
          cpu_time(c),
          submit_time(s),
          start_time(0),
          end_time(0),
          wait_time(0) {}

    /**
     * 默认构造函数
     * 所有属性初始化为0
     */
    Task()
        : id(0),
          cpu_time(0),
          submit_time(0),
          start_time(0),
          end_time(0),
          wait_time(0) {}
};

/**
 * 计算任务列表的平均等待时间
 * 算法步骤：
 * 1. 按照任务顺序模拟执行过程
 * 2. 计算每个任务的等待时间
 * 3. 返回平均等待时间
 * @param tasks 任务列表
 * @return 平均等待时间
 */
double average_waiting_time(ArrayList<Task> &tasks) {
    int current_time = 0;  // 当前系统时间
    double total_wait_time = 0;  // 总等待时间
    
    for (auto &task: tasks) {
        // 更新当前时间(考虑任务提交时间)
        current_time = max(current_time, task.submit_time);
        task.start_time = current_time;  // 设置开始时间
        task.end_time = current_time + task.cpu_time;  // 计算结束时间
        task.wait_time = task.start_time - task.submit_time;  // 计算等待时间
        total_wait_time += task.wait_time;  // 累加等待时间
        current_time = task.end_time;  // 更新当前时间
    }
    
    return total_wait_time / tasks.size();  // 返回平均等待时间
}

/**
 * 最短作业优先(SJF)调度算法
 * 算法原理：
 * 1. 将任务按CPU执行时间从小到大排序
 * 2. 按排序后的顺序执行任务
 * 特点：
 * - 最小化平均等待时间
 * - 可能导致长作业饥饿
 * @param tasks 任务列表
 * @return 平均等待时间
 */
double SJF(ArrayList<Task> &tasks) {
    // 按CPU时间排序(lambda表达式比较)
    sort(tasks.begin(), tasks.end(),
         [](Task const &a, Task const &b) { return a.cpu_time < b.cpu_time; });
    return average_waiting_time(tasks);
}

/**
 * 先来先服务(FCFS)调度算法
 * 算法原理：
 * 1. 按任务提交时间排序
 * 2. 相同提交时间的按任务ID排序
 * 3. 按排序后的顺序执行任务
 * 特点：
 * - 实现简单公平
 * - 对短作业不利
 * @param tasks 任务列表
 * @return 平均等待时间
 */
double FCFS(ArrayList<Task> &tasks) {
    // 先按提交时间排序，时间相同则按ID排序
    sort(tasks.begin(), tasks.end(), [](Task const &a, Task const &b) {
        if (a.submit_time == b.submit_time) {
            return a.id < b.id;
        }
        return a.submit_time < b.submit_time;
    });
    return average_waiting_time(tasks);
}

/**
 * 打印任务调度结果
 * 显示每个任务的详细信息和平均等待时间
 * @param tasks 任务列表
 * @param schedule_type 调度算法类型
 */
void print_task_schedule(ArrayList<Task> const &tasks,
                         string const &schedule_type) {
    double avg_wait = 0;
    // 打印每个任务的详细信息
    for (auto const &task: tasks) {
        avg_wait += task.wait_time;
        cout << "Task " << task.id 
             << ": Start=" << task.start_time
             << ", Submit=" << task.submit_time 
             << ", CPU=" << task.cpu_time
             << ", End=" << task.end_time 
             << ", Wait=" << task.wait_time
             << '\n';
    }
    // 打印平均等待时间
    cout << "Average Wait Time: " << avg_wait / tasks.size() << "\n\n";
}

/**
 * 主函数
 * 实现两个主要功能：
 * 1. 表达式计算
 * 2. 进程调度模拟
 */
int main() {
    cout << "输入: 0: 表达式, 1: 任务调度\n";
    int n;
    cin >> n;
    cin.ignore();  // 清除输入缓冲区
    
    // 表达式计算功能
    if (n == 0) {
        string expr;
        getline(cin, expr);  // 读取表达式
        cout << "前缀：" << to_prefix(expr) << '\n';  // 转换并显示前缀表达式
        cout << "后缀：" << to_suffix(expr) << '\n';  // 转换并显示后缀表达式
        cout << expr << " = " << eval(expr);  // 计算并显示结果
        return 0;
    }
    
    // 任务调度模拟功能
    if (n == 1) {
        cout << "请输入个数:";
        int m;
        cin >> m;
        ArrayList<Task> tasks;
        // 读入任务信息
        for (int i = 0; i < m; ++i) {
            int id, cpu, submit;
            cin >> id >> cpu >> submit;
            tasks.push_back(Task(id, cpu, submit));
        }

        // 创建任务列表副本用于不同调度算法
        ArrayList<Task> fcfs_tasks = tasks;
        ArrayList<Task> sjf_tasks = tasks;

        // 执行并显示FCFS调度结果
        cout << "\n== First Come First Serve (FCFS) ==\n";
        FCFS(fcfs_tasks);
        print_task_schedule(fcfs_tasks, "FCFS");

        // 执行并显示SJF调度结果
        cout << "\n== Shortest Job First (SJF) ==\n";
        SJF(sjf_tasks);
        print_task_schedule(sjf_tasks, "SJF");
    } else {
        cout << "输入错误\n";
    }
    return 0;
}