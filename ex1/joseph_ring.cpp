#include "../MyDS/arraylist.h"
#include "../MyDS/circularlist.h"
#include <iostream>
using namespace std;

struct Person {
    int id;
    int password;

    Person(int id = 0, int password = 0) : id(id), password(password) {}

    friend ostream &operator<<(ostream &os, Person const &p) {
        return os << p.id;
    }
};

int main() {
    int n;
    cin >> n;
    ArrayList<int> password;
    for (int i = 0; i < n; ++i) {
        int temp;
        cin >> temp;
        password.push_back(temp);
    }
    ArrayList<Person> result;
    CircularList<Person> circle;
    for (int i = 1; i <= n; ++i) {
        circle.push_back(Person(i, password[i - 1]));
    }
    int current = 0;
    while (!circle.empty()) {
        int m = circle.at(current).password;
        current = (current + m - 1) % circle.size();
        result.push_back(circle.at(current));
        circle.erase(current);
        if (current == circle.size()) {
            current = 0;
        }
    }
    cout << result;
}
