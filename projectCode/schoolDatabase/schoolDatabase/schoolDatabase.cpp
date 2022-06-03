// schoolDatabase.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
// Using namespaces
using std::cin;
using std::cout;
using std::endl;

// Using namespace declarations
using std::string;
using std::vector;

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
    recordExamples();
    // Startup Menu
    registerParent();
    return 0; // Placeholder
}