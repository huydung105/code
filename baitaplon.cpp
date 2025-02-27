#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// Cấu trúc sinh viên
struct Student {
    string id;          // Mã sinh viên
    string name;        // Họ tên
    int birthYear;      // Năm sinh
    float gpa;          // Điểm trung bình

    // Constructor
    Student(string _id = "", string _n = "", int _y = 0, float _g = 0.0) 
        : id(_id), name(_n), birthYear(_y), gpa(_g) {}
};

// Node cho cây nhị phân tìm kiếm
struct Node {
    Student data;
    Node* left;
    Node* right;
    Node(Student s) : data(s), left(nullptr), right(nullptr) {}
};

// Hàm đọc file
vector<Student> readFile(string filename) {
    vector<Student> students;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Khong mo duoc file!" << endl;
        return students;
    }

    string id, name;
    int year;
    float gpa;
    while (file >> id >> name >> year >> gpa) {
        students.push_back(Student(id, name, year, gpa));
    }
    file.close();
    return students;
}

// 1. Tìm kiếm tuyến tính
Student linearSearch(vector<Student>& students, string targetId) {
    for (int i = 0; i < students.size(); i++) {
        if (students[i].id == targetId) {
            return students[i];
        }
    }
    return Student(); // Trả về sinh viên rỗng nếu không tìm thấy
}

// Hàm hỗ trợ cho Quick Sort
int partition(vector<Student>& arr, int low, int high) {
    string pivot = arr[high].id;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].id <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<Student>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 2. Tìm kiếm nhị phân (yêu cầu mảng đã sắp xếp)
Student binarySearch(vector<Student>& students, string targetId) {
    int left = 0;
    int right = students.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (students[mid].id == targetId) {
            return students[mid];
        }
        if (students[mid].id < targetId) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return Student(); // Không tìm thấy
}

// 3. Cây nhị phân tìm kiếm
class BST {
private:
    Node* root;
    
    Node* insert(Node* node, Student s) {
        if (node == nullptr) {
            return new Node(s);
        }
        if (s.id < node->data.id) {
            node->left = insert(node->left, s);
        } else if (s.id > node->data.id) {
            node->right = insert(node->right, s);
        }
        return node;
    }
    
    Student search(Node* node, string id) {
        if (node == nullptr) {
            return Student();
        }
        if (node->data.id == id) {
            return node->data;
        }
        if (id < node->data.id) {
            return search(node->left, id);
        }
        return search(node->right, id);
    }
    
public:
    BST() : root(nullptr) {}
    
    void insert(Student s) {
        root = insert(root, s);
    }
    
    Student search(string id) {
        return search(root, id);
    }
};

// Hàm hiển thị thông tin sinh viên
void displayStudent(Student s) {
    if (s.id.empty()) {
        cout << "Khong tim thay sinh vien!" << endl;
        return;
    }
    cout << "Ma SV: " << s.id << endl;
    cout << "Ho ten: " << s.name << endl;
    cout << "Nam sinh: " << s.birthYear << endl;
    cout << "Diem TB: " << s.gpa << endl;
}

int main() {
    // Đọc dữ liệu từ file
    vector<Student> students = readFile("students.txt");
    if (students.empty()) return 1;

    // Tạo cây nhị phân tìm kiếm
    BST bst;
    for (auto& s : students) {
        bst.insert(s);
    }

    // Sắp xếp mảng cho tìm kiếm nhị phân
    quickSort(students, 0, students.size() - 1);

    string searchId;
    cout << "Nhap ma sinh vien can tim: ";
    cin >> searchId;

    cout << "\n1. Tim kiem tuyen tinh:\n";
    Student result1 = linearSearch(students, searchId);
    displayStudent(result1);

    cout << "\n2. Tim kiem nhi phan:\n";
    Student result2 = binarySearch(students, searchId);
    displayStudent(result2);

    cout << "\n3. Tim kiem tren cay nhi phan:\n";
    Student result3 = bst.search(searchId);
    displayStudent(result3);

    return 0;
}