// schoolDatabase.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
// Using namespaces
using std::cin;
using std::cout;
using std::endl;

using std::string;
using std::vector;
using std::ofstream;
using std::ifstream;

struct student {
    string name;
    int gender;
    vector<vector<int>> subjectGrades; // [SubjectID, gradeNum (0:NA, 1:A, 2:M, 3:E)
    student(string fullName, int identifiedGender, vector<vector<int>> subjects) {
        name = fullName;
        gender = identifiedGender;
        subjectGrades = subjects;
    }
};

// These structs will be stored individually when saved to the hard-drive
struct classroom {
    int classNumber;
    vector<student> students;
    classroom(int classNum, vector<student> studentInfo) {
        classNumber = classNum;
        students = studentInfo;
    }
};

// Caregiver data storage, used when registering a new parent
struct caregiver {
    string name;
    int phoneNum;
    // Caregiver constructor
    caregiver(string fullName, int number) {
        name = fullName;
        phoneNum = number;
    }
};

// Child data storage, used when registering a new parent
struct child {
    string name;
    int classNum;
    vector<caregiver> caregivers;
    // Child Constructor
    child(string fullName, int classroom, vector<caregiver> guardians) {
        name = fullName;
        classNum = classroom;
        caregivers = guardians;
    }
};

// Overarching student records storage
vector<classroom> classroomRecords = {
    // Placeholder values
    /*
    classroom(1, {
        student("John Doe", 1, {{1, 1}, {2, 2}, {3, 3}}),
        student("Jane Doe", 1, {{1, 2}, {2, 3}, {3, 1}}),
        student("Jack Doe", 0, {{1, 3}, {2, 1}, {3, 2}})
        }
    ),
    classroom(2, {
        student("Jill Doe", 1, {{1, 1}, {2, 2}, {3, 3}}),
        student("Jared Doe", 0, {{1, 2}, {2, 3}, {3, 1}}),
        student("Jenny Doe", 1, {{1, 3}, {2, 1}, {3, 2}})
        }
    ),
    classroom(3, {
        student("Jasmine Doe", 1, {{1, 1}, {2, 2}, {3, 3}}),
        student("Jasper Doe", 0, {{1, 2}, {2, 3}, {3, 1}}),
        student("Josie Doe", 1, {{1, 3}, {2, 1}, {3, 2}})
        }
    )
    */
};

// Saves all classroom data stored in classroomRecords
void saveClassrooms() {
    // Opens masterClassList file, containing data of all class file locations
    ofstream masterClassList;
    masterClassList.open("masterClassList.csv", ofstream::out | ofstream::trunc); // Clears current masterFile

    ofstream classManager; // Opener for each class file
    for (int room = 0; room < classroomRecords.size(); room++) { // Loops through each classroom
        // Opens a new file for each class, as requested in the assess. requirements
        classManager.open("class" + std::to_string(classroomRecords[room].classNumber) + ".csv");
        for (int studentNum = 0; studentNum < classroomRecords[room].students.size(); studentNum++) { // Loops through each student in given class
            // Writes student data to class file (One line per student)
            classManager << classroomRecords[room].students[studentNum].name
                << "," << classroomRecords[room].students[studentNum].gender;
            for (int sub = 0; sub < classroomRecords[room].students[studentNum].subjectGrades.size(); sub++) {
                classManager << "," << classroomRecords[room].students[studentNum].subjectGrades[sub][0]
                    << "," << classroomRecords[room].students[studentNum].subjectGrades[sub][1];
            }
            classManager << endl;
        }
        classManager.close();
        // Saves class file location to masterClassList
        masterClassList << "class" << classroomRecords[room].classNumber << ".csv" << endl; // NOTE: FileLoc stored instead of classNum for futureproofing, to allow for differing directiories
    }
    masterClassList.close();
}

// Loads all classroom data from filesystem
void loadClassrooms() {
    // Open file containing all class file locations
    ifstream masterClassList;
    masterClassList.open("masterClassList.csv");
    // Opener for each class file
    ifstream classManager;
    // File line variables
    string classFile, studentInfo;
    while (std::getline(masterClassList, classFile)) { // Loops through each class
        classManager.open(classFile);
        // Seperates class number from class file
        std::stringstream numStream; // Stream needed since class number may be > 1 digits
        std::regex_replace( std::ostream_iterator<char>(numStream), classFile.begin(),
            classFile.end(), (std::regex) "[^0-9]", "");
        int classNum = std::stoi(numStream.str()); // Converts stringStream to integer
        // Seperates each students data
        vector<student> classStudents;
        while (std::getline(classManager, studentInfo)) { // Loops through each student in class
            // Student info variable declarations
            string studentName;
            vector<vector<int>> studentGrades;
            int studentGender = 0, dataPos = 0; // dataPos used to determine which piece of info below loop is writing to.
            for (char& c : studentInfo)
            {
                if (c == ',') { // End of csv column
                    dataPos++;
                    continue;
                }
                switch (dataPos) { // Used for future proofing, since extra student data may be stored in future
                case 0: // Name
                    studentName += c;
                    break;
                case 1: // Gender
                    studentGender = c - '0';
                    break;
                default: // Grades
                    if (dataPos % 2 == 0) studentGrades.push_back({ c - '0' });
                    else studentGrades[studentGrades.size() - 1].push_back(c - '0');
                    break;
                }
            }
            classStudents.push_back(student(studentName, studentGender, studentGrades));
        }
        classroomRecords.push_back(classroom(classNum, classStudents));
        classManager.close();
    }
    masterClassList.close();
}

// Adds a new parent
void registerParent() {
    classroomRecords[0].students[0].subjectGrades[0][0];
    // Parent variable declarations
    string name, email, username, password;
    char addChild = 'y'; // When this is 'n', stops adding children to children vector
    int gender, birthDay, birthMonth, birthYear, contactNum;
    vector<child> children;

    // General parent data assignment
    cout << "Parent details:\n\nEnter name below:\n";
    cin.ignore(); // Needed as parent name can have spaces
    getline(cin, name);
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter gender (1:Male, 2:Female, 3:Other): ";
    cin >> gender;
    cout << "Enter day of birth (1-31): ";
    cin >> birthDay;
    cout << "Enter month of birth (1-12): ";
    cin >> birthMonth;
    cout << "Enter year of birth (1900-2022): ";
    cin >> birthYear;
    cout << "Enter phone number: ";
    cin >> contactNum;
    // Assigns given children to parent
    while (addChild != 'n') {
        // Child variable declarations
        string childName;
        char addCaregiver = 'y'; // When addCaregiver is 'n'. stops adding caregivers to caregiver vector
        int childClass;
        vector<caregiver> caregivers;
        // General child info
        cout << "Enter name of child below:\n";
        cin.ignore(); // Needed as child name can have spaces
        getline(cin, childName);
        cout << "Enter child classroom number: ";
        cin >> childClass;

        // Gets all caregivers assoc. with child
        while (addCaregiver != 'n') {
            // Caregiver variable declarations
            string caregiverName;
            int phoneNumber;
            // General caregiver info
            cout << "Enter caregiver name below:\n";
            cin.ignore(); // Needed as caregiver name can have spaces
            getline(cin, caregiverName);
            cout << "Enter caregiver emergency phone number: ";
            cin >> phoneNumber;
            // Adds caregiver to child
            caregivers.push_back(caregiver(caregiverName, phoneNumber));
            // Checks if user wants to add another caregiver
            cout << "Add another caregiver (y\\n)? ";
            cin >> addCaregiver;
        }
        // Adds child to parent
        children.push_back(child(childName, childClass, caregivers));
        // Checks if user wants to add another child to parent
        cout << "Add another child (y\\n)? ";
        cin >> addChild;
    }
    // Saving parent to hard-drive would be done here
}

// Adds a new teacher
void registerTeacher() {
    return; // Placeholder
}

// Main Function
int main()
{
    loadClassrooms();
    saveClassrooms();
}