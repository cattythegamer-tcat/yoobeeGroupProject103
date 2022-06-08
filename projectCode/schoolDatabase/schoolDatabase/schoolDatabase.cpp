#include <iostream>
using std::cout;
using std::cin;
using std::endl;

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
void parentRegister()
{
    cout << "Parent Registration Page\n";
}
void teacherRegister()
{
    cout << "Teacher Registration Page\n";
}
void parentLogin()
{
    cout << "Parent Login Page\n";
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

int main()
{
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
            case 2:
                parentLogin();
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
