#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

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

    void setID(long id) { student_id = id; }
    void setAttendance(int count) { attendance_count = count; }
};

long Student::next_student_id = 1; // Start IDs from 1

Student::Student(string name) {
    student_id = next_student_id++;
    this->name = name;
    attendance_count = 0;
}

void Student::markAttendance() {
    attendance_count++;
}

ostream& operator<<(ostream& os, const Student& student) {
    os << student.student_id << "," << student.name << "," << student.attendance_count;
    return os;
}

istream& operator>>(istream& is, Student& student) {
    string id_str, attendance_str;
    if (!getline(is, id_str, ',') || !getline(is, student.name, ',') || !getline(is, attendance_str)) {
        return is;
    }
    try {
        student.student_id = stol(id_str);
        student.attendance_count = stoi(attendance_str);
    } catch (exception& e) {
        cerr << "Error parsing student data: " << e.what() << endl;
    }
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

void AttendanceSystem::saveToFile() {
    ofstream file("data.txt", ios::trunc);
    if (!file.is_open()) {
        cerr << "Error: Unable to open data.txt for writing!" << endl;
        return;
    }

    for (const auto& s : students) {
        file << s.second.getID() << "," << s.second.getName() << "," << s.second.getAttendance() << endl;
    }

    file.flush();
    file.close();
    cout << "✅ Data successfully saved to data.txt" << endl;
}

void AttendanceSystem::loadFromFile() {
    ifstream file("data.txt");

    if (!file.is_open()) {
        cout << "Warning: No existing data file found. A new one will be created." << endl;
        return;
    }

    students.clear();
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        long id;
        string name;
        int attendance;

        string id_str, attendance_str;
        if (!getline(ss, id_str, ',') || !getline(ss, name, ',') || !getline(ss, attendance_str)) {
            cerr << "Warning: Skipping corrupt data: " << line << endl;
            continue;
        }

        try {
            id = stol(id_str);
            attendance = stoi(attendance_str);
        } catch (exception& e) {
            cerr << "Error parsing data: " << line << endl;
            continue;
        }

        students[id] = Student(name);
        students[id].setID(id);
        students[id].setAttendance(attendance);

        if (id >= Student::next_student_id) {
            Student::next_student_id = id + 1;
        }
    }

    file.close();
    cout << "✅ Data successfully loaded from data.txt" << endl;
}

void AttendanceSystem::addStudent(string name) {
    Student student(name);
    students[student.getID()] = student;
    saveToFile();
    cout << "✅ Student Added! ID: " << student.getID() << endl;
}

void AttendanceSystem::markAttendance(long id) {
    if (students.find(id) != students.end()) {
        students[id].markAttendance();
        saveToFile();
        cout << "✅ Attendance marked for Student ID: " << id << endl;
    } else {
        cout << "❌ Student not found!" << endl;
    }
}

void AttendanceSystem::showStudents() {
    cout << "\n=== All Students ===\n";
    for (const auto& s : students) {
        cout << "ID: " << s.second.getID() << ", Name: " << s.second.getName()
             << ", Attendance: " << s.second.getAttendance() << endl;
    }
}

Student AttendanceSystem::searchStudent(long id) {
    if (students.find(id) != students.end()) {
        return students[id];
    } else {
        throw runtime_error("❌ Student not found!");
    }
}

void AttendanceSystem::generateReport() {
    cout << "\n=== Attendance Report ===\n";
    for (const auto& s : students) {
        cout << "ID: " << s.second.getID() << ", Name: " << s.second.getName()
             << ", Attendance: " << s.second.getAttendance() << endl;
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
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter Student Name: ";
                getline(cin, name);
                system.addStudent(name);
                break;
            case 2:
                cout << "Enter Student ID: ";
                cin >> student_id;
                try {
                    Student student = system.searchStudent(student_id);
                    cout << "✅ Found: ID " << student.getID() << ", Name: " << student.getName()
                         << ", Attendance: " << student.getAttendance() << endl;
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
                cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 6);

    return 0;
}
