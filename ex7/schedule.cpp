#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <iostream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <thread>
using namespace std;

const int MAXN = 1010;  // 最大学生数
const int MAXM = 15;    // 最大课程数
const int MAXC = 15;    // 最大班级数

struct Student {
    string id, name;
    int class_id;
    vector<double> scores;
    double total;
    
    Student() : scores(MAXM) {}
    
    void calc_total() {
        total = accumulate(scores.begin(), scores.end(), 0.0);
    }
};

struct Course {
    int id, time_slot;
};

vector<Student> stu;
vector<Course> courses;
vector<vector<bool>> course_map;  // 班级课程映射
int n_stu = 0;
const int n_course = 10;
const int n_class = 10;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// 清屏函数
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 等待用户按键继续
void waitForKey() {
    cout << "\n按Enter键继续...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// 显示进度条
void showProgressBar(const string& operation, int progress, int total) {
    const int barWidth = 50;
    float percentage = static_cast<float>(progress) / total;
    int pos = barWidth * percentage;

    cout << operation;
    cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(percentage * 100.0) << " %\r";
    cout.flush();
}

// 生成测试数据
void gen_test_data() {
    course_map.resize(n_class, vector<bool>(n_course));
    
    for(int c = 0; c < n_class; c++) {
        // 每个班随机分配3门课
        vector<int> course_ids(n_course);
        iota(course_ids.begin(), course_ids.end(), 0);
        shuffle(course_ids.begin(), course_ids.end(), rng);
        
        for(int i = 0; i < 3; i++) {
            course_map[c][course_ids[i]] = true;
        }
        
        // 生成该班学生
        for(int i = 0; i < 30; i++) {
            Student s;
            s.id = "B" + to_string(c + 1) + (i < 9 ? "0" : "") + to_string(i + 1);
            s.name = "Student_" + s.id;
            s.class_id = c;
            
            uniform_real_distribution<double> score_dist(0, 100);
            for(int j = 0; j < n_course; j++) {
                s.scores[j] = course_map[c][j] ? score_dist(rng) : 0;
            }
            s.calc_total();
            stu.push_back(s);
            n_stu++;
        }
    }
}

// 安排考试时间
void schedule_exams() {
    vector<vector<bool>> conflict(n_course, vector<bool>(n_course));
    courses.resize(n_course);
    
    // 构建冲突矩阵
    for(int c = 0; c < n_class; c++) {
        for(int i = 0; i < n_course; i++) {
            if(!course_map[c][i]) continue;
            for(int j = i + 1; j < n_course; j++) {
                if(!course_map[c][j]) continue;
                conflict[i][j] = conflict[j][i] = true;
            }
        }
    }
    
    // 分配时间段
    for(int i = 0; i < n_course; i++) {
        courses[i].id = i;
        for(int t = 0; t < n_course; t++) {
            bool can = true;
            for(int j = 0; j < i; j++) {
                if(conflict[i][j] && courses[j].time_slot == t) {
                    can = false;
                    break;
                }
            }
            if(can) {
                courses[i].time_slot = t;
                break;
            }
        }
    }
}
// 在之前的代码中添加这个新函数
void show_exam_schedule() {
    clearScreen();
    cout << "\n=== 考试时间安排 ===\n\n";
    
    // 创建时间表矩阵
    vector<vector<string>> schedule(n_course, vector<string>(n_course, "      "));
    
    // 填充时间表
    for(const auto& c : courses) {
        schedule[c.time_slot][c.id] = "课程" + to_string(c.id + 1);
    }
    
    // 打印时间表头部
    cout << "     ";
    for(int i = 0; i < n_course; i++) {
        cout << setw(8) << ("Day " + to_string(i + 1));
    }
    cout << "\n";
    
    // 打印分隔线
    cout << "     " << string(n_course * 8, '-') << "\n";
    
    // 打印时间表主体
    vector<string> time_slots = {
        "8:00 ", "9:00 ", "10:00", "11:00",
        "14:00", "15:00", "16:00", "17:00",
        "19:00", "20:00"
    };
    
    for(int i = 0; i < n_course; i++) {
        cout << time_slots[i] << "|";
        for(int j = 0; j < n_course; j++) {
            if(schedule[i][j] != "      ") {
                cout << setw(8) << schedule[i][j];
            } else {
                cout << setw(8) << "  ";
            }
        }
        cout << "\n";
    }
    
    // 打印班级课程信息
    cout << "\n各班级考试课程：\n";
    cout << string(50, '-') << "\n";
    for(int c = 0; c < n_class; c++) {
        cout << "班级 " << c + 1 << ": ";
        bool first = true;
        for(int j = 0; j < n_course; j++) {
            if(course_map[c][j]) {
                if(!first) cout << ", ";
                cout << "课程" << j + 1;
                first = false;
            }
        }
        cout << "\n";
    }
    
    // 打印图例
    cout << "\n图例说明：\n";
    cout << "- 横轴表示考试日期（Day 1-10）\n";
    cout << "- 纵轴表示考试时间段\n";
    cout << "- 课程X表示第X号课程\n";
    cout << "- 每个时间段只安排没有冲突的考试\n";
    
    // 检查并显示可能的时间冲突
    cout << "\n时间安排检查：\n";
    cout << string(50, '-') << "\n";
    bool has_conflict = false;
    for(int c = 0; c < n_class; c++) {
        vector<int> same_time_courses;
        for(int i = 0; i < n_course; i++) {
            if(!course_map[c][i]) continue;
            for(int j = i + 1; j < n_course; j++) {
                if(!course_map[c][j]) continue;
                if(courses[i].time_slot == courses[j].time_slot) {
                    has_conflict = true;
                    cout << "警告：班级 " << c + 1 << " 的课程 " 
                         << i + 1 << " 和课程 " << j + 1 
                         << " 在同一时间段！\n";
                }
            }
        }
    }
    
    if(!has_conflict) {
        cout << "√ 时间安排检查通过，无冲突\n";
    }
    
    waitForKey();
}
// 显示主菜单
void displayMenu() {
    clearScreen();
    cout << "\n=== 学生成绩管理系统 ===\n\n";
    cout << "1. 查看所有学生成绩排名\n";
    cout << "2. 按班级查看成绩\n";
    cout << "3. 查看成绩分布分析\n";
    cout << "4. 查找学生信息\n";
    cout << "5. 查看考试时间安排\n";
    cout << "0. 退出系统\n\n";
    cout << "请输入选项 (0-5): ";
}

// 打印所有学生
void print_all() {
    clearScreen();
    cout << "\n=== 学生总成绩排名 ===\n\n";
    
    vector<Student> tmp = stu;
    sort(tmp.begin(), tmp.end(), 
        [](const Student& a, const Student& b) {
            return abs(a.total - b.total) > 1e-6 ? 
                   a.total > b.total : a.id < b.id;
        });
    
    cout << setw(8) << "排名" << setw(10) << "学号" 
         << setw(15) << "姓名" << setw(10) << "班级" 
         << setw(12) << "总分" << "\n";
    cout << string(55, '-') << "\n";
    
    for(int i = 0; i < tmp.size(); i++) {
        const auto& s = tmp[i];
        cout << setw(8) << i + 1
             << setw(10) << s.id 
             << setw(15) << s.name
             << setw(10) << s.class_id + 1
             << setw(12) << fixed << setprecision(2) << s.total << "\n";
    }
    
    waitForKey();
}

// 按班级打印
void print_by_class() {
    clearScreen();
    cout << "\n=== 班级成绩查看 ===\n";
    cout << "请输入要查看的班级 (1-" << n_class << "): ";
    int class_id;
    cin >> class_id;
    class_id--;
    
    if(class_id < 0 || class_id >= n_class) {
        cout << "\n错误：无效的班级编号！\n";
        waitForKey();
        return;
    }
    
    cout << "\n=== " << class_id + 1 << "班成绩单 ===\n\n";
    vector<Student> class_stu;
    copy_if(stu.begin(), stu.end(), back_inserter(class_stu),
            [class_id](const Student& s) { return s.class_id == class_id; });
    
    sort(class_stu.begin(), class_stu.end(),
         [](const Student& a, const Student& b) {
             return a.total > b.total;
         });
    
    cout << setw(8) << "班级排名" << setw(10) << "学号" 
         << setw(15) << "姓名" << setw(12) << "总分" << "\n";
    cout << string(45, '-') << "\n";
    
    for(int i = 0; i < class_stu.size(); i++) {
        const auto& s = class_stu[i];
        cout << setw(8) << i + 1
             << setw(10) << s.id 
             << setw(15) << s.name
             << setw(12) << fixed << setprecision(2) << s.total << "\n";
    }
    
    waitForKey();
}

// 分析分数分布
void analyze_scores() {
    clearScreen();
    cout << "\n=== 成绩分布分析 ===\n\n";
    
    vector<pair<int, int>> ranges = {{90,100}, {80,89}, {70,79}, {60,69}, {0,59}};
    vector<string> grade_names = {"优秀", "良好", "中等", "及格", "不及格"};
    
    cout << setw(15) << "分数段" << setw(15) << "等级" 
         << setw(12) << "人数" << setw(15) << "百分比" 
         << setw(20) << "分数范围" << "\n";
    cout << string(77, '-') << "\n";
    
    for(size_t i = 0; i < ranges.size(); i++) {
        const auto& range = ranges[i];
        vector<Student> range_stu;
        copy_if(stu.begin(), stu.end(), back_inserter(range_stu),
                [&](const Student& s) {
                    return s.total >= range.first && s.total <= range.second;
                });
        
        if(!range_stu.empty()) {
            auto [min_it, max_it] = minmax_element(range_stu.begin(), range_stu.end(),
                [](const Student& a, const Student& b) {
                    return a.total < b.total;
                });
            
            double percentage = 100.0 * range_stu.size() / n_stu;
            
            cout << setw(12) << (to_string(range.first) + "-" + to_string(range.second))
                 << setw(15) << grade_names[i]
                 << setw(12) << range_stu.size()
                 << setw(14) << fixed << setprecision(2) << percentage << "%"
                 << setw(20) << (to_string(min_it->total) + " - " + to_string(max_it->total))
                 << "\n";
        }
    }
    
    waitForKey();
}

// 查找学生
void find_student() {
    clearScreen();
    cout << "\n=== 学生信息查询 ===\n\n";
    cout << "请输入学生学号: ";
    string id;
    cin >> id;
    
    auto it = find_if(stu.begin(), stu.end(),
                     [&](const Student& s) { return s.id == id; });
    
    if(it != stu.end()) {
        cout << "\n=== 学生详细信息 ===\n\n";
        cout << "学号: " << it->id << "\n";
        cout << "姓名: " << it->name << "\n";
        cout << "班级: " << it->class_id + 1 << "\n";
        cout << "总分: " << fixed << setprecision(2) << it->total << "\n\n";
        
        cout << "各科成绩:\n";
        cout << string(30, '-') << "\n";
        for(int j = 0; j < n_course; j++) {
            if(course_map[it->class_id][j]) {
                cout << "课程 " << j + 1 << ": " 
                     << fixed << setprecision(2) << it->scores[j] << "\n";
            }
        }
    } else {
        cout << "\n错误：未找到该学生！\n";
    }
    
    waitForKey();
}

// 显示考试时间安排

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // 生成测试数据并显示进度条
    cout << "正在初始化系统...\n";
    for(int i = 0; i <= 100; i++) {
        showProgressBar("生成测试数据: ", i, 100);
        if(i == 50) gen_test_data();
        if(i == 80) schedule_exams();
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    cout << "\n初始化完成！\n";
    waitForKey();
    
    // 主循环
    while(true) {
        displayMenu();
        int choice;
        cin >> choice;
        
        switch(choice) {
            case 0:
                cout << "\n感谢使用！再见！\n";
                return 0;
            case 1:
                print_all();
                break;
            case 2:
                print_by_class();
                break;
            case 3:
                analyze_scores();
                break;
            case 4:
                find_student();
                break;
            case 5:
                show_exam_schedule();
                break;
            default:
                cout << "\n错误：无效的选项！\n";
                waitForKey();
        }
    }
    
    return 0;
}
