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
// Overarching student records storage
vector<classroom> classroomRecords = {
    classroom(1, {
        //                       A       B
        student("John Doe", 0, {{1, 1}, {2, 2}, {3, 3}}),
        student("Jane Doe", 1, {{1, 2}, {2, 3}, {3, 1}}),
        //                          C    
        student("Jack Doe", 0, {{1, 3}, {2, 1}, {3, 2}})
        }
    ),
    classroom(2, {
        //                       D
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
};

void recordExamples() {
    // Accessing student grades
    cout << classroomRecords[0].students[0].subjectGrades[0][0]; // A
    cout << classroomRecords[0].students[0].subjectGrades[1][0]; // B
    cout << classroomRecords[0].students[2].subjectGrades[0][1]; // C
    cout << classroomRecords[1].students[0].subjectGrades[0][0]; // D
    // Adding student to second class
    classroomRecords[1].students.push_back(
        student("Jello Doe", 1, { {1, 3}, {2, 1}, {3, 2} })
    );
}