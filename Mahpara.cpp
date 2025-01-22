#include <iostream>
#include <map>
#include <string>
#include <fstream>

using namespace std;

class Student {
private:
    long student_id;
    string name;
    int attendance_count;

public:
    static long next_student_id;

    Student() : student_id(0), name(""), attendance_count(0) {}
    Student(string name);

    long getID() const { return student_id; }
    string getName() const { return name; }
    int getAttendance() const { return attendance_count; }
    void markAttendance();

    friend ostream& operator<<(ostream& os, const Student& student);
    friend istream& operator>>(istream& is, Student& student);
};

long Student::next_student_id = 0;

Student::Student(string name) {
    student_id = ++next_student_id;
    this->name = name;
    attendance_count = 0;
}

void Student::markAttendance() {
    attendance_count++;
}

ostream& operator<<(ostream& os, const Student& student) {
    os << student.student_id << " " << student.name << " " << student.attendance_count;
    return os;
}

istream& operator>>(istream& is, Student& student) {
    is >> student.student_id;
    is.ignore();
    getline(is, student.name);
    is >> student.attendance_count;
    return is;
}

class AttendanceSystem {
private:
    map<long, Student> students;

public:
    AttendanceSystem();
    void addStudent(string name);
    void markAttendance(long id);
    void showStudents();
    Student searchStudent(long id);
    void generateReport();
    void saveToFile();
    void loadFromFile();
};

AttendanceSystem::AttendanceSystem() {
    loadFromFile();
}

void AttendanceSystem::loadFromFile() {
    ifstream file("students.txt");
    if (file.is_open()) {
        students.clear();
        Student student;
        while (file >> student) {
            students[student.getID()] = student;
            if (student.getID() > Student::next_student_id) {
                Student::next_student_id = student.getID();
            }
        }
        file.close();
    }
}

void AttendanceSystem::saveToFile() {
    ofstream file("students.txt");
    if (file.is_open()) {
        for (const auto& s : students) {
            file << s.second << endl;
        }
        file.close();
    }
}

void AttendanceSystem::addStudent(string name) {
    Student student(name);
    students[student.getID()] = student;
    saveToFile();
    cout << "Student Added! ID: " << student.getID() << endl;
}

void AttendanceSystem::markAttendance(long id) {
    if (students.find(id) != students.end()) {
        students[id].markAttendance();
        saveToFile();
        cout << "Attendance marked for student ID: " << id << endl;
    } else {
        cout << "Student not found!" << endl;
    }
}

void AttendanceSystem::showStudents() {
    for (const auto& s : students) {
        cout << s.second << endl;
    }
}

Student AttendanceSystem::searchStudent(long id) {
    if (students.find(id) != students.end()) {
        return students[id];
    } else {
        throw runtime_error("Student not found!");
    }
}

void AttendanceSystem::generateReport() {
    cout << "*** Attendance Report ***" << endl;
    for (const auto& s : students) {
        cout << s.second << endl;
    }
}

int main() {
    AttendanceSystem system;
    int choice;
    string name;
    long student_id;

    do {
        cout << "\n1. Add Student\n2. Search Student\n3. Mark Attendance\n4. Show All Students\n5. Attendance Report\n6. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Student Name: ";
                cin.ignore();
                getline(cin, name);
                system.addStudent(name);
                break;
            case 2:
                cout << "Enter Student ID: ";
                cin >> student_id;
                try {
                    Student student = system.searchStudent(student_id);
                    cout << student << endl;
                } catch (exception& e) {
                    cout << e.what() << endl;
                }
                break;
            case 3:
                cout << "Enter Student ID to mark attendance: ";
                cin >> student_id;
                system.markAttendance(student_id);
                break;
            case 4:
                system.showStudents();
                break;
            case 5:
                system.generateReport();
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 6);

    return 0;
}
