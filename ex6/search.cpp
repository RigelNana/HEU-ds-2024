// ①问题描述
// 有 512 人参与玩某游戏，从 1~512 给每个人分配一个编号，每个人的游戏得
// 分在0~999 之间，现要用不同方法查找出游戏参与者的最高分和次高分。要求：
//  自行产生512 个的随机整数作为所有游戏参与者的得分。
//  输出所有游戏参与者（用编号表示）及其得分。
// 
// 用顺序查找方法查找出其中取得最高分和次高分者及其分数，并输出。
// 
// 锦标赛法查找出其中取得最高分和次高分者及其分数，并输出。
// 
// 通过无序序列建堆和堆调整得到取得最高分者和次高分者及其分数，并 输出。
//  比较不同方法的查找效率和各自的特点。

#include "../MyDS/arraylist.h"
#include "../MyDS/heap.h"
#include "../MyDS/pair.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <limits>
#include <random>
using namespace std;

int main() {
    std::mt19937 mt_rand(static_cast<unsigned int>(time(nullptr)));

    std::uniform_int_distribution<unsigned int> dist(0, 999);
    ArrayList<Pair<unsigned int, unsigned int>> scores(512);
    for (unsigned int i = 0; i < 512; i++) {
        scores.push_back(Pair(dist(mt_rand), i + 1));
    }

    for (int i = 0; i < 512; i++) {
        cout << i + 1 << " " << scores[i] << endl;
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto max1 = Pair(numeric_limits<unsigned int>::min(), 0u);
    auto max2 = Pair(numeric_limits<unsigned int>::min(), 0u);
    int max_index = -1;
    int max2_index = -1;

    cout << "顺序查找法" << endl;
    for (int i = 0; i < 512; i++) {
        if (scores[i].first > max1.first) {
            max1 = scores[i];
        }
    }
    auto scores1 = scores;
    scores1.erase(scores1.begin() + max1.second - 1);
    for (int i = 0; i < 511; i++) {
        if (scores1[i].first > max2.first) {
            max2 = scores1[i];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    cout << "最高分：" << max1.first << " 编号：" << max1.second << endl;
    cout << "次高分：" << max2.first << " 编号：" << max2.second << endl;
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "耗时：" << duration.count() << "微秒" << endl;

    start = std::chrono::high_resolution_clock::now();
    // 锦标赛法部分的修改
    cout << "锦标赛法" << endl;
    int n = 512;
    ArrayList<Pair<unsigned int, unsigned int>> a(2 * n, Pair(0u, 0u));

    // 初始化数组 - 将原始数据放入后n个位置
    for (int i = 0; i < n; i++) {
        a[n + i] = scores[i];
    }

    // 进行锦标赛 - 从最后一个非叶子节点开始向上构建
    for (int i = n - 1; i >= 1; i--) {
        if (a[i * 2].first >= a[i * 2 + 1].first) {
            a[i] = a[i * 2];
        } else {
            a[i] = a[i * 2 + 1];
        }
    }

    // 找出最高分
    max1 = a[1];

    // 找次高分：沿着最高分选手比赛的路径寻找
    max2 = Pair(numeric_limits<unsigned int>::min(), 0u);
    int idx = 1;
    while (idx < n) {
        int left = idx * 2;
        int right = left + 1;

        // 如果最大值来自左子树
        if (a[idx] == a[left]) {
            // 检查右子树的值是否可能是次大值
            if (a[right].first > max2.first && a[right].first <= max1.first) {
                max2 = a[right];
            }
            idx = left;
        }
        // 如果最大值来自右子树
        else {
            // 检查左子树的值是否可能是次大值
            if (a[left].first > max2.first && a[left].first <= max1.first) {
                max2 = a[left];
            }
            idx = right;
        }
    }

    end = std::chrono::high_resolution_clock::now();
    cout << "最高分：" << max1.first << " 编号：" << max1.second << endl;
    cout << "次高分：" << max2.first << " 编号：" << max2.second << endl;
    duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "耗时：" << duration.count() << "微秒" << endl;

    start = std::chrono::high_resolution_clock::now();
    cout << "堆排序法" << endl;

    Heap<Pair<unsigned int, unsigned int>,
         std::greater<Pair<unsigned int, unsigned int>>>
        heap(scores);

    max1 = heap.top();

    heap.pop();
    max2 = heap.top();
    heap.pop();
    end = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    cout << "最高分：" << max1.first << " 编号："<< max1.second << endl;
    cout << "次高分：" << max2.first << " 编号："<< max2.second << endl;

    cout << "耗时：" << duration.count() << "微秒" << endl;
    return 0;
}
