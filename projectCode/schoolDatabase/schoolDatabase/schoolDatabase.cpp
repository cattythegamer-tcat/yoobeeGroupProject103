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
using std::getline;

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
    string name, phoneNum;
    // Caregiver constructor
    caregiver(string fullName, string number) {
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
        student("John Doe", 1, {{1, 1, 1}, {2, 2, 1}, {3, 3, 2}}),
        student("Jane Doe", 1, {{1, 2, 1}, {2, 3, 1}, {3, 1, 2}}),
        student("Jack Doe", 0, {{1, 3, 1}, {2, 1, 1}, {3, 2, 2}})
        }
    ),
    classroom(2, {
        student("Jill Doe", 1, {{1, 1, 2}, {2, 2, 1}, {3, 3, 3}}),
        student("Jared Doe", 0, {{1, 2, 2}, {2, 3, 1}, {3, 1, 3}}),
        student("Jenny Doe", 1, {{1, 3, 2}, {2, 1, 1}, {3, 2, 3}})
        }
    ),
    classroom(3, {
        student("Jasmine Doe", 1, {{1, 1, 1}, {2, 2, 4}, {3, 3, 1}}),
        student("Jasper Doe", 0, {{1, 2, 1}, {2, 3, 4}, {3, 1, 1}}),
        student("Josie Doe", 1, {{1, 3, 1}, {2, 1, 4}, {3, 2, 1}})
        }
    )
    */
};

// Teachers
vector<string> teachers = {}; // name, classroom number

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

// General, event, & date info loaded from files
vector<vector<string>> recentEvents = {};

vector<vector<string>> upcomingEvents = {};

vector<string> term1Dates = {};

vector<string> term2Dates = {};

vector<string> term3Dates = {};

vector<string> term4Dates = {};

string schoolName = "";
string schoolAddress = "Level 6, Wellington Railway Station\nWellington\nNew Zealand";
string schoolPhoneNumber = "";
string schoolEmailAddress = "";

// Returns a user-inputted int that matches a given range
int getInt(int lowerLimit = 0, int upperLimit = 100000) {
    string input;
    cin >> input;
    // Checks something has been inputted
    if (input == "") {
        cout << "Alert! Nothing inputted, please try again: ";
        return getInt(lowerLimit, upperLimit);
    }
    // Checks input is a number
    for (int inputChar = 0; inputChar < input.size(); inputChar++) {
        if (input[inputChar] < 47 || input[inputChar] > 57) {
            cout << "\nError: Input must be a number\nPlease try again: ";
            return getInt(lowerLimit, upperLimit); // Repeats function
        }
    }
    int inputInt = stoi(input);
    // Checks number is within range
    if (inputInt < lowerLimit) { // Lower limit
        cout << "\nError: Input must be greater or equal to " << lowerLimit << "\nPlease try again: ";
        return getInt(lowerLimit, upperLimit); // Repeats function
    }
    else if (inputInt > upperLimit) { // Upper limit
        cout << "\nError: Input must be less than or equal to " << upperLimit << "\nPlease try again: ";
        return getInt(lowerLimit, upperLimit); // Repeats function
    }
    // Returns integer
    return inputInt;
}

// Returns a user-inputted char
char getChar(string options = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") {
    string input;
    cin >> input;
    if (input.size() > 1) { // Checks only one character has been inputted
        cout << "\nError: Multiple characters inputted\nPlease try again: ";
        return getChar(options);
    }
    else if (options.find(input) == string::npos) { // Checks character is valid
        cout << "\nError: Character not recognized\nPlease try again: ";
        return getChar(options);
    }
    // Returns character
    return input[0];
}

// Returns a string with spaces
string getSpaced() {
    string input = "";
    if (cin.peek() != '\n') cin.putback('\n'); // Stops first char from being omitted during cin
    cin.ignore();
    getline(cin, input);
    // Prevents , from breaking csv files
    if (input.find(",") != string::npos || input == ",") {
        cout << "Alert! Illegal character \",\" found, please re-input without this character:\n";
        return getSpaced();
    }
    // Checks something has been inputted
    if (input == "") {
        cout << "Alert! Nothing inputted, please try again:\n";
        return getSpaced();
    }
    return input;
}

// Get teacher name
string getTeacher(int classNumber) {
    if (std::find(teachers.begin(), teachers.end(), std::to_string(classNumber)) != teachers.end())
        return teachers[std::find(teachers.begin(), teachers.end(), std::to_string(classNumber)) - teachers.begin() - 1];
    else return "Unknown";
}
// Get teacher name overload where classNum is string
string getTeacher(string classNumber) {
    if (std::find(teachers.begin(), teachers.end(), classNumber) != teachers.end())
        return teachers[std::find(teachers.begin(), teachers.end(), classNumber) - teachers.begin() - 1];
    else return "Unknown";
}

// Saves all classroom data stored in classroomRecords
void saveSchool() {
    // Opens masterClassList file, containing data of all class file locations
    ofstream masterClassList;
    masterClassList.open("masterClassList.csv", ofstream::trunc); // Clears current masterFile

    if (!masterClassList.is_open()) { // Checks file exists
        cout << "\nNo master class list file created, ending classroom save attempt!\n";
    } else {
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
                        << "," << classroomRecords[room].students[studentNum].subjectGrades[sub][1]
                        << "," << classroomRecords[room].students[studentNum].subjectGrades[sub][2];
                }
                classManager << endl;
            }
            classManager.close();
            // Saves class file location to masterClassList
            masterClassList << "class" << classroomRecords[room].classNumber << ".csv" << endl; // NOTE: FileLoc stored instead of classNum for futureproofing, to allow for differing directiories
        }
        masterClassList.close();
    }
    // Opens general information file
    ofstream generalInfo;
    generalInfo.open("generalInfo.csv", ofstream::trunc);
    if (!generalInfo.is_open()) { // Checks file exists
        cout << "\nNo general info file created, ending save attempt\n";
    }
    else {
        // Stores school info variables
        generalInfo << schoolName << "\n" <<
            schoolEmailAddress << "\n" <<
            schoolPhoneNumber << endl;
        // Stores recent events vector
        for (int recentIdx = 0; recentIdx < recentEvents.size(); recentIdx++) {
            if (recentIdx != 0) generalInfo << ",";
            generalInfo << recentEvents[recentIdx][0] << "," << recentEvents[recentIdx][1];
        }
        // Stores upcoming events vector
        for (int upcomingIdx = 0; upcomingIdx < upcomingEvents.size(); upcomingIdx++) {
            if (upcomingIdx == 0) generalInfo << endl;
            else generalInfo << ",";
            generalInfo << upcomingEvents[upcomingIdx][0] << "," << upcomingEvents[upcomingIdx][1];
        }
        // Stores term 1 dates vector
        for (int termIdx = 0; termIdx < term1Dates.size(); termIdx++) {
            if (termIdx == 0) generalInfo << endl;
            else generalInfo << ",";
            generalInfo << term1Dates[termIdx];
        }
        //  Term 2
        for (int termIdx = 0; termIdx < term2Dates.size(); termIdx++) {
            if (termIdx == 0) generalInfo << endl;
            else generalInfo << ",";
            generalInfo << term2Dates[termIdx];
        }
        //  Term 3
        for (int termIdx = 0; termIdx < term3Dates.size(); termIdx++) {
            if (termIdx == 0) generalInfo << endl;
            else generalInfo << ",";
            generalInfo << term3Dates[termIdx];
        }
        //  Term 4
        for (int termIdx = 0; termIdx < term4Dates.size(); termIdx++) {
            if (termIdx == 0) generalInfo << endl;
            else generalInfo << ",";
            generalInfo << term4Dates[termIdx];
        }
    }
}

// Loads all classroom data from filesystem
void loadSchool() {
    // Open file containing all class file locations
    ifstream masterClassList;
    masterClassList.open("masterClassList.csv");

    if (!masterClassList.is_open()) {
        cout << "\nNo master class list file found, ending classroom load attempt!\n";
    } else {
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
                        if ((dataPos-2) % 3 == 0) studentGrades.push_back({ c - '0' });
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
    // Load teacher accounts
    teachers = {""};
    ifstream teacherAccounts;
    teacherAccounts.open("teacherAccounts.csv");
    if (!teacherAccounts.is_open()) { // Checks file exists
        cout << "\nNo teacher accounts, skipping.\n";
    } else {
        string teacherInfo;
        // Loads each line in file
        while (std::getline(teacherAccounts, teacherInfo)) {
            int dataPos = 0;
            for (char& c : teacherInfo) {
                if (c == ',') {
                    dataPos++;
                    if (dataPos == 2 || dataPos == 9) teachers.push_back("");
                    continue;
                }
                else if (dataPos == 2 || dataPos == 9) teachers[teachers.size() - 1] += c;
            }
        }
        teacherAccounts.close();
    }
    // Load general info
    ifstream generalInfo;
    generalInfo.open("generalInfo.csv");
    if (!generalInfo.is_open()) { // Checks file exists
        schoolName = "Unknown";
        schoolEmailAddress = "Unknown";
        schoolPhoneNumber = "Unknown";
        cout << "\nNo school data found, skipping.\n";
    } else {
        string infoRow;
        int rowNum = 0;
        // Loads each line in file
        while (std::getline(generalInfo, infoRow)) {
            int dataPos = -1;
            infoRow = "," + infoRow;
            for (char& c : infoRow) {
                if (c == ',') {
                    dataPos++;
                    switch (rowNum) {
                    case 3:
                        if (dataPos % 2 == 1) recentEvents[recentEvents.size() - 1].push_back("");
                        else recentEvents.push_back({ "" });
                        break;
                    case 4:
                        if (dataPos % 2 == 1) upcomingEvents[upcomingEvents.size() - 1].push_back("");
                        else upcomingEvents.push_back({ "" });
                        break;
                    case 5:
                        term1Dates.push_back("");
                        break;
                    case 6:
                        term2Dates.push_back("");
                        break;
                    case 7:
                        term3Dates.push_back("");
                        break;
                    case 8:
                        term4Dates.push_back("");
                        break;
                    }
                }
                else {
                    switch (rowNum) {
                    case 0:
                        schoolName += c;
                        break;
                    case 1:
                        schoolEmailAddress += c;
                        break;
                    case 2:
                        schoolPhoneNumber += c;
                        break;
                    case 3:
                        recentEvents[recentEvents.size() - 1][recentEvents[recentEvents.size() - 1].size() - 1] += c;
                        break;
                    case 4:
                        upcomingEvents[upcomingEvents.size() - 1][upcomingEvents[upcomingEvents.size() - 1].size() - 1] += c;
                        break;
                    case 5:
                        term1Dates[term1Dates.size() - 1] += c;
                        break;
                    case 6:
                        term2Dates[term2Dates.size() - 1] += c;
                        break;
                    case 7:
                        term3Dates[term3Dates.size() - 1] += c;
                        break;
                    case 8:
                        term4Dates[term4Dates.size() - 1] += c;
                        break;
                    }
                }
            }
            rowNum++;
        }
    }

}

// Adds a new parent
void parentRegister() {
    // Parent variable declarations
    string name, email, contactNum, username, password = "", rePassword = "";
    char addChild = 'y'; // When this is 'n', stops adding children to children vector
    int gender, birthDay, birthMonth, birthYear;
    vector<child> children;

    // General account info
    cout << "Parent details:\n\nEnter name below:\n";
    name = getSpaced();
    cout << "Enter email: ";
    email = getSpaced();
    cout << "Enter username: ";
    username = getSpaced();

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
    gender = getInt(1, 3);
    cout << "Enter day of birth (1-31): ";
    birthDay = getInt(1, 31);
    cout << "Enter month of birth (1-12): ";
    birthMonth = getInt(1, 12);
    cout << "Enter year of birth (1900-2022): ";
    birthYear = getInt(1900, 2022);
    cout << "Enter phone number: ";
    contactNum = getSpaced();
    // Assigns given children to parent
    while (addChild != 'n') {
        // Child variable declarations
        string childName;
        char addCaregiver = 'y'; // When addCaregiver is 'n'. stops adding caregivers to caregiver vector
        int childClass;
        vector<caregiver> caregivers;
        // General child info
        cout << "Enter name of child below:\n";
        childName = getSpaced();
        cout << "Enter child classroom number: ";
        childClass = getInt();

        // Gets all caregivers assoc. with child
        while (addCaregiver != 'n') {
            // Caregiver variable declarations
            string caregiverName, phoneNumber;
            // General caregiver info
            cout << "Enter caregiver name below:\n";
            caregiverName = getSpaced();
            cout << "Enter caregiver emergency phone number: ";
            phoneNumber = getSpaced();
            // Adds caregiver to child
            caregivers.push_back(caregiver(caregiverName, phoneNumber));
            // Checks if user wants to add another caregiver
            cout << "Add another caregiver (y\\n)? ";
            addCaregiver = getChar("yn");
        }
        // Adds child to parent
        children.push_back(child(childName, childClass, caregivers));
        // Checks if user wants to add another child to parent
        cout << "Add another child (y\\n)? ";
        addChild = getChar("yn");
    }
    // Calculates password match based off inputs
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
        if (childNum + 1< children.size()) parentAccounts << ",";
    }
    // Close parent file
    parentAccounts << endl;
    parentAccounts.close();
}

// Adds a new teacher
void teacherRegister() {
    string name, email, phoneNumber, username, password, rePassword;
    int gender, birthDay, birthMonth, birthYear, teachClassNum, teachYLvl;

    cout << "What is your full name: ";
    name = getSpaced();

    cout << "What is your gender (1:Male, 2:Female, 3:Non-Binary): ";
    gender = getInt(1, 3);

    cout << "What is your email address: ";
    email = getSpaced();

    cout << "What is your phone number: ";
    phoneNumber = getSpaced();

    cout << "What is your classroom number: ";
    while (true) {
        teachClassNum = getInt();
        if (std::find(teachers.begin(), teachers.end(), std::to_string(teachClassNum)) != teachers.end()) {
            cout << "Teacher already assigned to class, please enter a different class number or contact the office.\nClass number: ";
            continue;
        }
        break;
    }

    cout << "What year level do you teach: ";
    teachYLvl = getInt();

    cout << "Enter day of birth (1-31): ";
    birthDay = getInt(1, 31);
    cout << "Enter month of birth (1-12): ";
    birthMonth = getInt(1, 12);
    cout << "Enter year of birth (1900-2022): ";
    birthYear = getInt(1900, 2022);

    cout << "What will your username be: ";
    username = getSpaced();

    while (true) {
        cout << "Please enter your password: ";
        cin >> password;
        cout << "Enter it again: ";
        cin >> rePassword;
        if (password != rePassword) cout << "The passwords do no match. Try again\n";
        else break;
    }

    string hashedPassword = username + password;
    SHA256 sha;
    sha.update(hashedPassword);
    uint8_t* digest = sha.digest();
    password = SHA256::toString(digest);

    // Load parent account file
    ofstream teacherAccounts;
    teacherAccounts.open("teacherAccounts.csv", std::ios::app); // Appends new parent to end of file
    if (!teacherAccounts.is_open()) {
        cout << "\nNo teacher account file found, ending registration attempt!\n";
        return;
    }
    // Write new parent info to file
    teacherAccounts << username << ","
        << password << ","
        << name << ","
        << email << ","
        << gender - 1 << ","
        << birthDay << ","
        << birthMonth << ","
        << birthYear << ","
        << phoneNumber << ","
        << teachClassNum << ","
        << teachYLvl;
    // Close parent file
    teacherAccounts << endl;
    teacherAccounts.close();
    loadSchool();
    return;
}

// Parent controls menu
void parentMenu(string name, string hashedPass) {
    system("cls");
    // String arrays used to convert integers into their string counterparts
    string genderOptions[3] = { "Male", "Female", "Non-Binary" };
    string gradeOptions[4] = { "Not Achieved", "Achieved", "Merit", "Excellence" };

    cout << "Welcome back " << name << "!";
    // Load all parent account information from file
    while (true) {
        vector<vector<string>> loadedParentAccounts;
        string parentUsername, parentName, fetchedParent, fetchedHash, password, rePassword;
        string username, email, gender, birthDay, birthMonth, birthYear, phoneNumber;
        vector<vector<string>> childrenInfo;
        // File stream declarations
        ifstream fileIn;
        fileIn.open("parentAccounts.csv");
        if (!fileIn.is_open()) {
            cout << "\nNo parent accounts exist!\n";
            return;
        }
        // Finds parent account
        while (std::getline(fileIn, fetchedParent)) {
            // Seperates csv row for parent into seperate values
            fetchedHash = "", username = "", email = "", gender = "", birthDay = "";
            birthMonth = "", birthYear = "", phoneNumber = "";
            childrenInfo = {};
            int dataPos = 0; // dataPos used to determine which piece of info below loop is writing to.
            for (char& c : fetchedParent) {
                if (c == ',') {
                    dataPos++;
                    if (dataPos >= 10) childrenInfo[childrenInfo.size() - 1].push_back("");
                    continue;
                }
                switch (dataPos) {
                case 0: // Username
                    username += c;
                    break;
                case 1: // Password (not needed)
                    continue;
                case 2: // Parent full name
                    fetchedHash += c;
                    break;
                case 3: // Email
                    email += c;
                    break;
                case 4: // Gender (0:Male, 1:Female, 2:Non-Binary)
                    gender += c;
                    break;
                case 5: // Day of birth
                    birthDay += c;
                    break;
                case 6: //     Month
                    birthMonth += c;
                    break;
                case 7: //     Year
                    birthYear += c;
                    break;
                case 8: // Phone number
                    phoneNumber += c;
                    break;
                default: // Children info
                    if (c == '[') { // Start of new child info group
                        if (childrenInfo.size() > 0) {
                            childrenInfo[childrenInfo.size() - 1].pop_back();
                        }
                        childrenInfo.push_back({ "" });
                    }
                    else childrenInfo[childrenInfo.size() - 1][childrenInfo[childrenInfo.size() - 1].size() - 1] += c;
                }
            }
            // If parent matches input, displays all found info on parent 
            if (fetchedHash == parentName) break;
        }
        fileIn.close();
        ofstream fileOut;
        SHA256 sha;
        uint8_t* digest;
        // Parent option selection
        int parentOption;
        cout << "\n1. View children information\n" <<
            "2. Edit account\n" <<
            "3. View account information\n" <<
            "4. Logout\n: ";
        parentOption = getInt(1, 4);
        switch (parentOption) {
        // View child info
        case 1:
            int childrenFound;
            childrenFound = 0;
            // Fetch all children related to parent
            for (int classIdx = 0; classIdx < classroomRecords.size(); classIdx++) {
                for (int studentIdx = 0; studentIdx < classroomRecords[classIdx].students.size(); studentIdx++) {
                    for (int childIdx = 0; childIdx < childrenInfo.size(); childIdx++) {
                        if (std::find(childrenInfo[childIdx].begin(), childrenInfo[childIdx].end(), classroomRecords[classIdx].students[studentIdx].name) != childrenInfo[childIdx].end()) {
                            childrenFound++;
                            // Displays found student info
                            vector<string> caregiverInfosDups; // Initial version, which may contain repeated parent infos
                            vector<string> caregiverInfos; // Cleaned version, with only one instance of each parent
                            // General info
                            cout << "\n" << classroomRecords[classIdx].students[studentIdx].name << " " <<
                                genderOptions[classroomRecords[classIdx].students[studentIdx].gender] <<
                                " - Classroom No." << classroomRecords[classIdx].classNumber << " Teacher: " <<
                                getTeacher(classroomRecords[classIdx].classNumber) <<
                                "\nGrades:";
                            // Subject grades
                            for (int subjectIdx = 0; subjectIdx < classroomRecords[classIdx].students[studentIdx].subjectGrades.size(); subjectIdx++) {
                                cout << "\n\t" <<
                                    subjectOrder[classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx][0]] <<
                                    ": " << gradeOptions[classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx][1]] << 
                                    " - Term No." << classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx][2];
                            }
                        }
                    }
                }
            }
            if (childrenFound == 0) cout << "\nNo children found!\n";
            break;
        // Edit account
        case 2:
            fileIn.open("parentAccounts.csv");
            if (!fileIn.is_open()) {
                cout << "\nNo parent accounts exist!\n";
                continue;
            }
            // Gets all parent accounts
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
            // Parent manipulation options
            int parentUpdateOption, parentIdx;
            parentIdx = -1;
            cout << "1. Change username & password\n" <<
                "2. Change name\n" <<
                "3. Update email\n" <<
                "4. Update number\n" <<
                "5. Change gender\n" <<
                "6. Cancel\n" <<
                ": ";
            parentUpdateOption = getInt(1, 6);
            // Get account index of parent
            for (int parentMatchIdx = 0; parentMatchIdx < loadedParentAccounts.size(); parentMatchIdx++) {
                if (loadedParentAccounts[parentMatchIdx][1] == hashedPass) parentIdx = parentMatchIdx;
            }
            if (parentIdx == -1) {
                cout << "Alert! Parent not found.";
                break;
            }
            switch (parentUpdateOption) {
            case 1: // Change username & password (Must be changed together, since password is salted with username)
                cout << "Enter new username: ";
                loadedParentAccounts[parentIdx][0] = getSpaced();
                while (true) {
                    cout << "Enter new password: ";
                    cin >> password;
                    cout << "Repeat password: ";
                    cin >> rePassword;
                    if (password == rePassword) break;
                    else cout << "Error! Passwords don't match.\n";
                }
                // Creates hashed password
                sha.update(loadedParentAccounts[parentIdx][0] + password);
                digest = sha.digest();
                loadedParentAccounts[parentIdx][1] = SHA256::toString(digest);

                cout << "Username and password have been updated successfully.";
                break;
            case 2: // Change name
                cout << "Enter new name: ";
                loadedParentAccounts[parentIdx][2] = getSpaced();
                cout << "Name change complete.";
                break;
            case 3: //     email
                cout << "Enter new email: ";
                loadedParentAccounts[parentIdx][3] = getSpaced();
                cout << "Email change complete.";
                break;
            case 4: //     number
                cout << "Enter new number: ";
                loadedParentAccounts[parentIdx][8] = getSpaced();
                cout << "Phone number change complete.";
                break;
            case 5: //     gender
                cout << "Enter new gender (1:Male, 2:Female, 3:Non-Binary): ";
                loadedParentAccounts[parentIdx][4] = std::to_string(getInt(1, 3) - 1);
                cout << "Gender updated.";
                break;
            case 6:
                continue;
            }
            fileIn.close();
            // Saves account changes
            fileOut.open("parentAccounts.csv");
            // Re-write parentAccount file
            for (int parentAccIdx = 0; parentAccIdx < loadedParentAccounts.size(); parentAccIdx++) {
                fileOut << loadedParentAccounts[parentAccIdx][0];
                for (int parentItemIdx = 1; parentItemIdx < loadedParentAccounts[parentAccIdx].size(); parentItemIdx++) {
                    fileOut << "," << loadedParentAccounts[parentAccIdx][parentItemIdx];
                }
                fileOut << endl;
            }
            fileOut.close();
            break;
        // View account information
        case 3:
            // General
            cout << "Stored information for " << name <<
                ":\n\tUsername: " << username <<
                "\n\tEmail: " << email <<
                "\n\tGender: " << genderOptions[std::stoi(gender)] <<
                "\n\tD.O.B: " << birthDay << "/" << birthMonth << "/" << birthYear <<
                "\n\tPhone Number: " << phoneNumber <<
                "\nChildren:";
            // Children
            for (int childIdx = 0; childIdx < childrenInfo.size(); childIdx++) {
                cout << "\n\t" << childrenInfo[childIdx][0] <<
                    " - Classroom No. " << childrenInfo[childIdx][1] << " Teacher: " <<
                    getTeacher(childrenInfo[childIdx][1]) <<
                    "\n\tCaregiver Contact Info" << (childrenInfo[childIdx].size() > 4 ? "s:" : ":");
                // Caregivers assigned to child
                for (int caregiverIdx = 2; caregiverIdx < childrenInfo[childIdx].size(); caregiverIdx++) {
                    if (caregiverIdx % 2 == 1) continue;
                    cout << "\n\t\t" << childrenInfo[childIdx][caregiverIdx] << ": " <<
                        childrenInfo[childIdx][caregiverIdx + 1] << childrenInfo[childIdx].size();
                }
            }
            break;
        // Logout
        case 4:
            return;
        }
    }
    loadSchool();
    return;
}

// Creates a student record (Admin)
void recordStudent() {
    if (classroomRecords.size() == 0) { // Checks there are classrooms loaded
        cout << "Alert! No classes exist, cannot add students!";
        return;
    }
    while (true) { // Allows for several students to be added
        int gender, recordClass, subjectInput, gradeInput, termInput;
        bool loopAgain = true, gradesLoop = true, foundClass = false;
        string name;
        vector<vector<int>> studentSubjects; // Grade storage vector
        vector<int> newSubject; // This allows us to store the subject, grade, and term under one variable

        // General student information
        cout << "\nWhat is the students full name: ";
        name = getSpaced();

        cout << "What is the students gender (1. Male, 2. Female, 3. Other): ";
        gender = getInt(1, 3) - 1;

        cout << "Please enter class number of student: ";
        while (!foundClass) { // Makes sure class exists
            recordClass = getInt(); // getInt receives input and checks if it is within the range listed 
            for (int classroomIdx = 0; classroomIdx < classroomRecords.size(); classroomIdx++) {
                if (classroomRecords[classroomIdx].classNumber == recordClass) {
                    recordClass = classroomIdx;
                    foundClass = true;
                    break;
                }
            }
            if (foundClass) break;
            cout << "Alert: Class doesn't exist, please try again: ";
        }
        
        // Student grade information
        cout << "Do you wish to add any subject grades to student (y/n): ";
        if (getChar("yn") == 'n') gradesLoop = false;
        while (gradesLoop) {
            // Get target subject
            cout << "\nWhat subject do you want to add?";

            for (int subjectIdx = 0; subjectIdx < subjectOrder.size(); subjectIdx++) {
                cout << "\n" << subjectIdx + 1 << ". " << subjectOrder[subjectIdx];
            }
            cout << "\n: ";
            subjectInput = getInt(1, 9) - 1;

            cout << "Please enter their grade (1: Not Achieved, 2: Achieved, 3: Merit, 4: Excellence): ";
            gradeInput = getInt(1, 4) - 1;

            cout << "Enter term grade was obtained in (1-4): ";
            termInput = getInt(1, 4);
            // Checks subject & term hasn't already been added
            int preexistingIdx = -1;
            for (int subjectIdx = 0; subjectIdx < studentSubjects.size(); subjectIdx++) {
                if (studentSubjects[subjectIdx][0] == subjectInput && studentSubjects[subjectIdx][2] == termInput) {
                    preexistingIdx = subjectIdx;
                }
            }

            if (preexistingIdx != -1) studentSubjects[preexistingIdx][1] = gradeInput;
            else studentSubjects.push_back({ subjectInput, gradeInput, termInput });

            cout << "\n\nWould you like to add more grades (y/n): ";
            if (getChar("yn") == 'n') break;
        }
        // Adds student to class
        classroomRecords[recordClass].students.push_back(student(name, gender, studentSubjects));

        cout << "\n\nDo you want to create another student record (y/n): ";
        if (getChar("yn") == 'n') break;
    }
}

// Edit a student record
void editStudentRecord() {
    int maxSize = 0;
    string name;
    bool editRecordLoop = true;
    // Gets name of student to edit
    cout << "\nPlease enter the student's name: ";
    name = getSpaced();

    bool studentFound = false;
    // Finds student index
    for (int classIdx = 0; classIdx < classroomRecords.size() && !studentFound; classIdx++) {
        for (int studentIdx = 0; studentIdx < classroomRecords[classIdx].students.size() && !studentFound; studentIdx++) {
            if (classroomRecords[classIdx].students[studentIdx].name == name) {
                while (editRecordLoop) { // Changes student info
                    cout << "What would you like to edit about " << name << "'s record?\n";
                    cout << "1. Name\n2. Gender\n3. Grades\n4. Delete Record\n5. Cancel\n: ";

                    switch (getInt(1, 5)) {
                    case 1: // Name
                        cout << "Please enter their new full name: ";
                        name = getSpaced();
                        classroomRecords[classIdx].students[studentIdx].name = name;
                        break;
                    case 2: // Gender
                        cout << "What is " << name << "'s gender (1: Male, 2: Female, 3: Non-Binary): ";
                        classroomRecords[classIdx].students[studentIdx].gender = getInt(1, 3) - 1;
                    case 3: // Grade adding, deletion and editing
                        while (true) {
                            int subjectInput, gradeInput, termInput, gradeEditOption;

                            cout << "1. Add/edit grade\n2. Delete grade\n: ";
                            gradeEditOption = getInt(1, 2);
                            // Gets subject
                            bool foundSubject;
                            foundSubject = false;
                            cout << "\nPlease choose a subject from below: ";
                            for (int subjectIdx = 0; subjectIdx < subjectOrder.size(); subjectIdx++) {
                                cout << "\n\t" << subjectIdx + 1 << ". " << subjectOrder[subjectIdx];
                            }
                            cout << "\n: ";
                            subjectInput = getInt(1, 9) - 1;
                            // Gets term
                            cout << "Please enter a term (1-4): ";
                            termInput = getInt(1, 4);

                            switch (gradeEditOption) {
                            case 1: // Adding/editing grade
                                cout << "Please enter a grade from below: " <<
                                    "\n1. Not Achieved\n2. Achieved\n3. Merit\n4. Excellence\n: ";
                                gradeInput = getInt(1, 4) - 1;
                                // Applying changes if editing
                                for (int i = 0; i < classroomRecords[classIdx].students[studentIdx].subjectGrades.size(); i++) {
                                    if (classroomRecords[classIdx].students[studentIdx].subjectGrades[i][0] == subjectInput &&
                                        classroomRecords[classIdx].students[studentIdx].subjectGrades[i][2] == termInput) {
                                        classroomRecords[classIdx].students[studentIdx].subjectGrades[i][1] = gradeInput; // Replace grade for subject and term
                                        foundSubject = true;
                                        break;
                                    }
                                }
                                // Applying changes if adding
                                if (!foundSubject) {
                                    classroomRecords[classIdx].students[studentIdx].subjectGrades.push_back({ subjectInput, gradeInput, termInput });
                                    cout << "Added grade!";
                                } else cout << "Updated grade!";
                                break;
                            case 2: // Delete grade
                                for (int i = 0; i < classroomRecords[classIdx].students[studentIdx].subjectGrades.size(); i++) {
                                    if (classroomRecords[classIdx].students[studentIdx].subjectGrades[i][0] == subjectInput &&
                                        classroomRecords[classIdx].students[studentIdx].subjectGrades[i][2] == termInput) {
                                        classroomRecords[classIdx].students[studentIdx].subjectGrades.erase(
                                            classroomRecords[classIdx].students[studentIdx].subjectGrades.begin() + i
                                        );
                                        foundSubject = true;
                                        break;
                                    }
                                }

                                if (!foundSubject) cout << "Unable to find grade!";
                                else cout << "Deleted grade!";
                                break;
                            }

                            cout << "\n\nWould you like to edit more grades (y/n): ";
                            if (getChar("yn") == 'n') break;
                        }
                        break;
                    case 4: // Delete student
                        cout << "\n\n\t!WARNING!\nOnce the student record has been deleted it cannot be recovered.\nDo you wish to continue (y/n): ";
                        if (getChar("yn") == 'y') {
                            cout << "\n" << name << "'s record has been deleted.";
                            classroomRecords[classIdx].students.erase(classroomRecords[classIdx].students.begin() + classIdx); // Deletes the vector data matching the inputted variables
                        }
                        break;
                    case 5: // Cancels editing
                        editRecordLoop = false;
                        break;
                    }

                    cout << "\n\nWould you like to continue editting " << name << "'s record (y/n): ";
                    if (getChar("yn") == 'n') break;
                }
                studentFound = true;
            }
        }
    }
    if (!studentFound) cout << "Unable to find student!";
    cout << "\nWould you like to edit another students record (y/n): ";
    if (getChar("yn") == 'y') editStudentRecord();
    return;
}

// View Record
void viewRecord() {
    int maxI = 0, recordClass, studentRecordNumber;
    string viewRecordName, viewRecordClass, studentGradeOptions[4] = { "Not Achieved", "Achieved", "Merit", "Excellence" };

    while (true) { // Allows for viewing of multiple students
        cout << "\nWhat class is your student in?";
        // Selects class
        for (int i = 0; i < classroomRecords.size(); i++) {
            cout << "\n Class No." << classroomRecords[i].classNumber << " - " << getTeacher(classroomRecords[i].classNumber);
            maxI = i + 1;
        }
        cout << "\n: ";
        recordClass = getInt(1, maxI) - 1;
        // Selects student in class
        cout << "\nWhich student do you wish to view?";
        for (int i = 0; i < classroomRecords[recordClass].students.size(); i++) {
            cout << "\n" << i + 1 << ". " << classroomRecords[recordClass].students[i].name;
            maxI = i + 1;
        }
        cout << "\n: ";
        studentRecordNumber = getInt(1, maxI) - 1;
        viewRecordName = classroomRecords[recordClass].students[studentRecordNumber].name;
        // Displays student general information
        cout << "\n\nCurrently viewing " << viewRecordName << "'s Record:";
        cout << "\nName: " << viewRecordName << "\nGender: ";
        switch (classroomRecords[recordClass].students[studentRecordNumber].gender) {
        case 0:
            cout << "Male";
            break;
        case 1:
            cout << "Female";
            break;
        case 2:
            cout << "Non-binary";
            break;
        }
        // Displays student grades
        for (int i = 0; i < classroomRecords[recordClass].students[studentRecordNumber].subjectGrades.size(); i++) {
            vector<int> subjectGrade = classroomRecords[recordClass].students[studentRecordNumber].subjectGrades[i];
            cout << "\n" << subjectOrder[subjectGrade[0]] << ": " << studentGradeOptions[subjectGrade[1]] <<
                " - Term No." << subjectGrade[2];
        }
        cout << "\nWould you like to view another record (y/n): ";
        if (getChar("yn") == 'n') break;
    }
}

// Teacher controls menu
void teacherMenu(string name) {
    system("cls");
    cout << "Welcome back " << name << "!";
    while (true) {
        cout << "\n1. Add student\n2. Edit student\n3. View student\n4. Logout\n: ";
        switch (getInt(1, 4)) {
        case 1: // Add
            recordStudent();
            saveSchool();
            break;
        case 2: // Edit
            editStudentRecord();
            saveSchool();
            break;
        case 3: // View
            viewRecord();
            break;
        case 4: // Logout
            return;
        }
    }
}

// Admin controls menu
void adminMenu(string username) {
    system("cls");
    cout << "Welcome " << username << "!\n";
    // Admin menu loop
    while (true) {
        // Some vectors, strings etc must be initialized here, since they can't be initialized inside of switch statement
        vector<int> classroomNums = {};
        vector<vector<string>> loadedParentAccounts;
        vector<string>* termPtrArr[4] = { &term1Dates, &term2Dates, &term3Dates, &term4Dates };
        vector<string>* termPtr;
        string parentUsername, parentName, fetchedParent, fetchedName, password, 
            rePassword, studentName, eventName, eventDescription, dateDescription;
        // File stream declarations
        ifstream fileIn;
        ofstream fileOut;
        SHA256 sha;
        uint8_t* digest;
        // String arrays used to convert integers into their string counterparts
        string genderOptions[3] = { "Male", "Female", "Non-Binary" };
        string gradeOptions[4] = { "Not Achieved", "Achieved", "Merit", "Excellence" };
        // Control options
        int adminOption;
        cout << "\n1. View class record/s\n" <<
            "2. Update, add, and delete classes\n" <<
            "3. View parent record/s\n" <<
            "4. Update parent record\n" <<
            "5. View student record/s\n" <<
            "6. Update student record\n" <<
            "7. Update events\n" << 
            "8. Update term dates\n" <<
            "9. Update school info\n" <<
            "10. Logout\n" <<
            ": ";
        adminOption = getInt(1, 10);
        // Individual option code
        switch (adminOption) {
        // View classroom records
        case 1:
            if (classroomRecords.size() == 0) {
                cout << "Alert! No classes exist!";
                continue;
            }
            int classOption;
            cout << "Please enter the class number, or type \"0\" for a list of all class numbers: ";
            classOption = getInt();
            // Display all class numbers
            if (classOption == 0) {
                cout << "Classes: " << classroomRecords[0].classNumber;
                for (int classroomIdx = 1; classroomIdx < classroomRecords.size(); classroomIdx++) {
                    cout << ", " << classroomRecords[classroomIdx].classNumber;
                }
                cout << "\n";
            } // Display all students in class
            else {
                for (int classroomIdx = 0; classroomIdx < classroomRecords.size(); classroomIdx++) {
                    if (classroomRecords[classroomIdx].classNumber == classOption) {
                        int rollCount = classroomRecords[classroomIdx].students.size();
                        string teacherName = getTeacher(classroomRecords[classroomIdx].classNumber);
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
                            // Display all subject grades for student
                            for (int subjectIdx = 0; subjectIdx < subjectGradeCount; subjectIdx++) {
                                vector<int> subjectGrade = classroomRecords[classroomIdx].students[studentIdx].subjectGrades[subjectIdx];
                                cout << "\t\t" << subjectOrder[subjectGrade[0]] << ": "
                                    << gradeOptions[subjectGrade[1]] << 
                                    " Term No." << subjectGrade[2] << endl;
                            }
                        }
                    }
                }
            }
            break;
        // Update class record
        case 2:
            // Variable setup
            int classUpdateOption, classNum;
            for (int classroomIdx = 0; classroomIdx < classroomRecords.size(); classroomIdx++) {
                classroomNums.push_back(classroomRecords[classroomIdx].classNumber);
            }
            // Class manipulation options
            cout << "1. Update existing class\n" <<
                "2. Delete class\n" <<
                "3. Add class\n" <<
                ": ";
            classUpdateOption = getInt(1, 3);
            
            if (classroomRecords.size() == 0 && classUpdateOption != 3) {
				cout << "No classes exist, cancelling.";
                continue;
            }
            // All cases require a class number
            cout << "Enter class number: ";
            classNum = getInt();
            switch (classUpdateOption) {
            case 1: // Change info in a given class
                int classUpdateSelector;
                // Check class exists
                if (std::find(classroomNums.begin(), classroomNums.end(), classNum) == classroomNums.end()) {
                    cout << "Alert, class doesn't exist!";
                    break;
                }
                // User selects which bit of information in class they want to change
                cout << "1. Change class number\n" <<
                    "2. Remove all students\n" << // No individual student deletion needed, since admin focuses on large-scale manipulation, not specific
                    ": ";
                classUpdateSelector = getInt(1, 2);
                switch (classUpdateSelector) {
                case 1: // Change class number
                    int newClassNum;
                    cout << "Enter new class number: ";
                    newClassNum = getInt();
                    // Check number isn't already in use
                    if (std::find(classroomNums.begin(), classroomNums.end(), newClassNum) != classroomNums.end()) {
                        cout << "Number already in use, change denied.";
                        break;
                    }
                    // Update files with new class number
                    remove(("class" + std::to_string(classNum) + ".csv").c_str());
                    classroomRecords[std::find(classroomNums.begin(), classroomNums.end(), classNum) - classroomNums.begin()].classNumber = newClassNum;
                    saveSchool();
                    cout << "Change accepted: " << classNum << " -> " << newClassNum;
                    break;
                case 2: // Delete all students
                    // Confirmation needed, since this destroys a lot of data
                    char confirmChar;
                    cout << "Confirm wipe of classroom No." << classNum << " (y/n): ";
                    confirmChar = getChar("yn");
                    if (confirmChar == 'y') {
                        // Deletes all students in classroom vector & saves changes
                        classroomRecords[std::find(classroomNums.begin(), classroomNums.end(), classNum) - classroomNums.begin()].students = {};
                        saveSchool();
                        cout << "Wipe complete!";
                        break;
                    }
                    cout << "Wipe process terminated.";
                    break;
                }
                break;
            case 2: // Delete a class
                // Checks class exists
                if (std::find(classroomNums.begin(), classroomNums.end(), classNum) == classroomNums.end()) {
                    cout << "Alert, class doesn't exist!";
                    break;
                }
                // Confirmation needed, since this destroys an entire class
                char deleteConfirmationChar;
                cout << "Confirm deletion of class No." << classNum << " (y/n): ";
                deleteConfirmationChar = getChar("yn");
                if (deleteConfirmationChar == 'y') {
                    // Clears class from memory and files
                    remove(("class" + std::to_string(classNum) + ".csv").c_str());
                    classroomRecords.erase(classroomRecords.begin() + (std::find(classroomNums.begin(), classroomNums.end(), classNum) - classroomNums.begin()));
                    saveSchool();
                    cout << "Deletion of class No." << classNum << " complete.";
                    break;
                }
                cout << "Deletion terminated.";
                break;
            case 3: // Adds a new class - Populated later by assigned teacher/s
                classroomRecords.push_back(classroom(classNum, {}));
                saveSchool();
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
            // Gets parent name
            cout << "Please enter parent name, or type \"ALL\" to view all parent names: ";
            parentName = getSpaced();
            // ALL option allows admin to view all parents that have accounts
            if (parentName == "ALL") {
                cout << "Parents:";
                string fetchedParent;
                // Gets all parent names in parentAccounts file
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
            } // Displays all information assoc. with a given parent
            else {
                // Finds parent account
                while (std::getline(fileIn, fetchedParent)) {
                    // Seperates csv row for parent into seperate values
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
                        case 0: // Username
                            username += c;
                            break;
                        case 1: // Password (not needed)
                            continue;
                        case 2: // Parent full name
                            fetchedName += c;
                            break;
                        case 3: // Email
                            email += c;
                            break;
                        case 4: // Gender (0:Male, 1:Female, 2:Non-Binary)
                            gender += c;
                            break;
                        case 5: // Day of birth
                            birthDay += c;
                            break;
                        case 6: //     Month
                            birthMonth += c;
                            break;
                        case 7: //     Year
                            birthYear += c;
                            break;
                        case 8: // Phone number
                            phoneNumber += c;
                            break;
                        default: // Children info
                            if (c == '[') { // Start of new child info group
                                if (childrenInfo.size() > 0) {
                                    childrenInfo[childrenInfo.size() - 1].pop_back();
                                }
                                childrenInfo.push_back({ "" });
                            }
                            else childrenInfo[childrenInfo.size() - 1][childrenInfo[childrenInfo.size() - 1].size() - 1] += c;
                        }
                    }
                    // If parent matches input, displays all found info on parent 
                    if (fetchedName == parentName) {
                        // General
                        cout << "Stored information for " << parentName <<
                            ":\n\tUsername: " << username <<
                            "\n\tEmail: " << email <<
                            "\n\tGender: " << genderOptions[std::stoi(gender)] <<
                            "\n\tD.O.B: " << birthDay << "/" << birthMonth << "/" << birthYear <<
                            "\n\tPhone Number: " << phoneNumber <<
                            "\nChildren:";
                        // Children
                        for (int childIdx = 0; childIdx < childrenInfo.size(); childIdx++) {
                            cout << "\n\t" << childrenInfo[childIdx][0] <<
                                " - Classroom No. " << childrenInfo[childIdx][1] << " Teacher: " << 
                                getTeacher(childrenInfo[childIdx][1]) <<
                                "\n\tCaregiver Contact Info" << (childrenInfo[childIdx].size() > 4 ? "s:" : ":");
                            // Caregivers assigned to child
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
            break;
        // Update parent record
        case 4:
            fileIn.open("parentAccounts.csv");
            if (!fileIn.is_open()) {
                cout << "\nNo parent accounts exist!\n";
                continue;
            }
            // Gets all parent accounts
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
            // Parent manipulation options
            int parentUpdateOption, parentIdx;
            parentIdx = -1;
            cout << "1. Delete parent record\n" <<
                "2. Change parent username & password\n" <<
                "3. Change parent name\n" <<
                "4. Update parent email\n" <<
                "5. Update parent number\n" <<
                "6. Change parent gender\n" <<
                "7. Re-assign children info\n" <<
                ": ";
            parentUpdateOption = getInt(1, 7);
            // Select parent based off their full name
            cout << "Enter name of parent: ";
            parentName = getSpaced();
            // Get account index of parent
            for (int parentMatchIdx = 0; parentMatchIdx < loadedParentAccounts.size(); parentMatchIdx++) {
                if (loadedParentAccounts[parentMatchIdx][2] == parentName) parentIdx = parentMatchIdx;
            }
            if (parentIdx == -1) {
                cout << "Alert! Parent not found.";
                break;
            }
            switch (parentUpdateOption) {
            case 1: // Delete parent account
                // Confirmation needed, since this deletes entire account
                char parentDeletionConfirmation;
                cout << "Confirm deletion of " << parentName << " (y/n): ";
                parentDeletionConfirmation = getChar("yn");
                if (parentDeletionConfirmation == 'y') {
                    loadedParentAccounts.erase(loadedParentAccounts.begin() + parentIdx);
                    cout << "Deletion complete.";
                    break;
                }
                cout << "Cancelled parent deletion.";
                break;
            case 2: // Change username & password (Must be changed together, since password is salted with username)
                cout << "Enter new username: ";
                loadedParentAccounts[parentIdx][0] = getSpaced();
                while (true) {
                    cout << "Enter new password: ";
                    cin >> password;
                    cout << "Repeat password: ";
                    cin >> rePassword;
                    if (password == rePassword) break;
                    else cout << "Error! Passwords don't match.\n";
                }
                // Creates hashed password
                sha.update(loadedParentAccounts[parentIdx][0] + password);
                digest = sha.digest();
                loadedParentAccounts[parentIdx][1] = SHA256::toString(digest);

                cout << "Username and password have been updated successfully.";
                break;
            case 3: // Change name
                cout << "Enter new name: ";
                loadedParentAccounts[parentIdx][2] = getSpaced();
                cout << "Name change complete.";
                break;
            case 4: //     email
                cout << "Enter new email: ";
                loadedParentAccounts[parentIdx][3] = getSpaced();
                cout << "Email change complete.";
                break;
            case 5: //     number
                cout << "Enter new number: ";
                loadedParentAccounts[parentIdx][8] = getSpaced();
                cout << "Phone number change complete.";
                break;
            case 6: //     gender
                cout << "Enter new gender (1:Male, 2:Female, 3:Non-Binary): ";
                loadedParentAccounts[parentIdx][4] = getInt(1, 3) - 1;
                cout << "Gender updated.";
                break;
            case 7: // Re-assign children info
                while (loadedParentAccounts[parentIdx].size() > 9) loadedParentAccounts[parentIdx].pop_back();
                char addChildCheck, addCaregiverCheck;
                do {
                    // Gets child name
                    cout << "Enter child name: ";
                    loadedParentAccounts[parentIdx].push_back("[" + getSpaced());
                    //     classroom
                    cout << "Enter child classroom number: ";
                    loadedParentAccounts[parentIdx].push_back(std::to_string(getInt()));
                    // Gets caregiver info for child
                    do {
                        // Name
                        cout << "Enter caregiver name: ";
                        loadedParentAccounts[parentIdx].push_back(getSpaced());
                        // Number
                        cout << "Enter caregiver emergency contact number: ";
                        loadedParentAccounts[parentIdx].push_back(getSpaced());
                        // Repeat confirmation
                        cout << "Add another caregiver (y/n)? ";
                        addCaregiverCheck = getChar("yn");
                    } while (addCaregiverCheck != 'n');
                    // Repeat confirmation
                    cout << "Add another child (y/n)? ";
                    addChildCheck = getChar("yn");
                } while (addChildCheck != 'n');
                cout << "Children updated.";
                break;
            }
            fileIn.close();
            // Saves account changes
            fileOut.open("parentAccounts.csv");
            // Re-write parentAccount file
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
            // View options
            int studentRecordViewOption;
            cout << "1. Show individual student\n" <<
                "2. Show students by grade\n" <<
                ": ";
            studentRecordViewOption = getInt(1, 2);

            switch (studentRecordViewOption) {
            case 1: // Show individual student
                cout << "Enter student name: ";
                studentName = getSpaced();
                // Needs to be seperated to avoid switch bug
                bool studentFound;
                studentFound = false;
                // Finds student index
                for (int classIdx = 0; classIdx < classroomRecords.size() && !studentFound; classIdx++) {
                    for (int studentIdx = 0; studentIdx < classroomRecords[classIdx].students.size() && !studentFound; studentIdx++) {
                        if (classroomRecords[classIdx].students[studentIdx].name == studentName) {
                            // Displays found student info
                            vector<string> caregiverInfosDups; // Initial version, which may contain repeated parent infos
                            vector<string> caregiverInfos; // Cleaned version, with only one instance of each parent
                            // General info
                            cout << genderOptions[classroomRecords[classIdx].students[studentIdx].gender] <<
                                " - Classroom No." << classroomRecords[classIdx].classNumber << " Teacher: " <<
                                getTeacher(classroomRecords[classIdx].classNumber) <<
                                "\nCurrent Term Grades:";
                            // Subject grades
                            for (int subjectIdx = 0; subjectIdx < classroomRecords[classIdx].students[studentIdx].subjectGrades.size(); subjectIdx++) {
                                cout << "\n\t" <<
                                    subjectOrder[classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx][0]] <<
                                    ": " << gradeOptions[classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx][1]] <<
                                    " - Term No." << classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx][2];
                            }
                            // Fetches caregiver info from file
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
                                    else if (studentRelated && c == '[') break; // Gets student info group
                                    else currentInfoChunk += c;
                                }
                                if (studentRelated && currentInfoChunk != "") {
                                    caregiverInfosDups.push_back(currentInfoChunk);
                                }
                            }
                            fileIn.close();
                            // Displays found caregiver info
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
            case 2: // Shows all students that meet a given criteria
                int subject, grade, startingClassNum, rollCounter, classLoopIdx, classIdx;
                rollCounter = 0;
                // Get target subject
                cout << "Enter subject:";
                for (int subjectIdx = 0; subjectIdx < subjectOrder.size(); subjectIdx++) {
                    cout << "\n" << subjectIdx + 1 << ". " << subjectOrder[subjectIdx];
                }
                cout << "\n: ";
                subject = getInt(1, subjectOrder.size());
                subject--;
                // Get target grade
                cout << "Enter grade:" <<
                    "\n1. Not Achieved" <<
                    "\n2. Achieved" <<
                    "\n3. Merit" <<
                    "\n4. Excellence" <<
                    "\n: ";
                grade = getInt(1, 4);
                grade--;
                // Get target class
                cout << "Enter class num (Or type \"0\" for all classes): ";
                startingClassNum = getInt();
                classLoopIdx = 0;
                // Display all students that meet subject, grade, and class criteria
                do { // Class loop
                    classIdx = classroomRecords[classLoopIdx].classNumber;
                    // Specific class filter
                    if (classIdx != startingClassNum && startingClassNum > 0) {
                        classLoopIdx++;
                        continue;
                    }
                    // Displays all matching students in class
                    for (int studentIdx = 0; studentIdx < classroomRecords[classLoopIdx].students.size(); studentIdx++) {
                        // Subject loop checks that student subject grades match criteria
                        for (int subjectIdx = 0; subjectIdx < classroomRecords[classLoopIdx].students[studentIdx].subjectGrades.size(); subjectIdx++) {
                            // Shortens grade reference
                            vector<int> subjectGrade = classroomRecords[classLoopIdx].students[studentIdx].subjectGrades[subjectIdx];
                            if (subjectGrade[0] == subject && subjectGrade[1] == grade) { // Subject-Grade criteria checker
                                rollCounter++;
                                studentName = classroomRecords[classLoopIdx].students[studentIdx].name;
                                // Display general student info
                                cout << classroomRecords[classLoopIdx].students[studentIdx].name << " (" <<
                                    genderOptions[classroomRecords[classLoopIdx].students[studentIdx].gender] <<
                                    ") - Class No." << classIdx << " Teacher: " << getTeacher(classroomRecords[classLoopIdx].classNumber);

                                vector<string> caregiverInfosDups; // Initial version, which may contain repeated parent infos
                                vector<string> caregiverInfos; // Cleaned version, with only one instance of each parent

                                // Fetches student caregiver info
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
                                            // Checks if student name matches requested student name
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
                                // Display student caregivers
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
                    classLoopIdx++;
                } while (classLoopIdx < classroomRecords.size());
                // Search ending message
                if (rollCounter > 0) cout << "\nTotal students found: " << rollCounter << endl;
                else cout << "\nNo students found under criteria!" << endl;
                break;
            }
            break;
        // Update student records
        case 6:
            editStudentRecord();
            break;
        // Update events
        case 7:
            int eventManipOption, eventGroup, targetEvent; // Group being either upcoming or recent
            // Get event manipulation option
            cout << "1. Add event\n" <<
                "2. Update event\n" << // Essentially creates a new event, but at a pre-existing event location
                "3. Remove event\n" <<
                "4. Shift upcoming event to recent event\n"
                "5. Cancel\n: ";
            eventManipOption = getInt(1, 5);
            if (eventManipOption == 5) continue; // Exit
            if (eventManipOption == 4) eventGroup = 1; // Shift event to recent
            else { // Get time group
                cout << "Enter time group (1: Upcoming, 2: Recent): ";
                eventGroup = getInt(1, 2);
            }

            if (eventManipOption != 1) { // When adding an event (Option 1), no need to select a pre-existing event
                if ((eventGroup == 1 ? upcomingEvents.size() : recentEvents.size()) == 0) {
                    cout << "No events in group exist, cancelling!";
                    continue;
                }
                cout << "Select event from below: ";
                // Displays all the events in time group
                for (int eventIdx = 0; eventIdx < (eventGroup == 1 ? upcomingEvents.size() : recentEvents.size()); eventIdx++) {
                    cout << "\n\t" << eventIdx + 1 << ". " <<
                        (eventGroup == 1 ? upcomingEvents[eventIdx][0] : recentEvents[eventIdx][0]);
                }
                cout << "\n: ";
                targetEvent = getInt(1, (eventGroup == 1 ? upcomingEvents.size() : recentEvents.size())) - 1;
            }

            if (eventManipOption == 1 || eventManipOption == 2) { // Gets new event information (For adding & editing events)
                cout << "Enter new event name: ";
                eventName = getSpaced();
                cout << "Enter event description below:\n";
                eventDescription = getSpaced();
            }
            // Applies changes to event vector
            switch (eventManipOption) {
            case 1: // Add
                if (eventGroup == 1) upcomingEvents.push_back({ eventName, eventDescription });
                else recentEvents.push_back({ eventName, eventDescription });
                break;
            case 2: // Update
                if (eventGroup == 1) upcomingEvents[targetEvent] = { eventName, eventDescription };
                else recentEvents[targetEvent] = {eventName, eventDescription};
                break;
            case 3: // Remove
                if (eventGroup == 1) upcomingEvents.erase(upcomingEvents.begin() + targetEvent);
                else recentEvents.erase(recentEvents.begin() + targetEvent);
                break;
            case 4: // Shift upcoming to recent
                recentEvents.push_back(upcomingEvents[targetEvent]);
                upcomingEvents.erase(upcomingEvents.begin() + targetEvent);
                break;
            }
            cout << "Update successful!\n";
            saveSchool();
            break;
        // Update dates
        case 8:
            int dateSelected, dateOption;
            // Target term
            cout << "Enter term (1, 2, 3, 4): ";
            termPtr = termPtrArr[getInt(1, 4)-1];
            // Term manipulation options
            cout << "1. Add date\n2. Edit date\n3. Remove date\n: ";
            dateOption = getInt(1, 3);
            if (dateOption != 1) { // Gets target date (Not needed for adding)
                if ((*termPtr).size() == 0) {
                    cout << "No dates exist in term, cancelling!";
                    continue;
                }
                cout << "Select date from below: ";
                for (int dateIdx = 0; dateIdx < (*termPtr).size(); dateIdx++)
                    cout << "\n\t" << dateIdx + 1 << ". " << (*termPtr)[dateIdx];
                cout << "\n: ";
                dateSelected = getInt(1, (*termPtr).size()) - 1;
            }
            if (dateOption != 3) { // Gets new date description (Not needed for removing)
                cout << "Enter new date description below:\n";
                dateDescription = getSpaced();
            }
            switch (dateOption) {
            case 1: // Add
                (*termPtr).push_back(dateDescription);
                break;
            case 2:// Update
                (*termPtr)[dateSelected] = dateDescription;
                break;
            case 3: // Remove
                (*termPtr).erase((*termPtr).begin() + dateSelected);
                break;
            }
            cout << "Update successful!\n";
            saveSchool();
            break;
        // Update school info
        case 9:
            cout << "1. Change name\n2. Change email\n3. Change phone number\n4. Cancel\n: ";
            switch (getInt(1, 5)) {
            case 1:
                cout << "Enter new school name: ";
                schoolName = getSpaced();
                cout << "Name change successful!";
                break;
            case 2:
                cout << "Enter new school email: ";
                schoolEmailAddress = getSpaced();
                cout << "Email change successful!";
                break;
            case 3:
                cout << "Enter new phone number: ";
                schoolPhoneNumber = getSpaced();
                cout << "Phone number change successful!";
                break;
            case 4:
                cout << "Change terminated!";
                continue;
            }
            saveSchool();
            break;
        // Return to main menu
        case 10:
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
        username = getSpaced();
        cout << "Please enter your password: ";
        cin >> password;
        // Forms username-password hash combo
        SHA256 sha;
        sha.update(username + password);
        uint8_t* digest = sha.digest();
        string hashedPass = SHA256::toString(digest);
        // Checks if hash combo is a valid account
        if (std::find(accounts.begin(), accounts.end(), hashedPass) != accounts.end()) {
            parentMenu(username, hashedPass);
            return;
        }
        if (attempt == 2) continue;
        cout << "\nUsername or password is incorrect.\nTry again (y/n)? "; // Allows for exit if user can't remember password
        attemptingSignin = getChar("yn");
    }
    cout << ((attemptingSignin == 'n') ?
        "\nUnsuccessful - Login attempt terminated.\n" :
        "\nUnsuccesful - Too many signin attempts. Please try again later.\n");
}

// Teacher signin
void teacherLogin()
{
    // Account String Declarations
    string username, password, fetchedInfo;
    // Stores all username-password hashes in teacherAccount DB
    vector<string> accounts;
    // Fetches teacherAccounts file for populating accounts Vector
    ifstream teacherAccounts;
    teacherAccounts.open("teacherAccounts.csv");
    if (!teacherAccounts.is_open()) {
        cout << "No teacher accounts exist!";
        return;
    }
    // Populates accounts Vector with all account hashes
    while (std::getline(teacherAccounts, fetchedInfo)) {
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
    teacherAccounts.close();
    // User signin input system
    char attemptingSignin = 'y';
    for (int attempt = 0; attempt < 3 && attemptingSignin != 'n'; attempt++) {
        cout << "Please enter your username: ";
        username = getSpaced();
        cout << "Please enter your password: ";
        cin >> password;
        // Forms username-password hash combo
        SHA256 sha;
        sha.update(username + password);
        uint8_t* digest = sha.digest();
        string hashedPass = SHA256::toString(digest);
        // Checks if hash combo is a valid account
        if (std::find(accounts.begin(), accounts.end(), hashedPass) != accounts.end()) {
            teacherMenu(username);
            return;
        }
        if (attempt == 2) continue;
        cout << "\nUsername or password is incorrect.\nTry again (y/n)? "; // Allows for exit if user can't remember password
        attemptingSignin = getChar("yn");
    }
    cout << ((attemptingSignin == 'n') ?
        "\nUnsuccessful - Login attempt terminated.\n" :
        "\nUnsuccesful - Too many signin attempts. Please try again later.\n");
}

// Admin signin
void adminLogin()
{
    string username, password;
    bool foundMatch = false;
    // Admin signin input system
    char attempting = 'y';
    for (int attempt = 0; attempt < 3 && !foundMatch && attempting != 'n'; attempt++) {
        cout << "Please enter your username (Case-sensitive): ";
        username = getSpaced();
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
        attempting = getChar("yn");
    }
    cout << ((attempting == 'n') ?
        "\nUnsuccessful - Login attempt terminated.\n" :
        "\nUnsuccesful - Too many signin attempts. Please try again later.\n");
}

// School contact info viewing
void contactInfo()
{
    cout << "+------------------------------+\n"
         << "Mebee School Contact Information\n"
         << "+------------------------------+\n"
         << schoolName << "\n"
         << schoolAddress << "\n"
         << schoolPhoneNumber << "\n"
         << schoolEmailAddress << endl;
}

// Event info viewing
void functionsEvents()
{
    cout << "\nMebee School Recent and Upcoming Events\n\n"
         << "Please choose one of the options below:\n"
         << "1. Recent Functions & Events\n2. Upcoming Functions & Events\n: ";

    if (getInt(1, 2) == 1) {
        cout << "\n+-------------------------+\n"
            << "Recent Mebee School Events\n"
            << "+-------------------------+\n";
        //output recentEvents vector
        for (int i = 0; i < recentEvents.size(); i++) {
            cout << recentEvents[i][0] << " - " << recentEvents[i][1] << endl;
        }
    } else {
        cout << "\n+---------------------------+\n"
             << "Upcoming Mebee School Events\n"
             << "+---------------------------+\n";
        //output upcomingEvents vector
        for (int i = 0; i < upcomingEvents.size(); i++) {
            cout << upcomingEvents[i][0] << " - " << upcomingEvents[i][1] << endl;
        }
    }
}

// Date info viewing
void importantDates()
{
    cout << "Choose an option to view\n"
         << "1. Term 1\n2. Term 2\n3. Term 3\n4. Term 4\n: ";

    switch (getInt(1, 4))
    {
    case 1:
        cout << "+---------------------------+\n"
             << "           Term One          \n"
             << "+---------------------------+\n";
        //output term 1 vector
        for (int i = 0; i < term1Dates.size(); i++) cout << term1Dates[i] << endl;
        break;
    case 2:
        cout << "+---------------------------+\n"
             << "           Term Two          \n"
             << "+---------------------------+\n";
        //output term 2 vector
        for (int i = 0; i < term2Dates.size(); i++) cout << term2Dates[i] << endl;
        break;
    case 3:
        cout << "+---------------------------+\n"
             << "          Term Three         \n"
             << "+---------------------------+\n";
        //output term 3 vector
        for (int i = 0; i < term3Dates.size(); i++) cout << term3Dates[i] << endl;
        break;
    case 4:
        cout << "+---------------------------+\n"
             << "           Term Four         \n"
             << "+---------------------------+\n";
        //output term 4 vector
        for (int i = 0; i < term4Dates.size(); i++) cout << term4Dates[i] << endl;
        break;
    }
}

// Main function (Executed on load)
int main()
{
    loadSchool();
    int startPageInput;
    int teacherInput;
    int parentInput;
    bool mainMenuActive = true;

    while (mainMenuActive == true)
    {
        // Introduction menu
        cout << "\nWelcome to " << schoolName << "\n\n"
            << "Please choose an option from the menu\n"
            << "1. School Functions & Events\n"
            << "2. Term Dates\n"
            << "3. Parent Register & Login\n"
            << "4. Teacher Register & Login\n"
            << "5. Administrator Login\n"
            << "6. Contact Us\n"
            << "7. Exit\n"
            << ": ";
        startPageInput = getInt(1, 7);

        switch (startPageInput)
        {
        case 1: // View events
            functionsEvents();
            break;
        case 2: // View dates
            importantDates();
            break;
        case 3: // Parent signin/up
            cout << "Would you like to register or login?\n";
            cout << "1. Register\n2. Login\n: ";
            parentInput = getInt(1, 2);
            switch (parentInput)
            {
            case 1: // Signup
                parentRegister();
                break;
            case 2: // Signin
                parentLogin();
                break;
            }
            break;
        case 4: // Teacher signin/up
            cout << "Would you like to register or login?\n";
            cout << "1. Register\n2. Login\n: ";
            teacherInput = getInt(1, 2);
            switch (teacherInput)
            {
            case 1: // Signup
                teacherRegister();
                break;
            case 2: // Signin
                teacherLogin();
                break;
            }
            break;
        case 5: // Admin login
            adminLogin();
            break;
        case 6: // View contact information
            contactInfo();
            break;
        case 7: // Exit
            cout << "You have chosen to exit the program, goodbye!\n";
            return 0;
        }
    }
}