#include "../MyDS/arraylist.h"
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string name;

public:
    Person(string n) : name(n) {}

    virtual ~Person() {}

    string get_name() const {
        return name;
    }

    virtual void show() const = 0;
};

class Undergraduate : public Person {
    string class_;

public:
    Undergraduate(string n, string c) : Person(n), class_(c) {}

    void show() const override {
        cout << "本科生: " << name << ", 班级: " << class_ << endl;
    }
};

class Graduate : public Person {
    string class_;
    ArrayList<Undergraduate *> undergrads;

public:
    Graduate(string n, string c) : Person(n), class_(c) {}

    ~Graduate() {
        for (auto u: undergrads) {
            delete u;
        }
    }

    void add_grad(Undergraduate *undergrad) {
        undergrads.push_back(undergrad);
    }

    void remove_grad(string name) {
        for (auto it = undergrads.begin(); it != undergrads.end(); it++) {
            if ((*it)->get_name() == name) {
                delete *it;
                undergrads.erase(it);
                break;
            }
        }
    }

    ArrayList<Undergraduate *> &get_undergrads() {
        return undergrads;
    }

    void show() const override {
        cout << "研究生: " << name << ", 班级: " << class_ << endl;
        cout << "指导的本科生:" << endl;
        for (auto u: undergrads) {
            u->show();
        }
    }
};

class Tutor : public Person {
    string title;
    ArrayList<Undergraduate *> undergrads;
    ArrayList<Graduate *> grads;

public:
    Tutor(string n, string t) : Person(n), title(t) {}

    ~Tutor() {
        for (auto g: grads) {
            delete g;
        }
        for (auto u: undergrads) {
            delete u;
        }
    }

    void add_grad(Graduate *g) {
        grads.push_back(g);
    }

    void add_undergrad(Undergraduate *u) {
        undergrads.push_back(u);
    }

    void remove_grad(string name) {
        for (auto it = grads.begin(); it != grads.end(); it++) {
            if ((*it)->get_name() == name) {
                auto &new_under = (*it)->get_undergrads();
                if (grads.size() == 1) {
                    for (auto u: new_under) {
                        undergrads.push_back(u);
                    }
                } else {
                    int index = 0;
                    for (auto u: new_under) {
                        auto target_grad = grads[index % grads.size()];
                        if (target_grad == *it) {
                            index++;
                            target_grad = grads[index % grads.size()];
                        }
                        target_grad->add_grad(u);
                        index++;
                    }
                }
                new_under.clear();
                delete *it;
                grads.erase(it);
                break;
            }
        }
    }

    void remove_undergrad(string name) {
        for (auto it = undergrads.begin(); it != undergrads.end(); ++it) {
            if ((*it)->get_name() == name) {
                delete *it;
                undergrads.erase(it);
                break;
            }
        }
    }

    void show() const override {
        cout << "导师: " << name << ", 职称: " << title << endl;
        cout << "指导的研究生:" << endl;
        for (auto g: grads) {
            g->show();
        }
        cout << "直接指导的本科生:" << endl;
        for (auto u: undergrads) {
            u->show();
        }
    }

    void statistics() const {
        int grad_count = grads.size();
        int undergrad_count = undergrads.size();
        for (auto g: grads) {
            undergrad_count += g->get_undergrads().size();
        }
        cout << "导师 " << name << " 共指导:" << endl;
        cout << "研究生: " << grad_count << " 人" << endl;
        cout << "本科生: " << undergrad_count << " 人" << endl;
    }

    ArrayList<Graduate *> const &get_grads() const {
        return grads;
    }
};

class TutorSystem {
    ArrayList<Tutor *> tutors;

public:
    ~TutorSystem() {
        for (auto t: tutors) {
            delete t;
        }
    }

    void add_tutor(Tutor *t) {
        tutors.push_back(t);
    }

    void remove_tutor(string name) {
        for (auto it = tutors.begin(); it != tutors.end(); ++it) {
            if ((*it)->get_name() == name) {
                delete *it;
                tutors.erase(it);
                break;
            }
        }
    }

    Tutor *find_tutor(string name) {
        for (auto t: tutors) {
            if (t->get_name() == name) {
                return t;
            }
        }
        return nullptr;
    }

    void display_all() const {
        for (auto t: tutors) {
            t->show();
            cout << "------------------------" << endl;
        }
    }
};

int main() {
    cout << "请选择您想要的选项" << '\n' << "1.幻方 2.导师管理系统";
    int m;
    cin >> m;
    switch (m) {
    case 1: {
        int n;
        cin >> n;
        if (!(n & 1)) {
            cout << "must be an odd number";
            return 0;
        }
        ArrayList<ArrayList<int>> matrix(n, ArrayList<int>(n, 0));
        int row = 0, col = n / 2;
        for (int i = 1; i <= n * n; i++) {
            matrix[row][col] = i;
            int row_next = (row - 1 + n) % n;
            int col_next = (col + 1) % n;
            if (matrix[row_next][col_next] != 0) {
                row_next = (row + 1) % n;
                col_next = col;
            }
            row = row_next;
            col = col_next;
        }

        for (auto const &v: matrix) {
            for (auto const &i: v) {
                cout << setw(3) << i << ' ';
            }
            cout << '\n';
        }
        break;
    }
    case 2: {
        TutorSystem ts;
        char choice;
        while (true) {
            cout << "\n导师管理系统" << endl;
            cout << "1. 添加导师" << endl;
            cout << "2. 添加研究生" << endl;
            cout << "3. 添加本科生(导师下)" << endl;
            cout << "4. 添加本科生(研究生下)" << endl;
            cout << "5. 删除人员" << endl;
            cout << "6. 查询信息" << endl;
            cout << "7. 统计信息" << endl;
            cout << "8. 显示所有信息" << endl;
            cout << "0. 退出" << endl;
            cout << "请选择: ";
            cin >> choice;
            if (choice == '0') {
                break;
            }

            string name, class_name, title, tutor_name;
            Tutor *tutor;
            switch (choice) {
            case '1': {
                cout << "输入导师姓名和职称: ";
                cin >> name >> title;
                ts.add_tutor(new Tutor(name, title));
                break;
            }
            case '2': {
                cout << "输入研究生姓名、班级和导师姓名: ";
                cin >> name >> class_name >> tutor_name;
                tutor = ts.find_tutor(tutor_name);
                if (tutor) {
                    tutor->add_grad(new Graduate(name, class_name));
                } else {
                    cout << "未找到该导师" << endl;
                }

                break;
            }

            case '3': {
                cout << "输入本科生姓名、班级和导师姓名: ";
                cin >> name >> class_name >> tutor_name;
                tutor = ts.find_tutor(tutor_name);
                if (tutor) {
                    tutor->add_undergrad(new Undergraduate(name, class_name));
                } else {
                    cout << "未找到该导师" << endl;
                }

                break;
            }
            case '4': {
                string grad_name;
                cout << "输入本科生姓名、班级、研究生姓名和导师姓名: ";
                cin >> name >> class_name >> grad_name >> tutor_name;

                tutor = ts.find_tutor(tutor_name);
                if (tutor) {
                    bool found = false;
                    for (auto grad: tutor->get_grads()) {
                        if (grad->get_name() == grad_name) {
                            grad->add_grad(new Undergraduate(name, class_name));
                            found = true;
                            cout << "成功添加本科生到研究生名下" << endl;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "未找到该研究生" << endl;
                    }
                } else {
                    cout << "未找到该导师" << endl;
                }
                break;
            }

            case '5': {
                cout << "输入要删除的人员姓名和导师姓名: ";
                cin >> name >> tutor_name;
                tutor = ts.find_tutor(tutor_name);
                if (tutor) {
                    tutor->remove_grad(name);
                    tutor->remove_undergrad(name);
                } else {
                    cout << "未找到!" << endl;
                }
                break;
            }

            case '6': {
                cout << "输入要查询的导师姓名: ";
                cin >> name;
                tutor = ts.find_tutor(name);
                if (tutor) {
                    tutor->show();
                } else {
                    cout << "未找到";
                }
                break;
            }

            case '7': {
                cout << "输入要统计的导师姓名: ";
                cin >> name;
                tutor = ts.find_tutor(name);
                if (tutor) {
                    tutor->statistics();
                } else {
                    cout << "未找到";
                }
                break;
            }
            case '8': {
                ts.display_all();
                break;
            }
            }
        }
    }
    }
}
