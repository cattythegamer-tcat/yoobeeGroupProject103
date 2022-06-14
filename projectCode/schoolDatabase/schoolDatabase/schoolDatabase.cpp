// schoolDatabase.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <algorithm>
#include <filesystem>

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

// Convert these vectors to files if time allows
vector<string> admins = {
    "Jayden", "e5c45796ab12b638d3ed06c71747625f018a74c3c83b32ef4429a9c0b5da5a95", // letmein123
    "Conor", "bb8dced85e0a71d285e69ebe6edc5387093202f1f07ddba5de85c2eb95313e9a", // smurf01
    "Kevin", "a9ecab845fa6559c418a6cf96e16cca21eb14d3a7b9d0539e3a890f920028fa6" // defaultGreen
};

vector<string> subjectOrder = {
    "Maths",
    "Science",
    "English",
    "Health",
    "P.E",
    "Digital Technologies",
    "Art",
    "Social Studies",
    "Music"
};

// Saves all classroom data stored in classroomRecords
void saveClassrooms() {
    // Opens masterClassList file, containing data of all class file locations
    ofstream masterClassList;
    masterClassList.open("masterClassList.csv", ofstream::out | ofstream::trunc); // Clears current masterFile

    if (!masterClassList.is_open()) {
        cout << "\nNo master class list file created, ending classroom save attempt!\n";
        return;
    }
    ofstream classManager; // Opener for each class file
    for (int room = 0; room < classroomRecords.size(); room++) { // Loops through each classroom
        // Opens a new file for each class, as requested in the assess. requirements
        classManager.open("class" + std::to_string(classroomRecords[room].classNumber) + ".csv");
        if (!classManager.is_open()) {
            cout << "\nClass file " << classroomRecords[room].classNumber << " not found, skipping!\n";
            continue;
        }
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

    if (!masterClassList.is_open()) {
        cout << "\nNo master class list file found, ending classroom load attempt!\n";
        return;
    }
    // Opener for each class file
    ifstream classManager;
    // File line variables
    string classFile, studentInfo;
    while (std::getline(masterClassList, classFile)) { // Loops through each class
        classManager.open(classFile);

        if (!classManager.is_open()) {
            cout << "\nNo class at file " << classFile << " found, skipping class loading!\n";
            continue;
        }
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
    string name, email, contactNum, username, password = "", rePassword = "";
    char addChild = 'y'; // When this is 'n', stops adding children to children vector
    int gender, birthDay, birthMonth, birthYear;
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
    cout << "Enter gender (1:Male, 2:Female, 3:Non-Binary): ";
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
    if (!parentAccounts.is_open()) {
        cout << "\nNo parent account file found, ending registration attempt!\n";
        return;
    }
    // Write new parent info to file
    parentAccounts << username << ","
        << password << ","
        << name << ","
        << email << ","
        << gender - 1 << ","
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
        parentAccounts << ",";
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

// Admin controls menu
void adminMenu(string username) {
    system("cls");
    cout << "Welcome " << username << "!\n";
    // Admin menu loop
    while (true) {
        // All vectors, strings etc needed by switch statements initialized here, since they can't be initialized inside of switch statement
        vector<int> classroomNums = {};
        vector<vector<string>> loadedParentAccounts;
        string genderOptions[3] = { "Male", "Female", "Non-Binary" };
        string gradeOptions[4] = { "Not Achieved", "Achieved", "Merit", "Excellence" };
        string parentUsername, parentName, fetchedParent, fetchedName, password, rePassword, studentName;
        ifstream fileIn;
        ofstream fileOut;
        SHA256 sha;
        uint8_t* digest;
        int parentIdx = -1;
        // Control options
        int adminOption;
        cout << "\n1. View class record/s\n" <<
            "2. Update class\n" <<
            "3. View parent record/s\n" <<
            "4. Update parent record\n" <<
            "5. View student record/s\n" <<
            "6. Update student record\n" <<
            "7. Logout\n" <<
            ": ";
        cin >> adminOption;
        // Individual option code
        switch(adminOption) {
        // View classroom records
        case 1:
            if (classroomRecords.size() == 0) {
                cout << "Alert! No classes exist!";
                continue;
            }
            int classOption;
            cout << "Please enter the class number, or type \"0\" for a list of all class numbers: ";
            cin >> classOption;
            if (classOption == 0) {
                cout << "Classes: " << classroomRecords[0].classNumber;
                for (int classroomIdx = 1; classroomIdx < classroomRecords.size(); classroomIdx++) {
                    cout << ", " << classroomRecords[classroomIdx].classNumber;
                }
                cout << "\n";
            }
            else {
                for (int classroomIdx = 0; classroomIdx < classroomRecords.size(); classroomIdx++) {
                    if (classroomRecords[classroomIdx].classNumber == classOption) {
                        int rollCount = classroomRecords[classroomIdx].students.size();
                        string teacherName = "Unknown";
                        // Need to find actual teacher name here

                        // Display general class info
                        cout << "Classroom " << classOption << " records:\n" <<
                            "Teacher: " << teacherName << 
                            "\nRoll (" << rollCount << "):";
                        // Display individual student info
                        for (int studentIdx = 0; studentIdx < rollCount; studentIdx++) {
                            cout << "\n\t" << classroomRecords[classroomIdx].students[studentIdx].name <<
                                " (" << 
                                genderOptions[classroomRecords[classroomIdx].students[studentIdx].gender] <<
                                "): \n";
                            int subjectGradeCount = classroomRecords[classroomIdx].students[studentIdx].subjectGrades.size();
                            for (int subjectIdx = 0; subjectIdx < subjectGradeCount; subjectIdx++) {
                                vector<int> subjectGrade = classroomRecords[classroomIdx].students[studentIdx].subjectGrades[subjectIdx];
                                cout << "\t\t" << subjectOrder[subjectGrade[0]] << ": "
                                    << gradeOptions[subjectGrade[1]] << "\n";
                            }
                        }
                    }
                }
            }
            break;
        // Update class record
        case 2:
            if (classroomRecords.size() == 0) {
                cout << "Alert! No classes exist!";
                continue;
            }
            int classUpdateOption, classNum;
            for (int classroomIdx = 0; classroomIdx < classroomRecords.size(); classroomIdx++) {
                classroomNums.push_back(classroomRecords[classroomIdx].classNumber);
            }
            cout << "1. Update existing class\n" <<
                "2. Delete class\n" <<
                "3. Add class\n" <<
                ": ";
            cin >> classUpdateOption;
            // All cases require a class number
            cout << "Enter class number: ";
            cin >> classNum;
            switch (classUpdateOption) {
            case 1:
                int classUpdateSelector;
                if (std::find(classroomNums.begin(), classroomNums.end(), classNum) == classroomNums.end()) {
                    cout << "Alert, class doesn't exist!";
                    break;
                }
                cout << "1. Change class number\n" <<
                    "2. Remove all students\n" <<
                    ": ";
                cin >> classUpdateSelector;
                switch (classUpdateSelector) {
                case 1:
                    int newClassNum;
                    cout << "Enter new class number: ";
                    cin >> newClassNum;
                    if (std::find(classroomNums.begin(), classroomNums.end(), newClassNum) != classroomNums.end()) {
                        cout << "Number already in use, change denied.";
                        break;
                    }
                    remove(("class" + std::to_string(classNum) + ".csv").c_str());
                    classroomRecords[std::find(classroomNums.begin(), classroomNums.end(), classNum) - classroomNums.begin()].classNumber = newClassNum;
                    saveClassrooms();
                    cout << "Change accepted: " << classNum << " -> " << newClassNum;
                    break;
                case 2:
                    char confirmChar;
                    cout << "Confirm wipe of classroom No." << classNum << " (y/n): ";
                    cin >> confirmChar;
                    if (confirmChar == 'y') {
                        classroomRecords[std::find(classroomNums.begin(), classroomNums.end(), classNum) - classroomNums.begin()].students = {};
                        saveClassrooms();
                        cout << "Wipe complete!";
                        break;
                    }
                    cout << "Wipe process terminated.";
                    break;
                }
                break;
            case 2:
                char deleteConfirmationChar;
                if (std::find(classroomNums.begin(), classroomNums.end(), classNum) == classroomNums.end()) {
                    cout << "Alert, class doesn't exist!";
                    break;
                }
                cout << "Confirm deletion of class No." << classNum << " (y/n): ";
                cin >> deleteConfirmationChar;
                if (deleteConfirmationChar == 'y') {
                    remove(("class" + std::to_string(classNum) + ".csv").c_str());
                    classroomRecords.erase(classroomRecords.begin() + (std::find(classroomNums.begin(), classroomNums.end(), classNum) - classroomNums.begin()));
                    saveClassrooms();
                    cout << "Deletion of class No." << classNum << " complete.";
                    break;
                }
                cout << "Deletion terminated.";
                break;
            case 3:
                classroomRecords.push_back(classroom(classNum, {}));
                cout << "Added empty class No." << classNum;
                break;
            }
            break;
            // View parent records
        case 3:
            fileIn.open("parentAccounts.csv");
            if (!fileIn.is_open()) {
                cout << "\nNo parent accounts exist!\n";
                continue;
            }

            cout << "Please enter parent name, or type \"ALL\" to view all parent names: ";
            cin.ignore(); // Needed as parent name can have spaces
            getline(cin, parentName);

            if (parentName == "ALL") {
                cout << "Parents:";
                string fetchedParent;
                while (std::getline(fileIn, fetchedParent)) {
                    string fetchedName;
                    int dataPos = 0; // dataPos used to determine which piece of info below loop is writing to.
                    for (char& c : fetchedParent) {
                        if (c == ',') dataPos++;
                        else if (dataPos == 2) fetchedName += c;
                    }
                    cout << "\n\t" << fetchedName;
                }
                break;
            }
            else {
                while (std::getline(fileIn, fetchedParent)) {
                    fetchedName = "";
                    string username, email, gender, birthDay, birthMonth, birthYear, phoneNumber;
                    vector<vector<string>> childrenInfo;
                    int dataPos = 0; // dataPos used to determine which piece of info below loop is writing to.
                    for (char& c : fetchedParent) {
                        if (c == ',') {
                            dataPos++;
                            if (dataPos >= 10) childrenInfo[childrenInfo.size() - 1].push_back("");
                            continue;
                        }
                        switch (dataPos) {
                        case 0:
                            username += c;
                            break;
                        case 1:
                            continue;
                        case 2:
                            fetchedName += c;
                            break;
                        case 3:
                            email += c;
                            break;
                        case 4:
                            gender += c;
                            break;
                        case 5:
                            birthDay += c;
                            break;
                        case 6:
                            birthMonth += c;
                            break;
                        case 7:
                            birthYear += c;
                            break;
                        case 8:
                            phoneNumber += c;
                            break;
                        default:
                            if (c == '[') {
                                if (childrenInfo.size() > 0) {
                                    childrenInfo[childrenInfo.size() - 1].pop_back();
                                }
                                childrenInfo.push_back({ "" });
                            }
                            else childrenInfo[childrenInfo.size() - 1][childrenInfo[childrenInfo.size() - 1].size() - 1] += c;
                        }
                    }
                    if (fetchedName == parentName) {
                        string genderOptions[3] = { "Male", "Female", "Non-Binary" };
                        cout << "Stored information for " << parentName <<
                            ":\n\tUsername: " << username <<
                            "\n\tEmail: " << email <<
                            "\n\tGender: " << genderOptions[std::stoi(gender)] <<
                            "\n\tD.O.B: " << birthDay << "/" << birthMonth << "/" << birthYear <<
                            "\n\tPhone Number: " << phoneNumber <<
                            "\nChildren:";
                        for (int childIdx = 0; childIdx < childrenInfo.size(); childIdx++) {
                            cout << "\n\t" << childrenInfo[childIdx][0] <<
                                " - Classroom No. " << childrenInfo[childIdx][1] <<
                                "\n\tCaregiver Contact Info" << (childrenInfo[childIdx].size() > 4 ? "s:" : ":");
                            for (int caregiverIdx = 2; caregiverIdx < childrenInfo[childIdx].size(); caregiverIdx++) {
                                if (caregiverIdx % 2 == 1) continue;
                                cout << "\n\t\t" << childrenInfo[childIdx][caregiverIdx] << ": " <<
                                    childrenInfo[childIdx][caregiverIdx + 1] << childrenInfo[childIdx].size();
                            }
                        }
                        break;
                    }
                }
                if (fetchedName != parentName) cout << "No parent with that name exists!";
                fileIn.close();
                break;
            }
        // Update parent record
        case 4:
            fileIn.open("parentAccounts.csv");
            if (!fileIn.is_open()) {
                cout << "\nNo parent accounts exist!\n";
                continue;
            }
            while (std::getline(fileIn, fetchedParent)) {
                loadedParentAccounts.push_back({});
                loadedParentAccounts[loadedParentAccounts.size() - 1].push_back("");
                for (char& c : fetchedParent) {
                    if (c == ',') {
                        loadedParentAccounts[loadedParentAccounts.size() - 1].push_back("");
                        continue;
                    }
                    loadedParentAccounts[loadedParentAccounts.size() - 1][loadedParentAccounts[loadedParentAccounts.size() - 1].size() - 1] += c;
                }
            }
            int parentUpdateOption;
            cout << "1. Delete parent record\n" <<
                "2. Change parent username & password\n" <<
                "3. Change parent name\n" <<
                "4. Update parent email\n" <<
                "5. Update parent number\n" <<
                "6. Change parent gender\n" <<
                "7. Re-assign children info\n" <<
                ": ";
            cin >> parentUpdateOption;
            cout << "Enter name of parent: ";
            cin.ignore();
            std::getline(cin, parentName);
            for (int parentMatchIdx = 0; parentMatchIdx < loadedParentAccounts.size(); parentMatchIdx++) {
                if (loadedParentAccounts[parentMatchIdx][2] == parentName) parentIdx = parentMatchIdx;
            }
            if (parentIdx == -1) {
                cout << "Alert! Parent not found.";
                break;
            }
            switch (parentUpdateOption) {
            case 1: // Delete parent
                char parentDeletionConfirmation;
                cout << "Confirm deletion of " << parentName << " (y/n): ";
                cin >> parentDeletionConfirmation;
                if (parentDeletionConfirmation == 'y') {
                    loadedParentAccounts.erase(loadedParentAccounts.begin() + parentIdx);
                    cout << "Deletion complete.";
                    break;
                }
                cout << "Cancelled parent deletion.";
                break;
            case 2: // Change username & password (Must be changed together, since password is salted with username)
                cout << "Enter new username: ";
                cin >> loadedParentAccounts[parentIdx][0];
                while (true) {
                    cout << "Enter new password: ";
                    cin >> password;
                    cout << "Repeat password: ";
                    cin >> rePassword;
                    if (password == rePassword) break;
                    else cout << "Error! Passwords don't match.\n";
                }

                sha.update(loadedParentAccounts[parentIdx][0] + password);
                digest = sha.digest();
                loadedParentAccounts[parentIdx][1] = SHA256::toString(digest);

                cout << "Username and password have been updated successfully.";
                break;
            case 3: // Change name
                cout << "Enter new name: ";
                if (cin.peek() != '\n') cin.putback('\n');
                cin.ignore();
                getline(cin, loadedParentAccounts[parentIdx][2]);
                cout << "Name change complete.";
                break;
            case 4: // Change email
                cout << "Enter new email: ";
                cin >> loadedParentAccounts[parentIdx][3];
                cout << "Email change complete.";
                break;
            case 5: // Change number
                cout << "Enter new number: ";
                cin >> loadedParentAccounts[parentIdx][8];
                cout << "Phone number change complete.";
                break;
            case 6: // Change gender
                cout << "Enter new gender (1:Male, 2:Female, 3:Non-Binary): ";
                cin >> loadedParentAccounts[parentIdx][4];
                loadedParentAccounts[parentIdx][4] = stoi(loadedParentAccounts[parentIdx][4]) - 1;
                cout << "Gender updated.";
                break;
            case 7: // Re-assign children info
                while (loadedParentAccounts[parentIdx].size() > 9) loadedParentAccounts[parentIdx].pop_back();
                char addChildCheck, addCaregiverCheck;
                do {
                    cout << "Enter child name: ";
                    if (cin.peek() != '\n') cin.putback('\n');
                    cin.ignore();
                    loadedParentAccounts[parentIdx].push_back("");
                    getline(cin, loadedParentAccounts[parentIdx][loadedParentAccounts[parentIdx].size() - 1]);
                    loadedParentAccounts[parentIdx][loadedParentAccounts[parentIdx].size() - 1] = "[" + loadedParentAccounts[parentIdx][loadedParentAccounts[parentIdx].size() - 1];
                    cout << "Enter child classroom number: ";
                    loadedParentAccounts[parentIdx].push_back("");
                    cin >> loadedParentAccounts[parentIdx][loadedParentAccounts[parentIdx].size() - 1];
                    do {
                        cout << "Enter caregiver name: ";
                        cin.ignore();
                        loadedParentAccounts[parentIdx].push_back("");
                        getline(cin, loadedParentAccounts[parentIdx][loadedParentAccounts[parentIdx].size() - 1]);
                        cout << "Enter caregiver emergency contact number: ";
                        loadedParentAccounts[parentIdx].push_back("");
                        cin >> loadedParentAccounts[parentIdx][loadedParentAccounts[parentIdx].size() - 1];
                        cout << "Add another caregiver (y/n)? ";
                        cin >> addCaregiverCheck;
                    } while (addCaregiverCheck != 'n');
                    cout << "Add another child (y/n)? ";
                    cin >> addChildCheck;
                } while (addChildCheck != 'n');
                cout << "Children updated.";
                break;
            }
            fileIn.close();
            fileOut.open("parentAccounts.csv");
            for (int parentAccIdx = 0; parentAccIdx < loadedParentAccounts.size(); parentAccIdx++) {
                fileOut << loadedParentAccounts[parentAccIdx][0];
                for (int parentItemIdx = 1; parentItemIdx < loadedParentAccounts[parentAccIdx].size(); parentItemIdx++) {
                    fileOut << "," << loadedParentAccounts[parentAccIdx][parentItemIdx];
                }
                fileOut << endl;
            }
            fileOut.close();
            break;
        // View student records
        case 5:
            int studentRecordViewOption;
            cout << "1. Show individual student\n" <<
                "2. Show students by grade\n" <<
                ": ";
            cin >> studentRecordViewOption;
            switch (studentRecordViewOption) {
            case 1: // Show individual student
                cout << "Enter student name: ";
                if (cin.peek() != '\n') cin.putback('\n');
                cin.ignore();
                getline(cin, studentName);
                // Needs to be seperated to avoid switch bug
                bool studentFound;
                studentFound = false;
                for (int classIdx = 0; classIdx < classroomRecords.size() && !studentFound; classIdx++) {
                    for (int studentIdx = 0; studentIdx < classroomRecords[classIdx].students.size() && !studentFound; studentIdx++) {
                        if (classroomRecords[classIdx].students[studentIdx].name == studentName) {
                            vector<string> caregiverInfosDups; // Initial version, which may contain repeated parent infos
                            vector<string> caregiverInfos; // Cleaned version, with only one instance of each parent
                            cout << genderOptions[classroomRecords[classIdx].students[studentIdx].gender] <<
                                " - Classroom No." << classroomRecords[classIdx].classNumber <<
                                "\nCurrent Term Grades:";
                            for (int subjectIdx = 0; subjectIdx < classroomRecords[classIdx].students[studentIdx].subjectGrades.size(); subjectIdx++) {
                                cout << "\n\t" <<
                                    subjectOrder[classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx][0]] <<
                                    ": " << gradeOptions[classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx][1]];
                            }
                            fileIn.open("parentAccounts.csv");
                            if (!fileIn.is_open()) {
                                cout << "\nNo parent account collections found!\n";
                                break;
                            }
                            while (std::getline(fileIn, fetchedParent)) {
                                string currentInfoChunk;
                                bool studentRelated = false, fetchingClassNum = true;
                                for (char& c : fetchedParent) {
                                    if (c == ',') {
                                        if (currentInfoChunk.find(studentName) != string::npos) studentRelated = true;
                                        else if (studentRelated) {
                                            if (fetchingClassNum) fetchingClassNum = false;
                                            else caregiverInfosDups.push_back(currentInfoChunk);
                                        }
                                        currentInfoChunk = "";
                                    }
                                    else if (studentRelated && c == '[') break;
                                    else currentInfoChunk += c;
                                }
                                if (studentRelated && currentInfoChunk != "") {
                                    caregiverInfosDups.push_back(currentInfoChunk);
                                }
                            }
                            fileIn.close();

                            if (caregiverInfosDups.size() > 0) cout << "\nParent/Guardian details: ";
                            for (int caregiverIdx = 0; caregiverIdx < caregiverInfosDups.size() / 2; caregiverIdx++) {
                                if (find(caregiverInfos.begin(), caregiverInfos.end(), caregiverInfosDups[caregiverIdx * 2]) == caregiverInfos.end()) {
                                    caregiverInfos.push_back(caregiverInfosDups[caregiverIdx * 2]);
                                    cout << "\n\t" << caregiverInfosDups[caregiverIdx * 2] <<
                                        ": " << caregiverInfosDups[caregiverIdx * 2 + 1];
                                }
                            }
                            studentFound = true;
                        }
                    }
                }
                if (!studentFound) cout << "No such student exists!";
                break;
            case 2:
                int subject, grade, startingClassNum, rollCounter;
                rollCounter = 0;
                cout << "Enter subject:";
                for (int subjectIdx = 0; subjectIdx < subjectOrder.size(); subjectIdx++) {
                    cout << "\n" << subjectIdx + 1 << ". " << subjectOrder[subjectIdx];
                }
                cout << "\n: ";
                cin >> subject;
                subject--;
                cout << "Enter grade:" <<
                    "\n1. Not Achieved" <<
                    "\n2. Achieved" <<
                    "\n3. Merit" <<
                    "\n4. Excellence" <<
                    "\n: ";
                cin >> grade;
                grade--;
                cout << "Enter class num (Or type \"0\" for all classes): ";
                cin >> startingClassNum;
                for (int classIdx = startingClassNum; classIdx < classroomRecords.size(); classIdx++) {
                    for (int studentIdx = 0; studentIdx < classroomRecords[classIdx].students.size(); studentIdx++) {
                        for (int subjectIdx = 0; subjectIdx < classroomRecords[classIdx].students[studentIdx].subjectGrades.size(); subjectIdx++) {
                            vector<int> subjectGrade = classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx];
                            if (subjectGrade[0] == subject && subjectGrade[1] == grade) {
                                rollCounter++;
                                studentName = classroomRecords[classIdx].students[studentIdx].name;
                                cout << classroomRecords[classIdx].students[studentIdx].name << " (" <<
                                    genderOptions[classroomRecords[classIdx].students[studentIdx].gender] <<
                                    ") - Class No." << classIdx + 1;

                                vector<string> caregiverInfosDups; // Initial version, which may contain repeated parent infos
                                vector<string> caregiverInfos; // Cleaned version, with only one instance of each parent

                                fileIn.open("parentAccounts.csv");
                                if (!fileIn.is_open()) {
                                    cout << "\nNo parent account collections found!\n";
                                    break;
                                }
                                while (std::getline(fileIn, fetchedParent)) {
                                    string currentInfoChunk;
                                    bool studentRelated = false, fetchingClassNum = true;
                                    for (char& c : fetchedParent) {
                                        if (c == ',') {
                                            if (currentInfoChunk.find(studentName) != string::npos) studentRelated = true;
                                            else if (studentRelated) {
                                                if (fetchingClassNum) fetchingClassNum = false;
                                                else caregiverInfosDups.push_back(currentInfoChunk);
                                            }
                                            currentInfoChunk = "";
                                        }
                                        else if (studentRelated && c == '[') break;
                                        else currentInfoChunk += c;
                                    }
                                    if (studentRelated && currentInfoChunk != "") {
                                        caregiverInfosDups.push_back(currentInfoChunk);
                                    }
                                }
                                fileIn.close();

                                for (int caregiverIdx = 0; caregiverIdx < caregiverInfosDups.size() / 2; caregiverIdx++) {
                                    if (find(caregiverInfos.begin(), caregiverInfos.end(), caregiverInfosDups[caregiverIdx * 2]) == caregiverInfos.end()) {
                                        caregiverInfos.push_back(caregiverInfosDups[caregiverIdx * 2]);
                                        cout << "\n\t" << caregiverInfosDups[caregiverIdx * 2] <<
                                            ": " << caregiverInfosDups[caregiverIdx * 2 + 1];
                                    }
                                }
                                cout << endl;
                                break;
                            }
                        }
                    }
                    if (startingClassNum != 0) classIdx = classroomRecords.size();
                }
            }
        // Update student records
        case 6:
            break;
        // Return to main menu
        case 7:
            system("cls");
            return;
        }
    }
}

// Parent signin
void parentLogin()
{
    // Account String Declarations
    string username, password, fetchedInfo;
    // Stores all username-password hashes in parentAccount DB
    vector<string> accounts;
    // Fetches parentAccounts file for populating accounts Vector
    ifstream parentAccounts;
    parentAccounts.open("parentAccounts.csv");
    if (!parentAccounts.is_open()) {
        cout << "No parent accounts exist!";
        return;
    }
    // Populates accounts Vector with all account hashes
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
        accounts.push_back(fetchedPassword);
    }
    parentAccounts.close();
    // User signin input system
    char attemptingSignin = 'y';
    for (int attempt = 0; attempt < 3 && attemptingSignin != 'n'; attempt++) {
        cout << "Please enter your username: ";
        cin >> username;
        cout << "Please enter your password: ";
        cin >> password;
        // Forms username-password hash combo
        SHA256 sha;
        sha.update(username + password);
        uint8_t* digest = sha.digest();
        string hashedPass = SHA256::toString(digest);
        // Checks if hash combo is a valid account
        if (std::find(accounts.begin(), accounts.end(), hashedPass) != accounts.end()) {
            cout << "\nSigned in successfully!\n";
            return;
        }
        if (attempt == 2) continue;
        cout << "\nUsername or password is incorrect.\nTry again (y/n)? "; // Allows for exit if user can't remember password
        cin >> attemptingSignin;
    }
    cout << ((attemptingSignin == 'n') ? 
        "\nUnsuccessful - Login attempt terminated.\n" : 
        "\nUnsuccesful - Too many signin attempts. Please try again later.\n");
}

void teacherLogin()
{
    cout << "Teacher Login Page\n";
}

void adminLogin()
{
    string username, password;
    bool foundMatch = false;
    // Admin signin input system
    char attempting = 'y';
    for (int attempt = 0; attempt < 3 && !foundMatch && attempting != 'n'; attempt++) {
        cout << "Please enter your username (Case-sensitive): ";
        cin >> username;
        cout << "Please enter your password: ";
        cin >> password;
        SHA256 sha;
        sha.update(username + password);
        uint8_t* digest = sha.digest();
        string hashedPass = SHA256::toString(digest);
        if (std::find(admins.begin(), admins.end(), hashedPass) != admins.end()) {
            adminMenu(username);
            return;
        }
        if (attempt == 2) continue;
        cout << "\nUsername or password is incorrect.\nTry again (y/n)? "; // Allows for exit if user can't remember password
        cin >> attempting;
    }
    cout << ((attempting == 'n') ?
        "\nUnsuccessful - Login attempt terminated.\n" : 
        "\nUnsuccesful - Too many signin attempts. Please try again later.\n");
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
        cout << "Welcome to Mebee College\n\n"
            << "Please choose an option from the menu\n"
            << "1. School Functions & Events\n" 
            << "2. Term Dates\n"
            << "3. Parent Register & Login\n" 
            << "4. Teacher Register & Login\n"
            << "5. Administrator Login\n"
            << "6. Contact Us\n" 
            << "7. Exit\n"
            << ": ";
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
            cout << "You have chosen to exit the program, goodbye!\n";
            mainMenuActive = false;
            return 0;

        }
    }
}