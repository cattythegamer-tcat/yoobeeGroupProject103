// schoolDatabase.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <algorithm>

// Hashing Library for password security
#include "SHA256.h"

// Using namespaces
using std::cin;
using std::cout;
using std::endl;

using std::string;
using std::vector;
using std::ofstream;
using std::ifstream;

//Teacher Variables - NOTE: THIS NEEDS TO BE REMOVED BY SPRINT 2 END
int teaAcc = 0;
string teachName[10];
char teachGen[10];
int teachDoB[10];
string teachEmail[10];
int teachPho[10];
int teachClassNum[10];
int teachYLvl[10];
string teachUn[10];
string Passw;
string Passw2;
string teachPassw[10];

// Student data storage, used to store the grades for a student
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
    )*/
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
        std::regex_replace(std::ostream_iterator<char>(numStream), classFile.begin(),
            classFile.end(), (std::regex)"[^0-9]", "");
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
void parentRegister() {
    classroomRecords[0].students[0].subjectGrades[0][0];
    // Parent variable declarations
    string name, email, username, password = "", rePassword = "";
    char addChild = 'y'; // When this is 'n', stops adding children to children vector
    int gender, birthDay, birthMonth, birthYear, contactNum;
    vector<child> children;

    // General account info
    cout << "Parent details:\n\nEnter name below:\n";
    cin.ignore(); // Needed as parent name can have spaces
    getline(cin, name);
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter username: ";
    cin >> username;

    // Password
    do {
        cout << "Enter password: ";
        cin >> password;
        cout << "Re-enter password: ";
        cin >> rePassword;
        if (password != rePassword) cout << "Passwords don't match!\n";
    } while (password != rePassword);

    // Personal details
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

    string hashedPassword = username + password;
    SHA256 sha;
    sha.update(hashedPassword);
    uint8_t* digest = sha.digest();
    password = SHA256::toString(digest);

    // Load parent account file
    ofstream parentAccounts;
    parentAccounts.open("parentAccounts.csv", std::ios::app); // Appends new parent to end of file
    // Write new parent info to file
    parentAccounts << username << ","
        << password << ","
        << name << ","
        << email << ","
        << gender << ","
        << birthDay << ","
        << birthMonth << ","
        << birthYear << ","
        << contactNum << ",";
    // Adds children info to parent file
    for (int childNum = 0; childNum < children.size(); childNum++) {
        parentAccounts << "["
            << children[childNum].name << ","
            << children[childNum].classNum;
        for (int guardianNum = 0; guardianNum < children[childNum].caregivers.size(); guardianNum++) {
            parentAccounts << "," << children[childNum].caregivers[guardianNum].name << ","
                << children[childNum].caregivers[guardianNum].phoneNum;
        }
        parentAccounts << "],";
    }
    // Close parent file
    parentAccounts << endl;
    parentAccounts.close();
}

// Adds a new teacher
void teacherRegister() {
    system("cls");

    cout << "\n\nWhat is your First name: ";
    cin.ignore();
    getline(cin, teachName[teaAcc]);

RedoGender: // goto loops will make us lose points, since they're bad practice to use, using a while loop or try/catch would be better
    cout << "\n\nWhat is your gender (m = Mr./f = Mrs.): ";
    cin >> teachGen[teaAcc];
    if (teachGen[teaAcc] != 'm' || teachGen[teaAcc] != 'f') {
        goto RedoGender;
        cout << "That is not a valid option: ";
    }

    cout << "\n\nWhat is you phone number: ";
    cin >> teachPho[teaAcc];

    cout << "\n\nWhat is your classroom number: ";
    cin >> teachClassNum[teaAcc];

    cout << "\n\nWhat year level do you teach: ";
    cin >> teachYLvl[teaAcc];

    cout << "\n\nWhat will your username be: ";
    cin >> teachUn[teaAcc];

RedoPassword:
    cout << "\n\nPlease enter your password: ";
    cin >> Passw;
    cout << "\n\nEnter it again: ";
    cin >> Passw2;
    if (Passw != Passw2) {
        cout << "\n\nThe passwords do no match. Try again";
        goto RedoPassword;
    }
    else
        teachPassw[teaAcc] = Passw;

    teaAcc++;
}

void adminMenu() {

    int controls = 0;
    bool looping = true;

    while (looping = true) {
        switch (controls) {
        case 1:

        default:
            break;
        }
    }
}

// Parent signin
void parentLogin()
{
    // Account String Declarations
    string username, password, fetchedInfo;
    // Stores all username-password hashes in parentAccount DB
    vector<string> validAccounts;
    // Fetches parentAccounts file for populating validAccounts Vector
    ifstream parentAccounts;
    parentAccounts.open("parentAccounts.csv");
    // Populates validAccounts Vector with all account hashes
    while (std::getline(parentAccounts, fetchedInfo)) {
        string fetchedPassword;
        int dataPos = 0; // dataPos used to determine which piece of info below loop is writing to.
        for (char& c : fetchedInfo)
        {
            if (c == ',') { // End of csv column
                dataPos++;
                if (dataPos == 2) break;
                continue;
            }
            if (dataPos == 1) fetchedPassword += c;
        }
        validAccounts.push_back(fetchedPassword);
    }
    parentAccounts.close();
    // User signin input system
    char attemptingSignin = 'y';
    while (attemptingSignin != 'n') {
        cout << "Please enter your username: ";
        cin >> username;
        cout << "Please enter your password: ";
        cin >> password;
        // Forms username-password hash combo
        string hashedAccount = username + password;
        SHA256 sha;
        sha.update(hashedAccount);
        uint8_t* digest = sha.digest();
        hashedAccount = SHA256::toString(digest);
        // Checks if hash combo is a valid account
        if (std::find(validAccounts.begin(), validAccounts.end(), hashedAccount) != validAccounts.end()) {
            cout << "\nSigned in successfully!\n";
            return;
        }
        cout << "\nUsername or password is incorrect.\nTry again (y/n)? "; // Allows for exit if user can't remember password
        cin >> attemptingSignin;
    }
    cout << "\nUnsuccessful!";
}

void teacherLogin()
{
    cout << "Teacher Login Page\n";
}

void adminLogin()
{
    cout << "Administrator Login Page\n";
}

void contactInfo()
{
    cout << "Mebee College Contact Information\n";
}

void functionsEvents()
{
    int functionsEventsInput, recentInput, upcomingInput;

    cout << "Mebee College Functions and Events Page\n";
    cout << " Date : June 29th 2022\n";
    cout << "Please choose an option below\n";
    cout << "1. Recent Functions & Events\n2. Upcoming Functions & Events";
    cin >> functionsEventsInput;

    switch (functionsEventsInput)
    {
    case 1:
        cout << "Recent Functions & Events\n";
        cout << "1. Matariki\n2. Mebee School Fair\n3. Mebee School Dance\n";
        cout << "Please choose an option\n";
        cin >> recentInput;
        break;
    case 2:
        cout << "Upcoming Functions & Events\n";
        cout << "1. Midterm Assessments\n2.";

        break;
    }
}

void importantDates()
{
    int importantDateInput;
    cout << "Choose an option to view\n";
    cout << "1. Term 1\n2. Term 2\n3. Term 3\n4. Term 4\n";
    cin >> importantDateInput;

    switch (importantDateInput)
    {
    case 1:
        cout << "Term 1: 3rd February - 14th April 2022\n\n";
        cout << "Tuesday 1st February       Teacher only day\n";
        cout << "Wednesday 2nd February     Teacher only day\n";
        cout << "Thursday 3rd February      Learning conferences\n";
        cout << "Friday 4th February        Learning conferences\n";
        cout << "Tuesday 8th February       Term 1 classes start\n";
        cout << "Thursday 14th April        Term 1 ends\n\n";
        cout << "Public Holidays\n";
        cout << "Monday 7th February        Waitangi Day observed\n";
        cout << "Friday 15th April  	    Good Friday\n";
        cout << "Term 1 Holidays            Saturday 16th April - Sunday 1st May 2022\n";
        cout << "School holidays includes Easter Monday, Easter Tuesday and ANZAC day.\n";
        break;
    case 2:
        cout << "Term 2: 2nd May - 8th July 2022\n\n";
        cout << "Monday 2nd May 	        Term 2 classes start\n";
        cout << "Friday 3rd June            Teacher only day\n";
        cout << "Friday 8th July            Teacher only day\n\n";
        cout << "Public Holidays\n";
        cout << "Monday 6th June  	        Queens Birthday\n";
        cout << "Friday 24th  June          Matariki\n";
        cout << "Term 2 Holidays            Saturday 9th July - Sunday 24th July 2022\n";
        break;
    case 3:
        cout << "Term 3 25th July - 30th September 2022\n\n";
        cout << "Monday 25th July           Term 3 classes start\n";
        cout << "Friday 26th August         Teacher only day\n\n";
        cout << "No Public holidays\n";
        cout << "Term 3 Holidays            Saturday 1st October - Sunday 16th October 2022\n";
        break;
    case 4:
        cout << "Term 4 17th October - 16th December 2022\n\n";
        cout << "Monday 17th October        Term 4 classes start\n";
        cout << "Friday 21st October        Teacher only\n\n";
        cout << "Public Holidays :\n";
        cout << "Monday 24th October        Labour Day\n";
        cout << "Term 4 Christmas school holidays start Saturday 17th December 2022\n";
        break;
    }
}

// Main Function
int main()
{
    loadClassrooms();
    int startPageInput;
    int teacherInput;
    int parentInput;
    bool mainMenuActive = true;

    while (mainMenuActive == true)
    {
        cout << "Welcome to Mebee College\n";
        cout << "\nPlease choose an option from the menu\n";
        cout << "1. School Functions & Events\n2. Term Dates\n";
        cout << "3. Parent Register & Login\n4. Teacher Register & Login\n";
        cout << "5. Administrator Login\n6. Contact Us\n7. Exit\n";
        cin >> startPageInput;

        switch (startPageInput)
        {
        case 1:
            functionsEvents();
            break;
        case 2:
            importantDates();
            break;
        case 3:
            cout << "Would you like to register or login?\n";
            cout << "1. Register\n2. Login\n";
            cin >> parentInput;
            switch (parentInput)
            {
            case 1:
                parentRegister();
                break;
            case 2:
                parentLogin();
                break;
            }
            break;
        case 4:
            cout << "Would you like to register or login?\n";
            cout << "1. Register\n2. Login\n";
            cin >> teacherInput;
            switch (teacherInput)
            {
            case 1:
                teacherRegister();
            case 2:
                teacherLogin();
            }
            break;
        case 5:
            adminLogin();
            break;
        case 6:
            contactInfo();
            break;
        case 7:
            cout << "You have chosen to Exit the program, goodbye!\n";
            mainMenuActive == false;
            return 0;

        }
    }
}