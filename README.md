# README

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

此仓库是一个数据结构和算法的集合，包含多个示例和实现，旨在帮助学习和理解常见的数据结构和算法。

## 目录

- [安装](#安装)
- [使用](#使用)
- [示例](#示例)
- [贡献](#贡献)
- [许可证](#许可证)

## 安装

### 先决条件

- [CMake](https://cmake.org/download/)
- [GCC](https://gcc.gnu.org/) 或 [Clang](https://clang.llvm.org/)

### 构建步骤

1. 克隆仓库：

    ```sh
    git clone https://github.com/RigelNana/HEU-ds-2024.git
    cd HEU-ds-2024
    ```

2. 创建构建目录并运行CMake：

    ```sh
    mkdir build
    cd build
    cmake ..
    ```

3. 编译项目：

    ```sh
    make
    ```

## 使用

构建完成后，您可以运行各个示例程序。

## 示例

### 约瑟夫环问题

`joseph_ring.cpp` 实现了经典的约瑟夫环问题，使用了自定义的 `ArrayList` 和 `CircularList` 数据结构。

### 表达式计算器和进程调度模拟

`expression.cpp` 包含了表达式计算器和进程调度模拟程序，展示了如何使用自定义的 `ArrayList` 和 `ArrayStack` 数据结构。

### 幻方和导师管理系统

`cube_teacher.cpp` 实现了幻方生成算法和导师管理系统，展示了如何使用自定义的 `ArrayList` 数据结构。

### 哈夫曼树压缩和解压缩

`hufftree.cpp` 实现了哈夫曼树的构建、编码、压缩和解压缩功能，展示了如何使用自定义的 `ArrayList`、`BinaryTree` 和 `Heap` 数据结构。

### 图算法

`graph_alg.cpp` 实现了图的广度优先搜索和深度优先搜索算法，展示了如何使用自定义的 `Graph` 数据结构。

### 搜索算法

`search.cpp` 实现了顺序查找、锦标赛法和堆排序法查找最高分和次高分，展示了如何使用自定义的 `ArrayList` 和 `Heap` 数据结构。

### 学生成绩管理系统

`schedule.cpp` 实现了学生成绩管理系统，展示了如何使用自定义的 `ArrayList` 数据结构。

### MyDS 数据结构库

MyDS 数据结构库包含以下数据结构的实现：

- ArrayList：动态数组实现，支持基本的列表操作。
- ArrayStack：基于数组的栈实现。
- BinaryTree：二叉树实现，支持前序、中序、后序遍历和广度优先搜索。
- CircularList：循环链表实现。
- CircularQueue：循环队列实现。
- Graph：图的实现，支持有向图和无向图，提供深度优先搜索、广度优先搜索和Dijkstra算法。
- Heap：堆的实现，支持最大堆和最小堆。
- LinkedList：双向链表实现。
- Map：基于动态数组的映射实现。
- Pair：键值对实现。
- Set：基于红黑树的集合实现。

## 贡献

欢迎贡献！请遵循以下步骤：

1.Fork 仓库
2.创建您的功能分支 (git checkout -b feature/AmazingFeature)
3.提交您的更改 (git commit -m 'Add some AmazingFeature')
4.推送到分支 (git push origin feature/AmazingFeature)
5.打开一个 Pull Request

## 许可证

此项目基于 MIT 许可证。
