#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

class Student {
private:
    string name;
    int rollNumber;
    double marks;

public:
    Student() : rollNumber(0), marks(0) {}
    Student(string name, int rollNumber, double marks) : name(name), rollNumber(rollNumber), marks(marks) {}

    // Friend functions
    friend void displayStudent(const Student& student);
    friend void updateStudentData(Student& student);
    friend void exportStudentToCSV(const Student& student, ofstream& file);
    friend void loadStudentFromFile(Student& student, const string& line);

    // Getters
    int getRollNumber() const { return rollNumber; }
    string getName() const { return name; }
    double getMarks() const { return marks; }

    // Setters
    void setName(const string& newName) { name = newName; }
    void setMarks(double newMarks) { marks = newMarks; }
};

// Friend function to display student details
void displayStudent(const Student& student) {
    string grade;
    if (student.marks >= 90) grade = "A+";
    else if (student.marks >= 80) grade = "A";
    else if (student.marks >= 70) grade = "B";
    else if (student.marks >= 60) grade = "C";
    else if (student.marks >= 50) grade = "D";
    else grade = "F";

    cout << fixed << setprecision(2);
    cout << "Roll Number: " << student.rollNumber
         << ", Name: " << student.name
         << ", Marks: " << student.marks
         << ", Grade: " << grade << endl;
}

// Friend function to update student details
void updateStudentData(Student& student) {
    cout << "Enter new Name: ";
    cin.ignore();
    getline(cin, student.name);

    while (true) {
        cout << "Enter new Marks: ";
        if (cin >> student.marks) {
            break; // Valid input
        } else {
            cin.clear(); // Clear the fail state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input! Please enter a number for marks." << endl;
        }
    }
}

// Friend function to export student data to CSV
void exportStudentToCSV(const Student& student, ofstream& file) {
    string grade;
    if (student.marks >= 90) grade = "A+";
    else if (student.marks >= 80) grade = "A";
    else if (student.marks >= 70) grade = "B";
    else if (student.marks >= 60) grade = "C";
    else if (student.marks >= 50) grade = "D";
    else grade = "F";

    file << student.rollNumber << ","
         << student.name << ","
         << student.marks << ","
         << grade << "\n";
}

// Friend function to load student data from file
void loadStudentFromFile(Student& student, const string& line) {
    stringstream ss(line);
    string rollStr, name, marksStr;

    getline(ss, rollStr, ',');
    getline(ss, name, ',');
    getline(ss, marksStr, ',');

    try {
        student.rollNumber = stoi(rollStr);
        student.name = name;
        student.marks = stod(marksStr);
    } catch (const invalid_argument& e) {
        cerr << "Error: Invalid data in file. Skipping line: " << line << endl;
    }
}

class StudentManager {
private:
    vector<Student> students;
    string filename = "students.csv";

    void saveToFile() {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& student : students) {
                file << student.getRollNumber() << ","
                     << student.getName() << ","
                     << student.getMarks() << "\n";
            }
            file.close();
        } else {
            cerr << "Error saving to file!" << endl;
        }
    }

    void loadFromFile() {
        ifstream file(filename);
        if (file.is_open()) {
            students.clear();
            string line;
            while (getline(file, line)) {
                if (line.empty()) continue;
                Student student;
                loadStudentFromFile(student, line);
                students.push_back(student);
            }
            file.close();
        }
    }

public:
    StudentManager() {
        loadFromFile();
    }

    void addStudent() {
        string name;
        int rollNumber;
        double marks;

        // Input validation for roll number
        while (true) {
            cout << "Enter Roll Number: ";
            if (cin >> rollNumber) {
                break; // Valid input
            } else {
                cin.clear(); // Clear the fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid input! Please enter an integer for roll number." << endl;
            }
        }

        cin.ignore(); // Clear the buffer
        cout << "Enter Name: ";
        getline(cin, name);

        // Input validation for marks
        while (true) {
            cout << "Enter Marks: ";
            if (cin >> marks) {
                break; // Valid input
            } else {
                cin.clear(); // Clear the fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid input! Please enter a number for marks." << endl;
            }
        }

        students.emplace_back(name, rollNumber, marks);
        saveToFile();
        cout << "Student added successfully!" << endl;
    }

    void displayAllStudents() const {
        if (students.empty()) {
            cout << "No students found!" << endl;
            return;
        }
        for (const auto& student : students) {
            displayStudent(student);
        }
    }

    void searchStudent() const {
        int rollNumber;

        // Input validation for roll number
        while (true) {
            cout << "Enter Roll Number to search: ";
            if (cin >> rollNumber) {
                break; // Valid input
            } else {
                cin.clear(); // Clear the fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid input! Please enter an integer for roll number." << endl;
            }
        }

        for (const auto& student : students) {
            if (student.getRollNumber() == rollNumber) {
                displayStudent(student);
                return;
            }
        }
        cout << "Student not found!" << endl;
    }

    void updateStudent() {
        int rollNumber;

        // Input validation for roll number
        while (true) {
            cout << "Enter Roll Number to update: ";
            if (cin >> rollNumber) {
                break; // Valid input
            } else {
                cin.clear(); // Clear the fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid input! Please enter an integer for roll number." << endl;
            }
        }

        for (auto& student : students) {
            if (student.getRollNumber() == rollNumber) {
                updateStudentData(student);
                saveToFile();
                cout << "Student updated successfully!" << endl;
                return;
            }
        }
        cout << "Student not found!" << endl;
    }

    void deleteStudent() {
        int rollNumber;

        // Input validation for roll number
        while (true) {
            cout << "Enter Roll Number to delete: ";
            if (cin >> rollNumber) {
                break; // Valid input
            } else {
                cin.clear(); // Clear the fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid input! Please enter an integer for roll number." << endl;
            }
        }

        auto it = remove_if(students.begin(), students.end(), [rollNumber](const Student& s) {
            return s.getRollNumber() == rollNumber;
        });

        if (it != students.end()) {
            students.erase(it, students.end());
            saveToFile();
            cout << "Student deleted successfully!" << endl;
        } else {
            cout << "Student not found!" << endl;
        }
    }

    void exportToCSV() const {
        ofstream file("students_export.csv");
        if (file.is_open()) {
            file << "Roll Number,Name,Marks,Grade\n";
            for (const auto& student : students) {
                exportStudentToCSV(student, file);
            }
            file.close();
            cout << "Data exported to students_export.csv successfully!" << endl;
        } else {
            cerr << "Error exporting data!" << endl;
        }
    }
};

int main() {
    StudentManager manager;
    int choice;

    do {
        cout << "\nStudent Management System\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Export to CSV\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        // Input validation for menu choice
        while (true) {
            if (cin >> choice) {
                break; // Valid input
            } else {
                cin.clear(); // Clear the fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid input! Please enter a number between 1 and 7." << endl;
                cout << "Enter your choice: ";
            }
        }

        switch (choice) {
            case 1:
                manager.addStudent();
                break;
            case 2:
                manager.displayAllStudents();
                break;
            case 3:
                manager.searchStudent();
                break;
            case 4:
                manager.updateStudent();
                break;
            case 5:
                manager.deleteStudent();
                break;
            case 6:
                manager.exportToCSV();
                break;
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
