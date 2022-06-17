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

// Will be sorted by date
vector<vector<string>> recentEvents = {
    // Name (For menu) - Description (For when viewing full article)
    {"Mebee School Dance\nMonday May 2nd\n" "On the 2nd of May Mebee celebrated its 1500th anniversary with a evening to remember, including a student band performance and a meet-in-greet with local enviromentalist Morgoth Bauglir!\n"},
    {"Mebee School Fair\nSaturday May 14th\n" "This year, our school fundraised over $15,520,001 to help support our Leaders in Yachts program. Expect even bigger things next year!\n"},
    {"Y3 Hiking Trip\nFriday May 26th\n" "Congratulations to the resilient class 431 on their recent surprise hiking trip up Mt Cook. Make sure to rest up over the coming holiday break, we've got even more exciting adventures planned next term!\n"},
    {"Regional Soccer Winner\nSaturday June 12th\n" "Congrats to our Weka Soccer team, who flew into first place in the Regional Soccer Tournament, narrowly beating the Eagles 7-5!\n"}
};

vector<vector<string>> upcomingEvents = {
    // Name (For menu) - Description (For when viewing full article)
    {"Parent Teacher Interviews\nFriday July 1st\n" "Parent teacher interviews are coming up on the 7th of July, so make sure to book yours before it's too late!\n"},
    {"Lost and found to be cleared on\nFriday July 1st\n" "To anyone missing their life-sized pink whale plushy, please collect it from the hall by friday, any remaining afterhours will be donated to our local second-hand shop.\n"},
    {"Annual Mebee Waterfight\nThursday July 8th\n" "Get ready for our annual waterfight on the 2nd of August. IMPORTANT: Autonomous drones are now banned, after the duck tragedy last year.\n"}
};

vector<string> term1Dates = {
    "Term 1: 3rd February - 14th April\n",
    "Tuesday 1st February       Teacher only day",
    "Wednesday 2nd February     Teacher only day",
    "Thursday 3rd February      Learning conferences",
    "Friday 4th February        Learning conferences",
    "Monday 7th February        Waitangi Day observed",
    "Tuesday 8th February       Term 1 classes start",
    "Friday 15th April  	    Good Friday"
};


vector<string> term2Dates = {
    "Term 2: 2nd May - 8th July 2022\n",
    "Monday 2nd May 	           Term 2 classes start",
    "Friday 3rd June            Teacher only day",
    "Friday 8th July            Teacher only day",
    "Public Holidays",
    "Monday 6th June  	   Queens Birthday",
    "Friday 24th  June          Matariki",
    "Term 2 Holidays            Saturday 9th July - Sunday 24th July 2022\n"
};

vector<string> term3Dates = {
  "Term 3 25th July - 30th September 2022",
  "Monday 25th July           Term 3 classes start",
  "Friday 26th August         Teacher only day",
  "No Public holidays",
  "Term 3 Holidays            Saturday 1st October - Sunday 16th October 2022\n",
};

vector<string> term4Dates = {
  "Term 4 17th October - 16th December 2022\n",
  "Monday 17th October        Term 4 classes start",
  "Friday 21st October        Teacher only",
  "Public Holidays :\n",
  "Monday 24th October        Labour Day",
  "Term 4 Christmas school holidays start Saturday 17th December 2022\n",
};

string schoolName = "Mebee School";
string schoolAddress = "Level 6, Wellington Railway Station\nWellington\nNew Zealand";
string schoolPhoneNumber = "013 024 0013";
string schoolEmailAddress = "helpdesk@mebee.eyes.co.nz";

// returns a user-inputted int that matches a given range
int getInt(int lowerLimit = 0, int upperLimit = 100000) {
    string input;
    cin >> input;
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

// returns a user-inputted char
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

// returns a string with spaces
string getSpaced() {
    string input;
    if (cin.peek() != '\n') cin.putback('\n'); // Stops first char from being omitted during cin
    cin.ignore();
    std::getline(cin, input);
    return input;
}

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
    name = getSpaced();
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
    string teachName;
    char teachGen;
    int teachDoB;
    string teachEmail;
    int teachPho;
    int teachClassNum;
    int teachYLvl;
    string teachUn;
    string Passw;
    string Passw2;
    string teachPassw;
    bool looping = true;

    system("cls");
    cout << "\n\nWhat is your First name: ";
    cin.ignore();
    getline(cin, teachName);

    while (looping == true) {
        cout << "\n\nWhat is your gender (m = Mr./f = Mrs.): ";
        cin >> teachGen;
        if (teachGen != 'm' || teachGen != 'f')
            cout << "That is not a valid option: \n\n";
        else
            looping = false;
    }

    cout << "\n\nWhat is you phone number: ";
    cin >> teachPho;

    cout << "\n\nWhat is your classroom number: ";
    cin >> teachClassNum;

    cout << "\n\nWhat year level do you teach: ";
    cin >> teachYLvl;

    cout << "\n\nWhat will your username be: ";
    cin >> teachUn;

    looping = true;
    while (looping == true) {
        cout << "\n\nPlease enter your password: ";
        cin >> Passw;
        cout << "\n\nEnter it again: ";
        cin >> Passw2;
        if (Passw != Passw2)
            cout << "\n\nThe passwords do no match. Try again";
        else {
            teachPassw = Passw;
            looping = false;
        }
    }
}

// Teacher controls menu
void teacherMenu(string name) {
    return;
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
        string parentUsername, parentName, fetchedParent, fetchedName, password, rePassword, studentName;
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
            "2. Update class\n" <<
            "3. View parent record/s\n" <<
            "4. Update parent record\n" <<
            "5. View student record/s\n" <<
            "6. Update student record\n" <<
            "7. Logout\n" <<
            ": ";
        adminOption = getInt(1, 7);
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
                            // Display all subject grades for student
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
                    saveClassrooms();
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
                        saveClassrooms();
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
                    saveClassrooms();
                    cout << "Deletion of class No." << classNum << " complete.";
                    break;
                }
                cout << "Deletion terminated.";
                break;
            case 3: // Adds a new class - Populated later by assigned teacher/s
                classroomRecords.push_back(classroom(classNum, {}));
                saveClassrooms();
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
                                " - Classroom No. " << childrenInfo[childIdx][1] <<
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
                cin >> loadedParentAccounts[parentIdx][3];
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
                                " - Classroom No." << classroomRecords[classIdx].classNumber <<
                                "\nCurrent Term Grades:";
                            // Subject grades
                            for (int subjectIdx = 0; subjectIdx < classroomRecords[classIdx].students[studentIdx].subjectGrades.size(); subjectIdx++) {
                                cout << "\n\t" <<
                                    subjectOrder[classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx][0]] <<
                                    ": " << gradeOptions[classroomRecords[classIdx].students[studentIdx].subjectGrades[subjectIdx][1]];
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
                                    ") - Class No." << classIdx;

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

// Creates a student record (Admin)
void adminRecordStudent() {
    bool makingRecords = true;

    while (makingRecords = true) {

        bool loopAgain = true;
        string studentsName;
        int studentsGender, subject, subjectGrade, control;
        vector<vector<int>> studentsSubjects; // Creates vectors within a vector that allows us to easily transfer data back to the structure
        vector<int> newSubject; // This allows us to store the subject and the grade under one variable

        system("cls"); // Clears all previous code in termonal to avoid screen clutter

        cout << "\nWhat is the students full name: ";
        cin.ignore(); // Allows the use of spaces in the string 
        getline(cin, studentsName);

        while (loopAgain = true) {
            cout << "\nWhat is the students gender (1. Male, 2. Female, 3. Other): ";
            cin >> studentsGender;
            if (studentsGender != 1 || 2 || 3)
                cout << "\nThat is not a valid. Please try again.";
            else
                loopAgain = false; // Breaks the loop if given a valid answer
        }

        for (subject = 0; subject < 3; subject++) {
            cout << "\n" << 3 - subject << " subjects left to register.";
            switch (subject) {
            case 0:
                cout << "\nCurrently registering Maths. ";
            case 1:
                cout << "\nCurrently registering English. ";
            case 2:
                cout << "\nCurrently registering History. ";
            }
            cout << "What was " << studentsName << "'s grade (0:NA, 1:A, 2:M, 3:E): ";
            cin >> subjectGrade;

            if (studentsGender != 0 || 1 || 2 || 3) {
                subject--;
                cout << "\nThat is not a valid. Please try again.";
            }
            else {
                newSubject.push_back(subject);
                newSubject.push_back(subjectGrade);
                studentsSubjects.push_back(newSubject);
            }
        }
        student newStudent = student(studentsName, studentsGender, studentsSubjects);

        cout << "\n\nDo you want to create another student record (1. Yes, 2. No):";
        cin >> control;
        if (control != 1)
            makingRecords = false;
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
    int teacherUser, teacherPass;
    cout << "Teacher Login Page\n";
    cout << "Please enter your username: ";
    cin >> teacherUser;
    //if
    cout << "Please enter your password: ";
    cin >> teacherPass;
    //if
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
    cout << "Mebee School Contact Information\n";
    cout << schoolName << endl;
    cout << schoolAddress << endl;
    cout << schoolPhoneNumber << endl;
    cout << schoolEmailAddress << endl;
}

void functionsEvents()
{
    int functionsEventsInput;

    cout << "\nMebee School Recent and Upcoming Events\n\n";
    cout << "Please choose one of the options below:\n";
    cout << "1. Recent Functions & Events\n2. Upcoming Functions & Events\n";
    cin >> functionsEventsInput;
    cout << endl;

    switch (functionsEventsInput)
    {
    case 1:
        cout << "+-------------------------+\n";
        cout << "Recent Mebee School Events\n";
        cout << "+-------------------------+\n";

        for (int i = 0; i < recentEvents.size(); i++) {
            for (int j = 0; j < recentEvents[i].size(); j++)
                cout << recentEvents[i][j] << " ";
            cout << endl;
        }
        break;
    case 2:
        cout << "+---------------------------+\n";
        cout << "Upcoming Mebee School Events\n";
        cout << "+---------------------------+\n";
        for (int i = 0; i < upcomingEvents.size(); i++) {
            for (int j = 0; j < upcomingEvents[i].size(); j++)
                cout << upcomingEvents[i][j] << " ";
            cout << endl;
        }
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
        cout << "+---------------------------+\n";
        cout << "           Term One          \n";
        cout << "+---------------------------+\n";
        for (int i = 0; i < term1Dates.size(); i++) {
            for (int j = 0; j < term1Dates[i].size(); j++)
                cout <<term1Dates[i][j] << "";
            cout << endl;
        }
        break;
    case 2:
        cout << "+---------------------------+\n";
        cout << "           Term Two          \n";
        cout << "+---------------------------+\n";
        for (int i = 0; i < term2Dates.size(); i++) {
            for (int j = 0; j < term2Dates[i].size(); j++)
                cout << term2Dates[i][j] << "";
            cout << endl;
        }
        break;
    case 3:
        cout << "+---------------------------+\n";
        cout << "          Term Three         \n";
        cout << "+---------------------------+\n";
        for (int i = 0; i < term3Dates.size(); i++) {
            for (int j = 0; j < term3Dates[i].size(); j++)
                cout << term3Dates[i][j] << "";
            cout << endl;
        }
        break;
    case 4:
        cout << "+---------------------------+\n";
        cout << "           Term Four         \n";
        cout << "+---------------------------+\n";
        for (int i = 0; i < term4Dates.size(); i++) {
            for (int j = 0; j < term4Dates[i].size(); j++)
                cout << term4Dates[i][j] << "";
            cout << endl;
        }
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
        cout << "\nWelcome to Mebee School\n\n"
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
            return 0;
        }
    }
}
