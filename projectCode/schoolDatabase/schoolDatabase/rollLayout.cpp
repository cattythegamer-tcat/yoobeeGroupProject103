struct student {
    string name;
    int gender;
    vector<int[2]> subjectGrades; // [SubjectID, gradeNum (0:NA, 1:A, 2:M, 3:E)
};

// These structs will be stored individually when saved to the hard-drive
struct classroom {
    int classNumber;
    vector<student> students;
};
// Overarching student records storage
vector<classroom> classroomRecords;