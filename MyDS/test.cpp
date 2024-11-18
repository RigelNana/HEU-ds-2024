#include "arraylist.h"
#include <cassert>
#include <iostream>
#include <string>

// 用于测试的辅助函数
template <typename T>
void printList(ArrayList<T> const &list, std::string const &msg = "") {
    if (!msg.empty()) {
        std::cout << msg << ": ";
    }
    std::cout << list << std::endl;
}

void testBasicOperations() {
    std::cout << "\n=== Testing Basic Operations ===\n";

    ArrayList<int> list;
    assert(list.empty());
    assert(list.size() == 0);

    // 测试push_back和基本访问
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    printList(list, "After push_back");
    assert(list.size() == 3);
    assert(!list.empty());
    assert(list.front() == 10);
    assert(list.back() == 30);

    // 测试at()
    assert(list.at(0) == 10);
    assert(list.at(1) == 20);
    assert(list.at(2) == 30);

    // 测试pop_back
    list.pop_back();
    printList(list, "After pop_back");
    assert(list.size() == 2);
    assert(list.back() == 20);
}

void testInsertAndErase() {
    std::cout << "\n=== Testing Insert and Erase ===\n";

    ArrayList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    // 测试insert
    list.insert(1, 10);
    printList(list, "After insert at position 1");
    assert(list.size() == 4);
    assert(list.at(1) == 10);

    // 测试erase
    list.erase(1);
    printList(list, "After erase at position 1");
    assert(list.size() == 3);
    assert(list.at(1) == 2);
}

void testCopyAndMove() {
    std::cout << "\n=== Testing Copy and Move Operations ===\n";

    ArrayList<int> list1;
    list1.push_back(1);
    list1.push_back(2);

    // 测试拷贝构造
    ArrayList<int> list2(list1);
    printList(list2, "Copy constructed list");
    assert(list2.size() == list1.size());
    assert(list2.at(0) == list1.at(0));

    // 测试移动构造
    ArrayList<int> list3(std::move(list1));
    printList(list3, "Move constructed list");
    assert(list1.empty()); // 原list应该为空
    assert(list3.size() == 2);
}

void testExceptionHandling() {
    std::cout << "\n=== Testing Exception Handling ===\n";

    ArrayList<int> list;

    // 测试空列表的异常
    try {
        list.front();
        assert(false); // 不应该到达这里
    } catch (std::runtime_error const &e) {
        std::cout << "Expected error caught: " << e.what() << std::endl;
    }

    // 测试越界访问的异常
    list.push_back(1);
    try {
        list.at(5);
        assert(false); // 不应该到达这里
    } catch (std::out_of_range const &e) {
        std::cout << "Expected error caught: " << e.what() << std::endl;
    }
}

void testReplace() {
    std::cout << "\n=== Testing Replace Operation ===\n";

    ArrayList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.replace(1, 20);
    printList(list, "After replacing position 1 with 20");
    assert(list.at(1) == 20);
}

int main() {
    try {
        testBasicOperations();
        testInsertAndErase();
        testCopyAndMove();
        testExceptionHandling();
        testReplace();

        std::cout << "\nAll tests passed successfully!\n";
    } catch (std::exception const &e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
