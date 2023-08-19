#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h> // Include the header file for boolean data type

#define MAX_PASSWORD_LENGTH 25

// Starting of Voting functions

#define MAX_CANDIDATES 50

// Adding colors to the program

void black(){
  printf("\033[1;30m");
}

void red(){
  printf("\033[1;31m");
}

void green(){
  printf("\033[1;32m");
}

void yellow(){
  printf("\033[1;33m");
}

void blue(){
  printf("\033[1;34m");
}

void magenta(){
  printf("\033[1;35m");
}

void cyan(){
  printf("\033[1;36m");
}

void white(){
  printf("\033[1;37m");
}

void reset(){
  printf("\033[0m");
}
//--------------------------------

struct Candidate
{
    int candidateNumber;
    char name[100];
    int voteCount;  // Variable to store the vote count for each candidate
};

void createCandidates(struct Candidate candidates[], int numCandidates)
{
    int i;

    cyan();
    printf("\t\t\t\tEnter the details for each candidate:\n");
    reset();

    for (i = 0; i < numCandidates; i++)
    {
        printf("\t\t\t\tCandidate %d:\n", i + 1);
        candidates[i].candidateNumber = i + 1;
        printf("\t\t\t\tName: ");
        scanf("%s", candidates[i].name);
        candidates[i].voteCount = 0;  // Initialize vote count to 0
    }

    // Store candidate details and vote counts in a CSV file
    FILE *file = fopen("candidate_details.csv", "w");

    if (file == NULL)
    {
        red();
        printf("\t\t\t\tError opening file for writing!\n");
        reset();
        return;
    }

    fprintf(file, "Candidate Number,Name,Vote Count\n");
    for (i = 0; i < numCandidates; i++)
    {
        fprintf(file, "%d,%s,%d\n", candidates[i].candidateNumber, candidates[i].name, candidates[i].voteCount);
    }

    fclose(file);

    green();
    printf("\t\t\t\tCandidate details stored successfully!\n");
    reset();
}

void updateCandidateVotesInFile(struct Candidate candidates[], int numCandidates, int candidateNumber, int newVoteCount)
{
    FILE *file = fopen("candidate_details.csv", "r+");

    if (file == NULL)
    {
        red();
        printf("Error opening file for writing!\n");
        reset();
        return;
    }

    char line[256];
    char updatedFileContent[4096] = "";
    int lineNum = 1;
    while (fgets(line, sizeof(line), file))
    {
        if (lineNum > 1)
        {
            int currCandidateNumber;
            char name[100];
            int voteCount;
            sscanf(line, "%d,%[^,],%d", &currCandidateNumber, name, &voteCount);
            if (currCandidateNumber == candidateNumber)
            {
                voteCount = newVoteCount;
            }
            sprintf(line, "%d,%s,%d\n", currCandidateNumber, name, voteCount);
        }
        strcat(updatedFileContent, line);
        lineNum++;
    }

    rewind(file);
    fputs(updatedFileContent, file);
    fclose(file);
}

void readCandidateDetailsFromFile(struct Candidate candidates[], int *numCandidates, int *nextCandidateID)
{
    FILE *file = fopen("candidate_details.csv", "r");

    if (file == NULL)
    {
        red();
        printf("\t\t\t\tError opening file for reading!\n");
        reset();
        return;
    }

    char line[256];
    int lineNum = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (lineNum > 0)
        {
            int currCandidateNumber;
            char name[100];
            int voteCount;
            sscanf(line, "%d,%[^,],%d", &currCandidateNumber, name, &voteCount);
            candidates[lineNum - 1].candidateNumber = currCandidateNumber;
            strcpy(candidates[lineNum - 1].name, name);
            candidates[lineNum - 1].voteCount = voteCount;
        }
        lineNum++;
    }

    *numCandidates = lineNum - 1;
    *nextCandidateID = *numCandidates + 1;

    fclose(file);
}

void castVote(struct Candidate candidates[], int numCandidates)
{
    // Read candidate details from the CSV file
    int nextCandidateID;
    readCandidateDetailsFromFile(candidates, &numCandidates, &nextCandidateID);

    int i, vote;

    cyan();
    printf("\t\t\t\tEnter the candidate ID: ");
    reset();
    scanf("%d", &vote);

    if (vote >= 1 && vote <= numCandidates)
    {
        candidates[vote - 1].voteCount++;  // Increment the vote count for the selected candidate
        green();
        printf("\t\t\t\tVoted %s Successfully!\n", candidates[vote - 1].name);
        reset();
        updateCandidateVotesInFile(candidates, numCandidates, candidates[vote - 1].candidateNumber, candidates[vote - 1].voteCount);
    }
    else
    {
        red();
        printf("\t\t\t\tInvalid candidate ID! Please enter a valid candidate ID.\n");
        reset();
    }
}

void displayResults(struct Candidate candidates[], int numCandidates)
{
    green();
    printf("\t\t\t\tVote counts:\n");
    printf("\t\t\t\t+----+------------------------+-------+\n");
    printf("\t\t\t\t| ID |         Name           | Votes |\n");
    printf("\t\t\t\t+----+------------------------+-------+\n");

    for (int i = 0; i < numCandidates; i++)
    {
        printf("\t\t\t\t| %-2d | %-22s | %-5d |\n", candidates[i].candidateNumber, candidates[i].name, candidates[i].voteCount);
    }

    printf("\t\t\t\t+----+------------------------+-------+\n");
    reset();
}


void addCandidate(struct Candidate candidates[], int *numCandidates, int *nextCandidateID)
{
    system("cls");
    printHeading();
    if (*numCandidates >= MAX_CANDIDATES)
    {
        red();
        printf("\t\t\t\tCannot add more candidates. Maximum limit reached.\n");
        reset();
        return;
    }

    cyan();
    printf("\t\t\t\tEnter the details for the new candidate:\n");
    printf("\t\t\t\tCandidate %d:\n", *nextCandidateID);
    candidates[*numCandidates].candidateNumber = *nextCandidateID;
    printf("\t\t\t\tName: ");
    scanf("%s", candidates[*numCandidates].name);
    candidates[*numCandidates].voteCount = 0;  // Initialize vote count to 0
    reset();

    // Check if the candidate name already exists
    for (int i = 0; i < *numCandidates; i++)
    {
        if (strcmp(candidates[i].name, candidates[*numCandidates].name) == 0)
        {
            red();
            printf("\t\t\t\tCandidate already exists! Please enter a different name.\n");
            reset();
            return;
        }
    }

    (*numCandidates)++;
    (*nextCandidateID)++;

    // Update candidate details and vote counts in the CSV file
    FILE *file = fopen("candidate_details.csv", "a");

    if (file == NULL)
    {
        red();
        printf("Error opening file for writing!\n");
        reset();
        return;
    }

    fprintf(file, "%d,%s,%d\n", candidates[*numCandidates - 1].candidateNumber, candidates[*numCandidates - 1].name, candidates[*numCandidates - 1].voteCount);

    fclose(file);

    green();
    printf("\t\t\t\tNew candidate added successfully!\n");
    reset();
}

void showResultsAndWinner(struct Candidate candidates[], int numCandidates)
{
    system("cls");
    printHeading();
    yellow();
    printf("\t\t\t\tVote counts:\n");
    green();
    printf("\t\t\t\t+----+------------------------+-------+\n");
    printf("\t\t\t\t| ID |         Name           | Votes |\n");
    printf("\t\t\t\t+----+------------------------+-------+\n");

    int maxVotes = -1;
    int winnerCount = 0;
    int winnerIndices[MAX_CANDIDATES];

    for (int i = 0; i < numCandidates; i++)
    {
        printf("\t\t\t\t| %-2d | %-22s | %-5d |\n", candidates[i].candidateNumber, candidates[i].name, candidates[i].voteCount);

        if (candidates[i].voteCount > maxVotes)
        {
            maxVotes = candidates[i].voteCount;
            winnerCount = 1;
            winnerIndices[0] = i;
        }
        else if (candidates[i].voteCount == maxVotes)
        {
            winnerIndices[winnerCount] = i;
            winnerCount++;
        }
    }

    printf("\t\t\t\t+----+------------------------+-------+\n");
    reset();

    if (winnerCount == 0)
    {
        red();
        printf("\t\t\t\tNo winner found. No votes have been cast.\n");
        reset();
    }
    else if (winnerCount == 1)
    {
        magenta();
        printf("\n\t\t\t\tWinner:\n");
        printf("\t\t\t\tID: %d, Name: %s, Votes: %d\n\n", candidates[winnerIndices[0]].candidateNumber, candidates[winnerIndices[0]].name, candidates[winnerIndices[0]].voteCount);
        reset();
    }
    else
    {
        yellow();
        printf("\n\t\t\t\tDraw between the following candidates:\n");
        for (int i = 0; i < winnerCount; i++)
        {
            printf("\t\t\t\tID: %d, Name: %s, Votes: %d\n", candidates[winnerIndices[i]].candidateNumber, candidates[winnerIndices[i]].name, candidates[winnerIndices[i]].voteCount);
        }
        reset();
    }

}


void showAllCandidates(struct Candidate candidates[], int numCandidates)
{
    system("cls");
    printHeading();
    yellow();
    printf("\n\t\t\t\tAll Candidates:\n");
    printf("\t\t\t\t+----+------------------------+\n");
    printf("\t\t\t\t| ID |         Name           |\n");
    printf("\t\t\t\t+----+------------------------+\n");

    for (int i = 0; i < numCandidates; i++)
    {
        printf("\t\t\t\t| %-2d | %-22s |\n", candidates[i].candidateNumber, candidates[i].name);
    }

    printf("\t\t\t\t+----+------------------------+\n");
    printf("\n\n\t\t\t");
    reset();
}

// Ending of voting functions
typedef struct
{
    char User_Name[50];
    char Password[50];
    char Email_ID[50];
    char Phone_number[15];
} User;

typedef struct {
    User adminUser;
} Admin;

void Timer();
void printHeading();
void WrongInputTimer();
void RegisterMenu();
void loginMenu();
void createAdminAccount();
void adminLogin();
void CreateUser();
int UserLogin();
void CandidateOptions(User storedUser);
void AdminOptions(User storedAdmin);
void viewUsers(User storedAdmin);

int main()
{
    system("cls");
    printHeading();
    yellow();
    printf("\n\n\n\t\t\t\t\t\tWelcome to Online Voters Hub!");
    reset();
    cyan();
    printf("\n\n\t\t\t\t1.Login to existing account             2.New Registration");
    reset();
    green();
    printf("\n\n\n\t\t\t---->>> Choose an option : ");
    reset();
    int IndexChoice;
    scanf("%d",&IndexChoice);
    if(IndexChoice==1)
    {
        loginMenu();
    }
    else if(IndexChoice==2)
        RegisterMenu();
    else
    {
        red();
        printf("\n\t\t\t\t****************Invalid option!****************\n");
        reset();
        WrongInputTimer();
        main();
    }

    return 0;
}

void RegisterMenu()
{
    int choice;
    do
    {
        system("cls");
        printHeading();

        yellow();
        printf("\n\t\t\t    ~ ~ ~ ~ ~ ~ ~ ~ Welcome to Registration Menu! ~ ~ ~ ~ ~ ~ ~ ~\n");
        cyan();
        printf("\n\t\t\t\t1. Create User Account");
        printf("\n\t\t\t\t2. Back to Main Menu");
        green();
        printf("\n\n\t\t\t---->>> Choose an option : ");
        reset();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            CreateUser();
            break;
        case 2:
            main();
            return;
        default:
            red();
            printf("\n\t\t\t\t****************Invalid option!****************\n");
            reset();
            WrongInputTimer();
            break;
        }
    } while (choice != 2);
}

void printHeading()
{
    magenta();
    printf("\t\t\t+-------------------------------------------------------------------+\n");
    reset();
    white();
    printf("\t\t\t\033[0;35m|\033[0;0m                             VOTERS HUB                            \033[0;35m|\n");
    reset();
    magenta();
    printf("\t\t\t+-------------------------------------------------------------------+\n");
    reset();
}

void WrongInputTimer()
{
    for(int i=3;i>=0;i--)
    {
        red();
        printf("\t\t\t\tTry again in....%d",i);
        Sleep(1000);
        printf("\b\x1B[2K\r");      //To earase the current line
        reset();
    }
    printf("\n");
}

void loginMenu()
{
    system("cls");
    printHeading();
    yellow();
    printf("\n\t\t\t\t  ~ ~ ~ ~ ~ ~ ~ ~ Welcome to Login Page! ~ ~ ~ ~ ~ ~ ~ ~");
    reset();
    int choice;
    cyan();
    printf("\n\n\n\t\t\t\t1.User\n");
    printf("\t\t\t\t2.Admin\n");
    printf("\t\t\t\t3.Back to main menu\n");
    reset();
    green();
    printf("\n\t\t\t\tEnter the choice : ");
    reset();
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        system("cls");
        printHeading();
        yellow();
        printf("\n\t\t\t\t  ~ ~ ~ ~ ~ ~ ~ Welcome to User Login Page! ~ ~ ~ ~ ~ ~ ~\n");
        reset();
        UserLogin();
        break;
    case 2:
        system("cls");
        printHeading();
        yellow();
        printf("\n\t\t\t\t  ~ ~ ~ ~ ~ ~ ~ Welcome to Admim Login Page! ~ ~ ~ ~ ~ ~ ~\n");
        reset();
        adminLogin();
        break;
    case 3:
        main();
        break;
    default:
        red();
        printf("\n\t\t\t\t****************Invalid option!****************\n");
        reset();
        WrongInputTimer();
        loginMenu();
        break;
    }
}

bool isAdminUsernameExists(const char* username)
{
    FILE* file = fopen("admin.bin", "rb");
    if (file == NULL)
    {
        red();
        printf("\t\t\t\tFailed to open file.\n");
        reset();
        return false;
    }

    User existingUser;
    while (fread(&existingUser, sizeof(User), 1, file) == 1)
    {
        if (strcmp(existingUser.User_Name, username) == 0)
        {
            fclose(file);
            return true; // Username already exists
        }
    }

    fclose(file);
    return false; // Username does not exist
}

void createAdminAccount(User storedAdmin)
{
    system("cls");
    printHeading();
    User adminUser;
    yellow();
    printf("\n\t\t\t\t--- Create Admin Account ---\n");
    cyan();
    printf("\n\t\t\t\tUsername: ");
    scanf("%s", adminUser.User_Name);

    // Check if the username already exists
    if (isAdminUsernameExists(adminUser.User_Name))
    {
        red();
        printf("\n\t\t\t\tUsername already exists.\n\n");
        reset();
        WrongInputTimer();
        CandidateOptions(storedAdmin);
    }

    cyan();
    printf("\t\t\t\tPassword: ");
    char ch;
    int i = 0;
    while (1)
    {
        ch = getch();

        if (ch == '\r') // Check for the Enter key (carriage return)
        {
            adminUser.Password[i] = '\0'; // Null-terminate the password string
            break;
        }
        else if (ch == '\b')    // Check for the Backspace key
        {
            if (i > 0)
            {
                i--;
                putch('\b'); // Move the cursor back
                putch(' ');  // Erase the character
                putch('\b'); // Move the cursor back again
            }
        }
        else
        {
            if (i < MAX_PASSWORD_LENGTH - 1)
            {
                adminUser.Password[i] = ch;
                i++;
                putch('*'); // Print a star instead of the actual character
            }
        }
    }

    printf("\n\t\t\t\tEmail: ");
    scanf("%s", adminUser.Email_ID);

    printf("\t\t\t\tPhone Number: ");
    scanf("%s", adminUser.Phone_number);
    reset();
    // Open the file in binary append mode
    FILE *file = fopen("admin.bin", "ab");
    if (file == NULL)
    {
        red();
        printf("Failed to open file.\n");
        reset();
        return;
    }

    // Write the admin details to the file
    fwrite(&adminUser, sizeof(User), 1, file);

    fclose(file);
    green();
    printf("Admin account created successfully!\n");
    reset();
    Timer();
    main();
}

// Function to perform admin login
void adminLogin()
{
    User adminUser;

    cyan();
    printf("\n\t\t\t\tUsername: ");
    scanf("%s", adminUser.User_Name);

    printf("\t\t\t\tPassword: ");

    char ch;
    int i = 0;
    while (1)
    {
        ch = getch();

        if (ch == '\r')         // Check for the Enter key (carriage return)
        {
            adminUser.Password[i] = '\0'; // Null-terminate the password string
            break;
        }
        else if (ch == '\b')    // Check for the Backspace key
        {
            if (i > 0)
            {
                i--;
                putch('\b'); // Move the cursor back
                putch(' ');  // Erase the character
                putch('\b'); // Move the cursor back again
            }
        }
        else
        {
            if (i < MAX_PASSWORD_LENGTH - 1)
            {
                adminUser.Password[i] = ch;
                i++;
                putch('*'); // Print a star instead of the actual character
            }
        }
    }
    reset();

    // Open the admin file in binary read mode
    FILE *file = fopen("admin.bin", "rb");
    if (file == NULL)
    {
        red();
        printf("\n\n\t\t\t\tFailed to open file.\n");
        reset();
        WrongInputTimer();
        main();
        return;
    }

    User storedAdmin;
    int found = 0;
    // Read each admin from the file and check for a match
    while (fread(&storedAdmin, sizeof(User), 1, file))
    {
        if (strcmp(adminUser.User_Name, storedAdmin.User_Name) == 0 && strcmp(adminUser.Password, storedAdmin.Password) == 0)
        {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found)
    {
        green();
        printf("\n\n\t\t\t\tAdmin login successful!\n");
        reset();
        Timer();
        AdminOptions(storedAdmin);
        // Place your admin-specific code here
    }
    else
    {
        red();
        printf("\n\n\t\t\t\tInvalid username or password.\n\n");
        reset();
        WrongInputTimer();
        loginMenu();
    }
}

// Function to perform candidate login
int UserLogin()
{
    User candidateUser;
    cyan();
    printf("\n\n\t\t\t\tUsername: ");
    scanf("%s", candidateUser.User_Name);

    printf("\t\t\t\tPassword: ");
    char ch;
    int i = 0;
    while (1)
    {
        ch = getch();

        if (ch == '\r')     // Check for the Enter key (carriage return)
        {
            candidateUser.Password[i] = '\0'; // Null-terminate the password string
            break;
        }
        else if (ch == '\b') // Check for the Backspace key
        {
            if (i > 0)
            {
                i--;
                putch('\b'); // Move the cursor back
                putch(' ');  // Erase the character
                putch('\b'); // Move the cursor back again
            }
        }
        else
        {
            if (i < MAX_PASSWORD_LENGTH - 1)
            {
                candidateUser.Password[i] = ch;
                i++;
                putch('*'); // Print a star instead of the actual character
            }
        }
    }
    reset();

    // Open the file in binary read mode
    FILE *file = fopen("users.dat", "rb");
    if (file == NULL)
    {
        red();
        printf("\n\n\t\t\t\tFailed to open file.\n");
        reset();
        WrongInputTimer();
        main();
        return 0;
    }

    User storedUser;
    int found = 0;
    // Read each user from the file and check for a match
    while (fread(&storedUser, sizeof(User), 1, file))
    {
        if (strcmp(candidateUser.User_Name, storedUser.User_Name) == 0 && strcmp(candidateUser.Password, storedUser.Password) == 0)
        {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found)
    {
        green();
        printf("\n\n\t\t\t\tCandidate login successful!\n");
        reset();
        Timer();
        CandidateOptions(storedUser);
        // Place your candidate-specific code here
    }
    else
    {
        red();
        printf("\n\n\t\t\t\tInvalid username or password.\n\n");
        reset();
        WrongInputTimer();
        loginMenu();
    }

    return found;
}
void Timer()
{
    for(int i=3;i>=0;i--)
    {
        magenta();
        printf("\t\t\t\tTaking to next page in....%d",i);
        Sleep(1000);                //To delay code execution for 1 second
        printf("\b\x1B[2K\r");      //To earase the current line
        reset();
    }
    printf("\n");
}

bool isUsernameExists(const char* username)
{
    FILE* file = fopen("users.dat", "rb");
    if (file == NULL)
    {
        return false;
    }

    User existingUser;
    while (fread(&existingUser, sizeof(User), 1, file) == 1)
    {
        if (strcmp(existingUser.User_Name, username) == 0)
        {
            fclose(file);
            return true; // Username already exists
        }
    }

    fclose(file);
    return false; // Username does not exist
}

void CreateUser()
{
    system("cls");
    printHeading();
    User newUser;
    yellow();
    printf("\n\t\t\t\t--- Create User Account ---\n");      // Creating new user

    cyan();
    printf("\t\t\t\tUsername: ");
    scanf("%s", newUser.User_Name);

    // Check if the username already exists
    if (isUsernameExists(newUser.User_Name))
    {
        red();
        printf("\n\t\t\t\tUsername already exists.\n");
        printf("\n\t\t\t\tSelect any other username.\n");
        reset();
        WrongInputTimer();
        return;
    }

    cyan();
    printf("\t\t\t\tPassword: ");
    reset();

    char ch;
    int i = 0;
    while (1)
    {
        ch = getch();

        if (ch == '\r')         // Check for the Enter key (carriage return)
        {
            newUser.Password[i] = '\0'; // Null-terminate the password string
            break;
        }
        else if (ch == '\b')    // Check for the Backspace key
        {
            if (i > 0)
            {
                i--;
                putch('\b'); // Move the cursor back
                putch(' ');  // Erase the character
                putch('\b'); // Move the cursor back again
            }
        }
        else
        {
            if (i < MAX_PASSWORD_LENGTH - 1)
            {
                newUser.Password[i] = ch;
                i++;
                putch('*'); // Print a star instead of the actual character
            }
        }
    }

    cyan();
    printf("\n\t\t\t\tEmail: ");
    scanf("%s", newUser.Email_ID);

    printf("\t\t\t\tPhone Number: ");
    scanf("%s", newUser.Phone_number);
    reset();

    // Open the file in binary append mode
    FILE *file = fopen("users.dat", "ab");
    if (file == NULL)
    {
        printf("\t\t\t\tFailed to open file.\n");
        return;
    }

    // Write the user details to the file
    fwrite(&newUser, sizeof(User), 1, file);

    fclose(file);
    green();
    printf("\n\n\t\t\t\tUser account created successfully!\n\n\n");
    reset();
    Timer();
}

void ChangePassword(User* user)
{
    system("cls");
    printHeading();

    char currentPassword[MAX_PASSWORD_LENGTH];
    char newPassword[MAX_PASSWORD_LENGTH];
    char confirmPassword[MAX_PASSWORD_LENGTH];

    yellow();
    printf("\n\t\t\t\t--- Change Password ---\n");

    cyan();
    printf("\t\t\t\tUsername: %s\n", user->User_Name);

    printf("\t\t\t\tCurrent Password: ");
    reset();

    char ch;
    int i = 0;
    while (1)
    {
        ch = getch();

        if (ch == '\r')         // Check for the Enter key (carriage return)
        {
            currentPassword[i] = '\0'; // Null-terminate the password string
            break;
        }
        else if (ch == '\b')    // Check for the Backspace key
        {
            if (i > 0)
            {
                i--;
                putch('\b'); // Move the cursor back
                putch(' ');  // Erase the character
                putch('\b'); // Move the cursor back again
            }
        }
        else
        {
            if (i < MAX_PASSWORD_LENGTH - 1)
            {
                currentPassword[i] = ch;
                i++;
                putch('*'); // Print a star instead of the actual character
            }
        }
    }
    // Check if the current password matches
    if (strcmp(currentPassword, user->Password) != 0)
    {
        red();
        printf("\n\t\t\t\tIncorrect password. Password change failed.\n\n");
        reset();
        WrongInputTimer();
        return;
    }

    i=0;
    cyan();
    printf("\n\t\t\t\tNew Password: ");
    reset();

    while (1)
    {
        ch = getch();

        if (ch == '\r')     // Check for the Enter key (carriage return)
        {
            newPassword[i] = '\0'; // Null-terminate the password string
            break;
        }
        else if (ch == '\b')    // Check for the Backspace key
        {
            if (i > 0)
            {
                i--;
                putch('\b'); // Move the cursor back
                putch(' ');  // Erase the character
                putch('\b'); // Move the cursor back again
            }
        }
        else
        {
            if (i < MAX_PASSWORD_LENGTH - 1)
            {
                newPassword[i] = ch;
                i++;
                putch('*'); // Print a star instead of the actual character
            }
        }
    }

    i=0;
    cyan();
    printf("\n\t\t\t\tConfirm New Password: ");
    reset();

    while (1)
    {
        ch = getch();

        if (ch == '\r')         // Check for the Enter key (carriage return)
        {
            confirmPassword[i] = '\0'; // Null-terminate the password string
            break;
        }
        else if (ch == '\b')
        { // Check for the Backspace key
            if (i > 0)
            {
                i--;
                putch('\b'); // Move the cursor back
                putch(' ');  // Erase the character
                putch('\b'); // Move the cursor back again
            }
        }
        else
        {
            if (i < MAX_PASSWORD_LENGTH - 1)
            {
                confirmPassword[i] = ch;
                i++;
                putch('*'); // Print a star instead of the actual character
            }
        }
    }

    // Check if the new password and confirm password match
    if (strcmp(newPassword, confirmPassword) != 0)
    {
        red();
        printf("\n\t\t\t\tNew passwords do not match. Password change failed.\n\n");
        reset();
        WrongInputTimer();
        return;
    }

    // Update the password in the user struct
    strcpy(user->Password, newPassword);

    // Open the file in binary read/write mode
    FILE* file = fopen("users.dat", "rb+");
    if (file == NULL)
    {
        red();
        printf("\t\t\t\tFailed to open file.\n");
        reset();
        return;
    }

    // Search for the user in the file and update the password
    User existingUser;
    while (fread(&existingUser, sizeof(User), 1, file) == 1)
    {
        if (strcmp(existingUser.User_Name, user->User_Name) == 0)
        {
            // Move the file pointer back by the size of User struct
            fseek(file, -sizeof(User), SEEK_CUR);

            // Write the updated user details to the file
            fwrite(user, sizeof(User), 1, file);

            fclose(file);
            printf("\n\t\t\t\tPassword changed successfully!\n\n");
            Timer();
            return;
        }
    }

    fclose(file);

    red();
    printf("\n\t\t\t\tFailed to find the user in the file. Password change failed.\n\n");
    reset();
}


void CandidateOptions(User storedUser)
{
    struct Candidate candidates[MAX_CANDIDATES];
    int numCandidates = 0, choice, nextCandidateID = 1;

    // Read candidate details from the CSV file
    readCandidateDetailsFromFile(candidates, &numCandidates, &nextCandidateID);

    system("cls");
    printHeading();
    yellow();
    printf("\n\t\t\t\t ~ ~ ~ ~ ~ ~ ~ ~ Welcome, %s! ~ ~ ~ ~ ~ ~ ~ ~\n\n", storedUser.User_Name);
    reset();

    // Print user details
    int option;
    do
    {
        cyan();
        printf("\n\t\t\t\t1. Show Candidates");
        printf("\n\t\t\t\t2. Vote Candidate");
        printf("\n\t\t\t\t3. Change Password");
        printf("\n\t\t\t\t4. View Profile");
        printf("\n\t\t\t\t5. Back to Main Menu");
        green();
        printf("\n\n\t\t\t\tChoose an option: ");
        scanf("%d", &option);
        reset();

        switch (option)
        {
            case 1:
                yellow();
                printf("\n\n\t\t\t\t--- Showing Candidates ---\n");
                reset();
                showAllCandidates(candidates, numCandidates);
                system("pause");
                CandidateOptions(storedUser);
                break;
            case 2:
                yellow();
                printf("\n\n\t\t\t\t--- Voting for a Candidate ---\n");
                reset();
                showAllCandidates(candidates, numCandidates);
                printf("\n\n");
                castVote(candidates, numCandidates);
                printf("\t\t\t\t");
                system("pause");
                CandidateOptions(storedUser);
                break;
            case 3:
                yellow();
                printf("\n\n\t\t\t\t--- Changing Password ---\n");
                reset();
                ChangePassword(&storedUser);
                CandidateOptions(storedUser);
                break;
            case 4:
                system("cls");
                printHeading();
                yellow();
                printf("\n\t\t\t\t----Personal Details----\n\n");
                reset();
                cyan();
                printf("\t\t\t\tName        : %s\n", storedUser.User_Name);
                printf("\t\t\t\tEmail       : %s\n", storedUser.Email_ID);
                printf("\t\t\t\tPhone Number: %s\n", storedUser.Phone_number);
                reset();
                green();
                printf("\n\n\t\t\tClick any key to return...");
                reset();
                getch();
                CandidateOptions(storedUser);
                break;
            case 5:
                loginMenu();
                break;
            default:
                red();
                printf("\n\t\t\t\t****************Invalid option!****************\n");
                reset();
                WrongInputTimer();
                CandidateOptions(storedUser);
                break;
        }
    } while (option != 4);
}

void AdminOptions(User storedAdmin)
{
    struct Candidate candidates[MAX_CANDIDATES];
    int numCandidates = 0, choice, nextCandidateID = 1;

    // Read candidate details from the CSV file
    readCandidateDetailsFromFile(candidates, &numCandidates, &nextCandidateID);
    system("cls");
    printHeading();
    yellow();
    printf("\n\t\t\t\t ~ ~ ~ ~ ~ ~ ~ ~ Welcome, %s! ~ ~ ~ ~ ~ ~ ~ ~\n\n", storedAdmin.User_Name);
    reset();

    // Print user details
    cyan();
    printf("\t\t\t\tEmail: %s\n", storedAdmin.Email_ID);
    printf("\t\t\t\tPhone Number: %s\n", storedAdmin.Phone_number);
    reset();
    int option;
    do
    {
        yellow();
        printf("\n\t\t\t\t1. Show Candidates");
        printf("\n\t\t\t\t2. Show Results");
        printf("\n\t\t\t\t3. Add new candidate");
        printf("\n\t\t\t\t4. Create new Admin Account");
        printf("\n\t\t\t\t5. View Registered Users");
        printf("\n\t\t\t\t6. Back to Main Menu");
        reset();
        green();
        printf("\n\n\t\t\t\tChoose an option: ");
        reset();
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                yellow();
                printf("\n\n\t\t\t\t  --- Showing Candidates ---\n\n");
                reset();
                showAllCandidates(candidates, numCandidates);
                system("pause");
                AdminOptions(storedAdmin);
                break;
            case 2:
                yellow();
                printf("\n\n\t\t\t\t--- Voting for a Candidate ---\n");
                reset();
                showResultsAndWinner(candidates, numCandidates);
                printf("\t\t\t\t");
                system("pause");
                AdminOptions(storedAdmin);
                break;
            case 3:
                addCandidate(candidates, &numCandidates, &nextCandidateID);
                printf("\t\t\t\t");
                system("pause");
                AdminOptions(storedAdmin);
                break;
            case 4:
                createAdminAccount(storedAdmin);
                break;
            case 5:
                viewUsers(storedAdmin);
                break;
            case 6:
                loginMenu();
                break;
            default:
                red();
                printf("\n\t\t\t\tInvalid option!\n");
                reset();
                WrongInputTimer();
                AdminOptions(storedAdmin);
                break;
        }
    } while (option != 4);
}

void viewUsers(User storedAdmin)
{
    system("cls");
    printHeading();
    yellow();
    printf("\n\t\t\t\t--- View Users ---\n");
    reset();

    // Open the file in binary read mode
    FILE *file = fopen("users.dat", "rb");
    if (file == NULL)
    {
        red();
        printf("\n\t\t\t\tFailed to open file.\n");
        reset();
        return;
    }

    User user;
    int count = 0;

    // Print the table headers
    cyan();
    printf("\n\t\t\t+-----------------------------------------------------------+\n");
    printf("\t\t\t| %-10s | %-20s | %-20s  |\n", "Username", "Email", "Phone Number");
    printf("\t\t\t+-----------------------------------------------------------+\n");
    reset();

    // Read each user from the file and display their data in the table
    while (fread(&user, sizeof(User), 1, file) == 1)
    {
        printf("\t\t\t| %-10s | %-20s | %-20s  |\n", user.User_Name, user.Email_ID, user.Phone_number);
        count++;
    }

    fclose(file);

    // Print the footer with the total number of users
    cyan();
    printf("\t\t\t+-----------------------------------------------------------+\n");
    printf("\n\t\t\tTotal Users: %d\n", count);
    reset();
    printf("\n\t\t\t\t");
    system("pause");
    AdminOptions(storedAdmin);
}
